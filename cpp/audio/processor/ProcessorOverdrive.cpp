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
    setMix(1.0f);
}

void ProcessorOverdrive::recompute() {
    
}

void ProcessorOverdrive::setDrive(float thresholdDb) {
    drive = powf(10.0f, thresholdDb / 20.0f);
}

void ProcessorOverdrive::setMix(float wet) {
    if (wet > 1.0f)
        wet = 1.0f;
    if (wet < 0.0f)
        wet = 0.0f;
    
    this->wet = wet;
}

Sample ProcessorOverdrive::process(Sample input) {
    if (wet > 0) {
        Sample output = input;
        
        if (input < 0)
            output = (input > -drive) ? input : -drive;
        else
            output = (input < drive) ? input : drive;
        
        return (output * wet) + (input * (1.0f - wet));
    } else
        return input * gain;
}
