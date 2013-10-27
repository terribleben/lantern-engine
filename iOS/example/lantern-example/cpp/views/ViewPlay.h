//
//  ViewPlay.h
//  lantern
//

#ifndef __LANTERN_VIEW_PLAY_H__
#define __LANTERN_VIEW_PLAY_H__

#include "View.h"

class ViewPlay : public View {
public:
    void init(ViewParams*);
    void step();
    void draw();
    void drawForeground();
    
    bool isFinished();
    void stop(ViewParams*);
    void event(Event&);
};

#endif
