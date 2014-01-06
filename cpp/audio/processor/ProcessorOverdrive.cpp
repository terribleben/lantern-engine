//
//  ProcessorOverdrive.cpp
//  lantern
//
//  Created by Ben Roth on 1/5/14.
//  Copyright (c) 2014 Ben Roth. All rights reserved.
//

#include "ProcessorOverdrive.h"

ProcessorOverdrive::ProcessorOverdrive() : Processor() {
    setDrive(-3.0f);
}

void ProcessorOverdrive::recompute() {
    
}

void ProcessorOverdrive::setDrive(float thresholdDb) {
    drive = powf(10.0f, thresholdDb / 20.0f);
}

Sample ProcessorOverdrive::process(Sample input) {
    if (input < 0)
        input = fmaxf(input, drive);
    else
        input = fminf(input, drive);
    
    return input * gain;
}
