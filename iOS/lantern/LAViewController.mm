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
#import "LAAudio.h"

#import "Lantern.h"

NSString* const kLanternConfigAccelerometerEnabled = @"accelerometer_enabled";
NSString* const kLanternConfigAudioEnabled = @"audio_enabled";

void audioCallback(Sample* buffer, unsigned int numFrames, void* userData);

@interface LAViewController ()
{
    NSInteger animationFrameInterval;
    NSDictionary* lanternConfig;
    BOOL isLanternInitialized;
    Lantern *_lantern;
    LAAudio *theAudio;
}

- (void) startAnimation;
- (void) stopAnimation;

@property (nonatomic, readonly, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

@property (nonatomic, strong) EAGLContext* context;
@property (nonatomic, assign) CADisplayLink* displayLink;

- (void) draw;
- (void) initializeLantern;

@end

@implementation LAViewController

- (id) init
{
    if (self = [super init]) {
        isLanternInitialized = NO;
    }
    return self;
}

- (void) setLantern:(void*)lantern
{
    if (_lantern) {
        delete _lantern;
        _lantern = NULL;
    }
    
    _lantern = (Lantern*) lantern;
    if (self.view)
        ((EAGLView*)self.view).lantern = _lantern;
}

- (void*) lantern
{
    return _lantern;
}

- (void) initializeLantern
{
    if (!_lantern) {
        self.lantern = new Lantern();
    }
    
    isLanternInitialized = YES;
    
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
    _lantern->init();
    
    // retina display?
    // scale should be 2.0 for retina displays.
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        [(EAGLView*)self.view setScaleFactor:[UIScreen mainScreen].scale];
        _lantern->setScale([UIScreen mainScreen].scale);
    }
	
    [(EAGLView*)self.view setContext:_context];
    [(EAGLView*)self.view setFramebuffer];
    
    _animating = NO;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    // enable the accelerometer?
    NSString* accelParam = [lanternConfig objectForKey:kLanternConfigAccelerometerEnabled];
    if (accelParam && [accelParam intValue] == 1) {
        [[UIAccelerometer sharedAccelerometer] setDelegate:self];
    }
    
    // enable audio?
    NSString * audioParam = [lanternConfig objectForKey:kLanternConfigAudioEnabled];
    if (audioParam && [audioParam intValue] == 1) {
        theAudio = [[LAAudio alloc] initWithSampleRate:LANTERN_AUDIO_SAMPLE_RATE bufferSize:LANTERN_AUDIO_BUFFER_SIZE callback:audioCallback userData:_lantern];
        if (![theAudio startSession]) {
            NSLog(@"* Failed to enable audio");
        }
    }
}

- (void) loadView
{
    self.view = [[EAGLView alloc] init];
    self.view.frame = [[UIScreen mainScreen] applicationFrame];
    [self.view setMultipleTouchEnabled:YES];
    if (_lantern)
        ((EAGLView*)self.view).lantern = _lantern;
}

- (void) viewDidLoad
{
    if (!isLanternInitialized) {
        [self initializeLantern];
    }
}

- (void) dealloc
{
    if (theAudio) {
        theAudio = nil;
    }
    
    if (_context) {
        if ([EAGLContext currentContext] == _context)
            [EAGLContext setCurrentContext:nil];
        _context = nil;
    }
    
    if (lanternConfig) {
        lanternConfig = nil;
    }
    
    if (self.lantern) {
        _lantern->stop();
        delete _lantern;
        _lantern = NULL;
        isLanternInitialized = NO;
    }
    
    // [super dealloc];
}

- (void) viewWillAppear: (BOOL)animated
{
    if (!isLanternInitialized) {
        [self initializeLantern];
    }
    
    // gain focus event
    Event e(LANTERN_EVENT_GAIN_FOCUS, 0, NULL);
    _lantern->event(e);
    
    // resume graphics and audio
    [self resume];
    [super viewWillAppear:animated];
}

- (void) viewWillDisappear:(BOOL)animated
{
    // lose focus event
    Event e(LANTERN_EVENT_LOSE_FOCUS, 0, NULL);
    _lantern->event(e);
    
    // suspend graphics and audio
    [self suspend];
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
    
    _lantern->stop();
    
    [super viewDidUnload];
}

- (void) suspend
{
    if (theAudio)
        [theAudio suspendSession];
    [self stopAnimation];
}

- (void) resume
{
    if (theAudio)
        [theAudio resumeSession];
    [self startAnimation];
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
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void) startAnimation
{
    if (!_animating) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(draw)];
        [_displayLink setFrameInterval:animationFrameInterval];
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        _animating = YES;
    }
}

- (void) stopAnimation
{
    if (_animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        _animating = NO;
    }
}

- (void) draw {
    [(EAGLView*)self.view setFramebuffer];
    
    _lantern->draw();
	
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
        _lantern->event(touchEvent);
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
        _lantern->event(touchEvent);
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
        _lantern->event(touchEvent);
    }
}

- (void) touchesCancelled: (NSSet*)touches withEvent: (UIEvent*)event
{
    [self touchesEnded:touches withEvent:event];
}

- (void) accelerometer: (UIAccelerometer*)accelerometer didAccelerate: (UIAcceleration*)acceleration
{
    // accel event
    float accelParam[] = { (float)acceleration.x, (float)acceleration.y, (float)acceleration.z };
    Event accelEvent(LANTERN_EVENT_ACCEL, (unsigned int)accelerometer, accelParam);
    _lantern->event(accelEvent);
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

void audioCallback(Sample* buffer, unsigned int nFrames, void* userData) {
    memset(buffer, 0, LANTERN_AUDIO_NUM_CHANNELS * nFrames * sizeof(Sample));
    
    for (unsigned int i = 0; i < nFrames; i++) {
        ((Lantern*)userData)->getAudioFrame(buffer);
        buffer += LANTERN_AUDIO_NUM_CHANNELS;
    }
    return;
}
