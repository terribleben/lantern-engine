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

- (UIImage *)getSnapshot
{
    // from here: https://developer.apple.com/library/ios/qa/qa1704/_index.html
    
    GLint backingWidth, backingHeight;
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    NSInteger x = 0, y = 0, width = backingWidth, height = backingHeight;
    NSInteger dataLength = width * height * 4;
    GLubyte *data = (GLubyte*)malloc(dataLength * sizeof(GLubyte));
    
    // Read pixel data from the framebuffer
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // Create a CGImage with the pixel data
    // If your OpenGL ES content is opaque, use kCGImageAlphaNoneSkipLast to ignore the alpha channel
    // otherwise, use kCGImageAlphaPremultipliedLast
    CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, data, dataLength, NULL);
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    CGImageRef iref = CGImageCreate(width, height, 8, 32, width * 4, colorspace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
                                    ref, NULL, true, kCGRenderingIntentDefault);
    
    // support retina
    NSInteger widthInPoints, heightInPoints;
    if (UIGraphicsBeginImageContextWithOptions) {
        CGFloat scale = self.contentScaleFactor;
        widthInPoints = width / scale;
        heightInPoints = height / scale;
        UIGraphicsBeginImageContextWithOptions(CGSizeMake(widthInPoints, heightInPoints), NO, scale);
    } else {
        widthInPoints = width;
        heightInPoints = height;
        UIGraphicsBeginImageContext(CGSizeMake(widthInPoints, heightInPoints));
    }
    
    CGContextRef cgcontext = UIGraphicsGetCurrentContext();
    
    // UIKit coordinate system is upside down to GL/Quartz coordinate system
    CGContextSetBlendMode(cgcontext, kCGBlendModeCopy);
    CGContextDrawImage(cgcontext, CGRectMake(0.0, 0.0, widthInPoints, heightInPoints), iref);
    UIImage *result = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    // Clean up
    free(data);
    CFRelease(ref);
    CFRelease(colorspace);
    CGImageRelease(iref);
    
    return result;
}

@end
