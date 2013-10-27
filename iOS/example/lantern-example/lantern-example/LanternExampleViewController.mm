//
//  LanternExampleViewController.m
//  lantern-example
//

#import "LanternExampleViewController.h"
#include "LanternExample.h"

@interface LanternExampleViewController ()

@end

@implementation LanternExampleViewController

- (id)init
{
    if (self = [super init]) {
        self.lantern = new LanternExample();
    }
    return self;
}
@end
