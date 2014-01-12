//
//  ProcessorReverbSimple.cpp
//  lantern
//

#include "ProcessorReverbSimple.h"

ProcessorReverbSimple::ProcessorReverbSimple() : Processor() {
    // allpass => 3 parallel mutually prime combs => add gate
    biquad = new ProcessorBiquad();
    biquad->setAllpass(1, M_PI * 0.3f);
    
    combs[0] = new ProcessorDelay(113, 0.7f);
    combs[1] = new ProcessorDelay(337, 0.7f);
    combs[2] = new ProcessorDelay(1051, 0.7f);
    combs[0]->setInput(biquad);
    combs[1]->setInput(biquad);
    combs[2]->setInput(biquad);
    
    output = new ProcessorGroup();
    output->addInput(combs[0]);
    output->addInput(combs[1]);
    output->addInput(combs[2]);
}

ProcessorReverbSimple::~ProcessorReverbSimple() {
    if (output) {
        output->stop();
        delete output;
        output = NULL;
    }
    
    if (combs[0]) {
        combs[0]->stop();
        combs[1]->stop();
        delete combs[0];
        delete combs[1];
    }
    
    if (biquad) {
        biquad->stop();
        delete biquad;
        biquad = NULL;
    }
}

void ProcessorReverbSimple::setInput(Track* input) {
    biquad->setInput(input);
}

void ProcessorReverbSimple::recompute() {
    
}

void ProcessorReverbSimple::getFrame(Sample* samples, long long frameId) {
    output->getFrameCached(samples, frameId);
}

Sample ProcessorReverbSimple::process(Sample input) {
    // override getFrame
    return 0;
}
