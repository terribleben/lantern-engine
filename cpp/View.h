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

#include "Event.h"

#include <map>
#include <string>
using std::map;
using std::string;

#define LANTERN_VIEW_PARAM_WIDTH "width"
#define LANTERN_VIEW_PARAM_HEIGHT "height"

typedef struct ViewParams {
    map<string, void*> params;
    string nextView;
} ViewParams;

class View {
public:
    virtual ~View() {  }
    
    virtual void init(ViewParams*);
    
    virtual void step() = 0;
    virtual void draw() = 0;
    virtual void drawForeground() = 0;
    virtual void event(Event& e) = 0;
    
    virtual bool isFinished() = 0;
    virtual void stop(ViewParams*) = 0;
    
    virtual void setSize(float width, float height);
    
protected:
    float viewWidth, viewHeight;
};

#endif
