//
//  ParticleManager.h
//  lantern
//
//  Object that takes care of animating all particles in the system and destroying them when they've expired.
//  Use ParticleManager::getInstance().add(myParticle) (assumes myParticle is on the heap)

#ifndef __LANTERN_PARTICLE_MANAGER_H__
#define __LANTERN_PARTICLE_MANAGER_H__

#include "Particle.h"
#include <list>

using std::list;

class ParticleManager {
public:
    static ParticleManager& getInstance();
    
    void step();
    void draw();
    
    void add(Particle*);
    void clear();
    
private:
    ParticleManager();
    ParticleManager(const ParticleManager&);
    ~ParticleManager();
    
    list<Particle*> particles;
};

#endif
