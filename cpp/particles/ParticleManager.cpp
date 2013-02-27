//
//  ParticleManager.cpp
//  lantern
//

#include "ParticleManager.h"

ParticleManager& ParticleManager::getInstance() {
    static ParticleManager theManager;
    return theManager;
}

ParticleManager::ParticleManager() {
    
}

ParticleManager::~ParticleManager() {
    this->clear();
}

void ParticleManager::add(Particle* p) {
    particles.push_back(p);
}

void ParticleManager::draw() {
    for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        (*it)->draw();
    }
}

void ParticleManager::step() {
    list<Particle*>::iterator it = particles.begin();
    while (it != particles.end()) {
        Particle* p = *it;
        p->step();
        
        if (!p->isAlive()) {
            delete *it;
            particles.erase(it++);
        } else
            it++;
    }
}

void ParticleManager::clear() {
    for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        delete *it;
    }
    particles.clear();
}