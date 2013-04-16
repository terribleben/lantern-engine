//
//  Particle.cpp
//  lantern
//

#include "Particle.h"
#include "draw.h"
#include "ResourceManager.h"
#include "Angle.h"

Particle::Particle() : PhysicalObject() {
    setLifespan(0);
    textureKey = "";
    width = height = 0;
    radius = 0;
    scale = 1.0f;
    alpha = 1.0f;
    internalScale = 1.0f;
    internalAlpha = 1.0f;
    color.set(1, 1, 1);
}

Particle::~Particle() {
    
}

void Particle::step() {
    PhysicalObject::step();
    
    if (ttl > 0) {
        ttl--;
    }
}

void Particle::draw() {
    PhysicalObject::draw();
    glColor4f(color.x, color.y, color.z, alpha * internalAlpha);
    
    if (textureKey.length() > 0) {
        ResourceManager::getInstance().bindTexture(textureKey.c_str());
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glRotatef(Angle::toDegrees(orientation), 0, 0, 1);
        drawTexture(Point3f(), width * scale * internalScale, height * scale * internalScale);
        glPopMatrix();
    } else {
        drawRectangle(position - Point3f(radius, radius, 0), position + Point3f(radius, radius, 0), true);
    }
}

void Particle::setLifespan(unsigned int steps) {
    lifespan = ttl = steps;
}

bool Particle::isAlive() {
    return (ttl > 0);
}