//
//  Event.h
//  lantern
//

#ifndef __LANTERN_EVENT_H__
#define __LANTERN_EVENT_H__

#define LANTERN_EVENT_TOUCH_DOWN 0
#define LANTERN_EVENT_TOUCH_UP 1

class Event {
public:
    Event(unsigned int type, void* data) { this->data = data; this->type = type; }
    ~Event() {  }
    
public:
    unsigned int type;
    void* data;
};

#endif
