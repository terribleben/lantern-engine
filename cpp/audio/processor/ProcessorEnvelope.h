//
//  ProcessorEnvelope.h
//  Linear ADSR envelope.
//  lantern
//

#ifndef __LANTERN_PROCESSOR_ENVELOPE_H__
#define __LANTERN_PROCESSOR_ENVELOPE_H__

#include "Processor.h"

typedef enum ProcessorEnvelopeStage {
    kProcessorEnvelopeStageAttack,
    kProcessorEnvelopeStageDecay,
    kProcessorEnvelopeStageSustain,
    kProcessorEnvelopeStageRelease
} ProcessorEnvelopeStage;

class ProcessorEnvelope : public Processor {
public:
    ProcessorEnvelope();
    ProcessorEnvelope(float attack, float decay, float sustain, float release);
    ~ProcessorEnvelope();
    
    // envelope properties
    float getAttack();
    void setAttack(float seconds);

    float getDecay();
    void setDecay(float seconds);

    float getSustain();
    void setSustain(float level);

    float getRelease();
    void setRelease(float seconds);
    
    // envelope actions
    void on();
    void off();
    Sample process(Sample input);

protected:
    float attack, decay, sustain, release;
    float attackInc, decayInc, releaseInc;
    
    bool isOn;
    ProcessorEnvelopeStage stage;
    float level;
    unsigned int framePosition;
    
    void recompute();
    void reset();
};

#endif
