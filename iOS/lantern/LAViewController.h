//
//  LAViewController.h
//  lantern
//

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT NSString* const kLanternConfigAccelerometerEnabled;

@interface LAViewController : UIViewController <UIAccelerometerDelegate>

- (void) resume;
- (void) suspend;

/**
 *  Take a snapshot of the lantern view.
 */
- (UIImage *) getSnapshot;

/**
 *  The Lantern instance that this view controller will manage.
 */
@property (nonatomic, assign) void* lantern;

@end
