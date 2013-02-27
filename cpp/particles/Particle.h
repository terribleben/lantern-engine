//
//  Particle.h
//  lantern
//

#ifndef __LANTERN_PARTICLE_H__
#define __LANTERN_PARTICLE_H__

#import <string>
#import "PhysicalObject.h"

using std::string;

class Particle : public PhysicalObject {
public:
    Particle();
    virtual ~Particle();
    
    virtual void step();
    virtual void draw();
    
    virtual void setLifespan(unsigned int steps);
    virtual bool isAlive();
    
    float scale;
    float alpha;
    Point3f color;
    string textureKey;
    float width, height;
    
protected:
    unsigned int lifespan;
    unsigned int ttl;
    
    float internalScale;
    float internalAlpha;
};

#endif
