//
//  View.h
//  lantern
//  
//  init() is called once each time the view is loaded.
//  isFinished() is called once each step.
//  stop() is called as soon as isFinished() returns true.
//  A ViewParams instance is passed from each view's stop() method into its successor's init() method.
//

#ifndef __LANTERN_VIEW_H__
#define __LANTERN_VIEW_H__

#define LANTERN_VIEW_PLAY 0

#include "Event.h"

#include <map>
using std::map;
using std::string;

typedef struct ViewParams {
    unsigned int nextView;
    map<string, void*> params;
} ViewParams;

class View {
public:
    virtual ~View() {  }
    
    virtual void init(ViewParams*) = 0;
    
    virtual void step() = 0;
    virtual void draw() = 0;
    virtual void drawForeground() = 0;
    virtual void event(Event& e) = 0;
    
    virtual bool isFinished() = 0;
    virtual void stop(ViewParams*) = 0;
};

#include "ViewPlay.h"

#endif
