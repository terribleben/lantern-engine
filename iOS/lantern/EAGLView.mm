//
//  EAGLView.m
//  lantern
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"
#import "Lantern.h"

@interface EAGLView ()

- (void) createFramebuffer;
- (void) deleteFramebuffer;

@end

@implementation EAGLView

@synthesize context = _context;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithCoder: (NSCoder*)coder
{
	if (self = [super initWithCoder:coder]) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
    }
    
    return self;
}

- (void) dealloc
{
    [self deleteFramebuffer];    
    
    // [super dealloc];
}

- (void) setContext: (EAGLContext *)newContext
{
    if (_context != newContext) {
        [self deleteFramebuffer];

        _context = newContext;
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void) setScaleFactor: (CGFloat)scale
{
    self.contentScaleFactor = scale;
    CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
    eaglLayer.contentsScale = scale;
}

- (void) createFramebuffer
{
    if (_context && !defaultFramebuffer) {
        [EAGLContext setCurrentContext:_context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
        
        if (_lantern)
            _lantern->setDimensions(framebufferWidth, framebufferHeight);
		
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
}

- (void) deleteFramebuffer
{
    if (_context) {
        [EAGLContext setCurrentContext:_context];
        
        if (defaultFramebuffer) {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer) {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
    }
}

- (void) setFramebuffer
{
    if (_context) {
        [EAGLContext setCurrentContext:_context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL) presentFramebuffer
{
    BOOL success = NO;
    
    if (_context) {
        [EAGLContext setCurrentContext:_context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        success = [_context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (CGSize) frameBufferSize
{
    return CGSizeMake(framebufferWidth, framebufferHeight);
}

- (void) layoutSubviews
{
    // will be recreated at the beginning of the next setFramebuffer call.
    [self deleteFramebuffer];
}

@end
