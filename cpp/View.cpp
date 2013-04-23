//
//  View.cpp
//  lantern
//

#include "View.h"

void View::init(ViewParams* p) {
    viewWidth = viewHeight = 0;
    
    if (p->params.count(LANTERN_VIEW_PARAM_WIDTH))
        viewWidth = *(float*)(p->params[LANTERN_VIEW_PARAM_WIDTH]);
    if (p->params.count(LANTERN_VIEW_PARAM_HEIGHT))
        viewHeight = *(float*)(p->params[LANTERN_VIEW_PARAM_HEIGHT]);
}

void View::setSize(float width, float height) {
    viewWidth = width;
    viewHeight = height;
}
