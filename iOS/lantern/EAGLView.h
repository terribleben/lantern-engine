//
//  EAGLView.h
//  lantern
//

#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "Lantern.h"

@interface EAGLView : UIView
{
@private
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    GLuint defaultFramebuffer, colorRenderbuffer;
}

@property (nonatomic, strong) EAGLContext* context;
@property (nonatomic, readonly) CGSize frameBufferSize;
@property (nonatomic, assign) Lantern *lantern;

- (void) setScaleFactor: (CGFloat)scale;
- (void) setFramebuffer;
- (BOOL) presentFramebuffer;

@end
