//
//  lantern.cpp
//  lantern
//

#include <iostream>

#include "Lantern.h"
#include "View.h"
#include "ParticleManager.h"
#include "FontManager.h"

static Lantern* theLantern = NULL;

/**
 * Override this method to prepare any views, resources, etc. once before the main loop begins.
 */
void Lantern::gameWillBegin() {

}

Lantern::Lantern() : screenWidth(0), screenHeight(0), screenScale(1), view(NULL), isPortrait(false), isRunning(false), isMicrophoneEnabled(false) {
    Lantern::setPrimaryInstance(this);
}

Lantern* Lantern::getPrimaryInstance() {
    return theLantern;
}

void Lantern::setPrimaryInstance(Lantern *lantern) {
    theLantern = lantern;
}

void Lantern::init() {
    // reset graphics properties
    screenWidth = screenHeight = 0;
    screenScale = 1.0f;
    isPortrait = false;
    initialViewKey = "";
    isRunning = false;
    isMicrophoneEnabled = false;
    audioFrameId = 0;
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
}

void Lantern::start() {
    if (!isRunning) {
        isRunning = true;
        
        gameWillBegin();
        transitionView();
    }
}

void Lantern::setDimensions(GLfloat width, GLfloat height) {
    screenWidth = width;
    screenHeight = height;
    
    if (view)
        view->setSize(screenWidth / screenScale, screenHeight / screenScale);
}

void Lantern::setScale(GLfloat scale) {
    screenScale = scale;
    if (screenScale == 0)
        screenScale = 1.0f;
}

void Lantern::setOrientation(bool isPortrait) {
    this->isPortrait = isPortrait;
}

void Lantern::stop() {
    if (isRunning) {
        map<string, View*>::iterator viewItr = views.begin();
        while (viewItr != views.end()) {
            delete (*viewItr).second;
            views.erase(viewItr++);
        }
        view = NULL;
        
        for (list<Track*>::iterator tItr = audioTracks.begin(); tItr != audioTracks.end(); tItr++) {
            delete *tItr;
        }
        audioTracks.clear();
        
        for (map<unsigned int, AudioSharedBuffer*>::iterator bufItr = inputBuffers.begin(); bufItr != inputBuffers.end(); bufItr++) {
            AudioSharedBuffer* buf = bufItr->second;
            if (buf->buffer)
                free(buf->buffer);
            free(buf);
        }
        inputBuffers.clear();
        
        isRunning = false;
    }
}

void Lantern::addView(View* view, string key) {
    if (views.find(key) != views.end()) {
        delete views[key];
    }
    views[key] = view;
}

void Lantern::setInitialView(string initialViewKey) {
    this->initialViewKey = initialViewKey;
}

void Lantern::transitionView() {
    ViewParams p;
    bool shouldTransition = false;
    
    if (view == NULL) {
        if (initialViewKey.length())
            p.nextView = initialViewKey;
        else if (views.size())
            p.nextView = views.begin()->first;
        shouldTransition = true;
    }
    else if (view->isFinished()) {
        shouldTransition = true;
        view->stop(&p);
    }
    
    if (shouldTransition) {
        if (p.nextView.length() && views.find(p.nextView) != views.end()) {
            float viewWidth = screenWidth / screenScale;
            float viewHeight = screenHeight / screenScale;
            
            if (!isPortrait) {
                float temp = viewWidth;
                viewWidth = viewHeight;
                viewHeight = temp;
            }
            
            p.params[LANTERN_VIEW_PARAM_WIDTH] = &viewWidth;
            p.params[LANTERN_VIEW_PARAM_HEIGHT] = &viewHeight;
            
            view = views[p.nextView];
            view->init(&p);
        } else {
            // some kind of problem assigning next view; crash
            fprintf(stdout, "Lantern: Invalid next view when transitioning between views.");
            int* crash = 0;
            *crash = 0;
        }
    }
}

