//
//  ViewController.h
//  lantern
//

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT NSString* const kLanternConfigAccelerometerEnabled;

@interface ViewController : UIViewController <UIAccelerometerDelegate>

- (void) startAnimating;
- (void) stopAnimating;

@end
