//
//  EAGLView.h
//  lantern
//

#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface EAGLView : UIView
{
@private
    EAGLContext *context;
    
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    GLuint defaultFramebuffer, colorRenderbuffer;
}

@property (nonatomic, retain) EAGLContext* context;
@property (nonatomic, readonly) CGSize frameBufferSize;

- (void) setScaleFactor: (CGFloat)scale;
- (void) setFramebuffer;
- (BOOL) presentFramebuffer;

@end
