//
//  LAViewController.h
//  lantern
//

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT NSString* const kLanternConfigAccelerometerEnabled;

@interface LAViewController : UIViewController <UIAccelerometerDelegate>

- (void) startAnimating;
- (void) stopAnimating;

/**
 *  The Lantern instance that this view controller will manage.
 */
@property (nonatomic, assign) void* lantern;

@end
