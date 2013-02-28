//
//  ViewController.m
//  lantern
//

#import "ViewController.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "EAGLView.h"

#import "Lantern.h"

NSString* const kLanternConfigAccelerometerEnabled = @"accelerometer_enabled";


@interface ViewController ()
{
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink* displayLink;
    NSDictionary* lanternConfig;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

@property (nonatomic, retain) EAGLContext* context;
@property (nonatomic, assign) CADisplayLink* displayLink;

- (void) draw;

@end

@implementation ViewController

@synthesize animating, context, displayLink;

- (void) viewDidLoad
{
    // load config
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"LanternConfig.plist"];
    lanternConfig = [[NSDictionary alloc] initWithContentsOfFile:path];
    
    // set up GL context
	EAGLContext* _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!_context)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:_context])
        NSLog(@"Failed to set ES context current");
    
	self.context = _context;
	[_context release];
    
    // retina display?
    // scale should be 2.0 for retina displays.
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        [(EAGLView*)self.view setScaleFactor:[UIScreen mainScreen].scale];
        Lantern::getInstance().setScale([UIScreen mainScreen].scale);
    }
    
    [self.view setMultipleTouchEnabled:YES];
	
    [(EAGLView*)self.view setContext:context];
    [(EAGLView*)self.view setFramebuffer];
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    // enable the accelerometer?
    NSString* accelParam = [lanternConfig objectForKey:kLanternConfigAccelerometerEnabled];
    if (accelParam && [accelParam intValue] == 1) {
        [[UIAccelerometer sharedAccelerometer] setDelegate:self];
    }
    
    // init lantern
    Lantern::getInstance().init();
}

- (void) dealloc
{
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    [context release];
    
    [super dealloc];
    
    if (lanternConfig) {
        [lanternConfig release];
        lanternConfig = nil;
    }
    
    Lantern::getInstance().stop();
}

- (void) viewWillAppear: (BOOL)animated
{
    [self startAnimating];
    [super viewWillAppear:animated];
}

- (void) viewWillDisappear:(BOOL)animated
{
    [self stopAnimating];
    [super viewWillDisappear:animated];
}

- (void) viewDidUnload
{    
    [super viewDidUnload];
    
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;
    
    if (lanternConfig) {
        [lanternConfig release];
        lanternConfig = nil;
    }
}

- (NSInteger) animationFrameInterval
{
    return animationFrameInterval;
}

- (void) setAnimationFrameInterval: (NSInteger)frameInterval
{
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimating];
            [self startAnimating];
        }
    }
}

- (void) startAnimating
{
    if (!animating) {
        CADisplayLink* _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(draw)];
        [_displayLink setFrameInterval:animationFrameInterval];
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = _displayLink;
        
        animating = YES;
    }
}

- (void) stopAnimating
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = NO;
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

- (void) didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
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
