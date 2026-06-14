#include "PhysicsWorld.h"

using namespace GameEngine;

// Gravity points toward +y so "down" matches the game's y-down screen space.
PhysicsWorld::PhysicsWorld()
    : m_world(b2Vec2(0.0f, 30.0f))
{
}

void PhysicsWorld::Step(float dt)
{
    // Step the world with a fixed timestep for stability.
    constexpr float fixedStep = 1.0f / 60.0f;
    constexpr int velocityIterations = 8;
    constexpr int positionIterations = 3;

    m_accumulator += dt;
    int steps = 0;
    while (m_accumulator >= fixedStep && steps < 5) // cap to avoid spiral-of-death
    {
        m_world.Step(fixedStep, velocityIterations, positionIterations);
        m_accumulator -= fixedStep;
        ++steps;
    }
}

void PhysicsWorld::SetGravity(float gravityPxPerSec2)
{
    // +y is down in screen space, so a downward pull is a positive y component.
    m_world.SetGravity(b2Vec2(0.0f, ToMeters(gravityPxPerSec2)));
}

b2Body* PhysicsWorld::CreateBlockBody(float topLeftX, float topLeftY, float widthPx, float heightPx,
                                      float initialDownVelPxPerSec)
{
    const float halfW = widthPx * 0.5f;
    const float halfH = heightPx * 0.5f;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(ToMeters(topLeftX + halfW), ToMeters(topLeftY + halfH));
    // A little damping keeps a freshly-stacked pile from jittering forever.
    bodyDef.linearDamping = 0.05f;
    bodyDef.angularDamping = 0.05f;
    b2Body* body = m_world.CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(ToMeters(halfW), ToMeters(halfH));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.6f;     // enough grip to form leaning stacks
    fixtureDef.restitution = 0.0f;  // blocks should not bounce
    body->CreateFixture(&fixtureDef);

    body->SetLinearVelocity(b2Vec2(0.0f, ToMeters(initialDownVelPxPerSec)));
    return body;
}

void PhysicsWorld::DestroyBody(b2Body* body)
{
    if (body)
        m_world.DestroyBody(body);
}

void PhysicsWorld::CreateBounds(float leftX, float rightX, float floorY, float topY)
{
    b2BodyDef groundDef; // static by default
    b2Body* ground = m_world.CreateBody(&groundDef);

    b2EdgeShape edge;

    // Floor
    edge.SetTwoSided(b2Vec2(ToMeters(leftX), ToMeters(floorY)),
                     b2Vec2(ToMeters(rightX), ToMeters(floorY)));
    ground->CreateFixture(&edge, 0.0f);

    // Left wall
    edge.SetTwoSided(b2Vec2(ToMeters(leftX), ToMeters(topY)),
                     b2Vec2(ToMeters(leftX), ToMeters(floorY)));
    ground->CreateFixture(&edge, 0.0f);

    // Right wall
    edge.SetTwoSided(b2Vec2(ToMeters(rightX), ToMeters(topY)),
                     b2Vec2(ToMeters(rightX), ToMeters(floorY)));
    ground->CreateFixture(&edge, 0.0f);
}
