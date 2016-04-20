#pragma once
#include "Vector2.h"
#include <iostream>

/* Simplified and Streamlined Rigidbody */

class Rigidbody 
{
public:
    float mass;

    Vector2 velocity, acceleration, force;

    Rigidbody() :
        acceleration({ 0,0 }), velocity({ 0,0 }), mass(1), force({ 0,0 })
                        { }

    void addForce(const Vector2 &f) { force += f; }

    void Rigidbody::integrate(Vector2 &position, float dt)
    {
        acceleration = force / mass;
        velocity += acceleration * dt;
        position += velocity     * dt;

        force = { 0, 0 };
    }
};