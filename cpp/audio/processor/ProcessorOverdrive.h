//
//  ProcessorOverdrive.h
//  lantern
//
//  Simplest possible overdrive distortion.
//  The drive parameter controls how aggressively we clip the input signal.
//  The (inherited) gain parameter can be used to compensate the level of the output signal.
//
//  Created by Ben Roth on 1/5/14.
//  Copyright (c) 2014 Ben Roth. All rights reserved.
//

#ifndef __LANTERN_PROCESSOR_OVERDRIVE_H__
#define __LANTERN_PROCESSOR_OVERDRIVE_H__

#include "Processor.h"

class ProcessorOverdrive : public Processor {
public:
    ProcessorOverdrive();
    
    // threshold at which to clip the input signal.
    // measured in decibels (zero is no clip, -3 is 3 decibels below max, etc.)
    void setDrive(float thresholdDb);
    Sample process(Sample);
    
protected:
    void recompute();
    
    float drive;
};

#endif
