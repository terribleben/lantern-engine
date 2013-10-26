//
//  LAViewController.h
//  lantern
//

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT NSString* const kLanternConfigAccelerometerEnabled;

@interface LAViewController : UIViewController <UIAccelerometerDelegate>

- (void) startAnimating;
- (void) stopAnimating;

@end