void Lantern::event(Event& e) {
    if (e.type == LANTERN_EVENT_AUDIO_TRACK) {
        Track* t = (Track*) e.data;
        audioTracks.push_back(t);
    } else {
        if (view) {
            if (isPortrait) {
                switch (e.type) {
                    case LANTERN_EVENT_TOUCH_DOWN: case LANTERN_EVENT_TOUCH_UP: case LANTERN_EVENT_TOUCH_MOVE: {
                        float* touchPoint = (float*)e.data;
                        
                        // invert x, then swap y and x
                        touchPoint[0] = (screenWidth / screenScale) - touchPoint[0]; // for relative, just touchPoint[0] = -touchPoint[0]
                        
                        float swap = touchPoint[1];
                        touchPoint[1] = touchPoint[0];
                        touchPoint[0] = swap;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
            view->event(e);
        }
    }
}

void Lantern::setIsMicrophoneEnabled(bool isEnabled) {
    this->isMicrophoneEnabled = isEnabled;
}

bool Lantern::getIsMicrophoneEnabled() {
    return isMicrophoneEnabled;
}

void Lantern::setAudioInputBuffer(unsigned int key, AudioSharedBuffer *buffer) {
    if (buffer == NULL) {
        if (inputBuffers.find(key) != inputBuffers.end()) {
            AudioSharedBuffer* existingBuffer = inputBuffers[key];
            free(existingBuffer->buffer);
            free(existingBuffer);
            inputBuffers.erase(key);
        }
    } else {
        if (inputBuffers.find(key) != inputBuffers.end()) {
            AudioSharedBuffer* existingBuffer = inputBuffers[key];
            if (existingBuffer->length != buffer->length) {
                free(existingBuffer->buffer);
                if (buffer->length)
                    existingBuffer->buffer = (Sample*) malloc(buffer->length);
                else
                    existingBuffer->buffer = NULL;
                existingBuffer->length = buffer->length;
            }
            if (existingBuffer->length)
                memcpy(existingBuffer->buffer, buffer->buffer, existingBuffer->length);
        } else {
            AudioSharedBuffer* newBuffer = (AudioSharedBuffer *) malloc(sizeof(AudioSharedBuffer*));
            newBuffer->length = buffer->length;
            if (newBuffer->length) {
                newBuffer->buffer = (Sample *) malloc(newBuffer->length);
                memcpy(newBuffer->buffer, buffer->buffer, newBuffer->length);
            } else
                newBuffer->buffer = NULL;
            inputBuffers[key] = newBuffer;
        }
    }
}

AudioSharedBuffer* Lantern::getAudioInputBuffer(unsigned int key) {
    if (inputBuffers.find(key) != inputBuffers.end())
        return inputBuffers[key];
    else
        return NULL;
}

void Lantern::getAudioFrame(Sample* samples) {
    Sample singleTrack[LANTERN_AUDIO_NUM_CHANNELS];
    memset(singleTrack, 0, LANTERN_AUDIO_NUM_CHANNELS * sizeof(Sample));
    
    list<Track*>::iterator it = audioTracks.begin(), end_it = audioTracks.end();
    int cc = 0;
    
    while (it != end_it) {
        (*it)->getFrame(singleTrack, audioFrameId);
        
        for (cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            samples[cc] += singleTrack[cc];
        }
        
        if ((*it)->isFinished()) {
            delete *it;
            audioTracks.erase(it++);
        } else
            it++;
    }
    audioFrameId++; // allow overflow
}

void Lantern::draw() {
    glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                  
    glOrthof(0, screenWidth / screenScale, 0, screenHeight / screenScale, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (isPortrait) {
        // translate
        glTranslatef((screenWidth / screenScale) / 2, (screenHeight / screenScale) / 2, 0);
        // rotate around z
        glRotatef(90, 0, 0, 1);
        // translate
        glTranslatef(-(screenHeight / screenScale) / 2, -(screenWidth / screenScale) / 2, 0);
    }
    
    if (view != NULL)
        view->step();
    ParticleManager::getInstance().step();
    
    transitionView();
    
    if (view != NULL) {
        view->draw();
        ParticleManager::getInstance().draw();
        view->drawForeground();
    }
}