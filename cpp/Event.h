//
//  Event.h
//  lantern
//

#ifndef __LANTERN_EVENT_H__
#define __LANTERN_EVENT_H__

// enum of event types
#define LANTERN_EVENT_TOUCH_DOWN 0
#define LANTERN_EVENT_TOUCH_UP 1
#define LANTERN_EVENT_TOUCH_MOVE 2
#define LANTERN_EVENT_ACCEL 3
#define LANTERN_EVENT_LOSE_FOCUS 4
#define LANTERN_EVENT_GAIN_FOCUS 5
#define LANTERN_EVENT_AUDIO_TRACK 6

class Event {
public:
    Event(unsigned int type, unsigned int sourceId, void* data) { this->data = data; this->sourceId = sourceId; this->type = type; }
    ~Event() {  }
    
public:
    unsigned int type;       // from the enum above
    unsigned int sourceId;   // varies. for example, the id of the touch, or the id of the joystick
    void* data;              // varies. for example, the (x, y) pair indicating the touch position onscreen.
};

#endif
