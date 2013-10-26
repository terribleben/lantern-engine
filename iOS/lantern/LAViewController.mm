//
//  LAViewController.m
//  lantern
//

#import "LAViewController.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "EAGLView.h"

#import "Lantern.h"

NSString* const kLanternConfigAccelerometerEnabled = @"accelerometer_enabled";


@interface LAViewController ()
{
    NSInteger animationFrameInterval;
    NSDictionary* lanternConfig;
}

@property (nonatomic, readonly, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

@property (nonatomic, strong) EAGLContext* context;
@property (nonatomic, assign) CADisplayLink* displayLink;

- (void) draw;

@end

@implementation LAViewController

- (void) viewDidLoad
{
    // load config
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"LanternConfig.plist"];
    lanternConfig = [[NSDictionary alloc] initWithContentsOfFile:path];
    
    // set up GL context
	_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!_context)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:_context])
        NSLog(@"Failed to set ES context current");
    
    // init lantern
    Lantern::getInstance().init();
    
    // retina display?
    // scale should be 2.0 for retina displays.
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        [(EAGLView*)self.view setScaleFactor:[UIScreen mainScreen].scale];
        Lantern::getInstance().setScale([UIScreen mainScreen].scale);
    }
    
    [self.view setMultipleTouchEnabled:YES];
	
    [(EAGLView*)self.view setContext:_context];
    [(EAGLView*)self.view setFramebuffer];
    
    _animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    // enable the accelerometer?
    NSString* accelParam = [lanternConfig objectForKey:kLanternConfigAccelerometerEnabled];
    if (accelParam && [accelParam intValue] == 1) {
        [[UIAccelerometer sharedAccelerometer] setDelegate:self];
    }
}

- (void) dealloc
{
    if (_context) {
        if ([EAGLContext currentContext] == _context)
            [EAGLContext setCurrentContext:nil];
        _context = nil;
    }
    
    if (lanternConfig) {
        lanternConfig = nil;
    }
    
    Lantern::getInstance().stop();
    
    // [super dealloc];
}

- (void) viewWillAppear: (BOOL)animated
{
    // gain focus event
    Event e(LANTERN_EVENT_GAIN_FOCUS, 0, NULL);
    Lantern::getInstance().event(e);
    
    // restart the graphics loop
    [self startAnimating];
    [super viewWillAppear:animated];
}

- (void) viewWillDisappear:(BOOL)animated
{
    // lose focus event
    Event e(LANTERN_EVENT_LOSE_FOCUS, 0, NULL);
    Lantern::getInstance().event(e);
    
    // stop the graphics loop
    [self stopAnimating];
    [super viewWillDisappear:animated];
}

- (void) viewDidUnload
{
    if (_context) {
        if ([EAGLContext currentContext] == _context)
            [EAGLContext setCurrentContext:nil];
        _context = nil;
    }
    
    if (lanternConfig) {
        lanternConfig = nil;
    }
    
    Lantern::getInstance().stop();
    
    [super viewDidUnload];
}

- (NSInteger) animationFrameInterval
{
    return animationFrameInterval;
}

- (void) setAnimationFrameInterval: (NSInteger)frameInterval
{
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (_animating) {
            [self stopAnimating];
            [self startAnimating];
        }
    }
}

- (void) startAnimating
{
    if (!_animating) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(draw)];
        [_displayLink setFrameInterval:animationFrameInterval];
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        _animating = YES;
    }
}

- (void) stopAnimating
{
    if (_animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        _animating = NO;
    }
}

- (void) draw {
    [(EAGLView*)self.view setFramebuffer];
    
    Lantern::getInstance().draw();
	
	[(EAGLView*)self.view presentFramebuffer];
}

- (void) touchesBegan: (NSSet*)touches withEvent: (UIEvent*)event
{
    CGSize screenSize = [(EAGLView*)self.view frameBufferSize];
    
    for (UITouch* touch in touches) {
        CGPoint location = [touch locationInView:self.view];
        
        location.y = (screenSize.height / [UIScreen mainScreen].scale) - location.y; // cocoa uses inverted y axis
        
        // touch
        float touchParam[] = { location.x, location.y };
        Event touchEvent(LANTERN_EVENT_TOUCH_DOWN, (unsigned int)touch, touchParam);
        Lantern::getInstance().event(touchEvent);
    }
}

- (void) touchesMoved: (NSSet*)touches withEvent: (UIEvent*)event
{
    CGSize screenSize = [(EAGLView*)self.view frameBufferSize];
    
    for (UITouch* touch in touches) {
        CGPoint location = [touch locationInView:self.view];
        
        location.y = (screenSize.height / [UIScreen mainScreen].scale) - location.y; // cocoa uses inverted y axis
        
        // touch
        float touchParam[] = { location.x, location.y };
        Event touchEvent(LANTERN_EVENT_TOUCH_MOVE, (unsigned int)touch, touchParam);
        Lantern::getInstance().event(touchEvent);
    }
}

- (void) touchesEnded: (NSSet*)touches withEvent: (UIEvent*)event
{
    CGSize screenSize = [(EAGLView*)self.view frameBufferSize];
    
    for (UITouch* touch in touches) {
        CGPoint location = [touch locationInView:self.view];
        
        location.y = (screenSize.height / [UIScreen mainScreen].scale) - location.y; // cocoa uses inverted y axis
        
        // touch
        float touchParam[] = { location.x, location.y };
        Event touchEvent(LANTERN_EVENT_TOUCH_UP, (unsigned int)touch, touchParam);
        Lantern::getInstance().event(touchEvent);
    }
}

- (void) touchesCancelled: (NSSet*)touches withEvent: (UIEvent*)event
{
    [self touchesEnded:touches withEvent:event];
}

- (void) accelerometer: (UIAccelerometer*)accelerometer didAccelerate: (UIAcceleration*)acceleration
{
    // accel event
    float accelParam[] = { acceleration.x, acceleration.y, acceleration.z };
    Event accelEvent(LANTERN_EVENT_ACCEL, (unsigned int)accelerometer, accelParam);
    Lantern::getInstance().event(accelEvent);
}

- (BOOL) shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
    } else {
        return YES;
    }
}

@end