#pragma once

#include <box2d/box2d.h>

namespace GameEngine
{
    // Box2D works in meters; the game works in pixels. This is the conversion factor.
    constexpr float PIXELS_PER_METER = 30.0f;

    // Thin wrapper around a b2World: owns the physics simulation, the static play-area
    // bounds (floor + side walls), and pixel<->meter conversion helpers.
    class PhysicsWorld
    {
    public:
        PhysicsWorld();

        void Step(float dt);                 // fixed-timestep accumulator
        b2World* GetWorld() { return &m_world; }

        // Create the static floor and left/right walls (positions in pixels).
        void CreateBounds(float leftX, float rightX, float floorY, float topY);

        // Point gravity downward at the given screen acceleration (pixels/sec^2).
        void SetGravity(float gravityPxPerSec2);

        // Spawn a dynamic box body for a TextBlock. Inputs are in pixels/top-left
        // (matching the rest of the game); the body is positioned at the box center
        // and given an initial downward velocity so its fall continues smoothly.
        b2Body* CreateBlockBody(float topLeftX, float topLeftY, float widthPx, float heightPx,
                                float initialDownVelPxPerSec);

        // Remove a body from the simulation (safe to call with nullptr).
        void DestroyBody(b2Body* body);

        static float ToMeters(float px) { return px / PIXELS_PER_METER; }
        static float ToPixels(float m) { return m * PIXELS_PER_METER; }

    private:
        b2World m_world;
        float m_accumulator = 0.0f;
    };
}
