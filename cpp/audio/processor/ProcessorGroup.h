//
//  ProcessorGroup.h
//  A processor that takes multiple inputs and sends them to one output.
//  lantern
//

#ifndef __LANTERN_PROCESSOR_GROUP_H__
#define __LANTERN_PROCESSOR_GROUP_H__

#include "Processor.h"

#include <list>
using std::list;

class ProcessorGroup : public Processor {
public:
    ProcessorGroup();
    virtual ~ProcessorGroup();
    virtual Sample process(Sample input);
    virtual void getFrame(Sample* samples);

    // this class maintains a list of inputs and outputs the sum of their outputs.
    virtual void setInput(Track*);
    virtual void addInput(Track*);
    virtual void clearInputs();
protected:
    list<Track*> inputs;
    virtual void recompute();
};

#endif
