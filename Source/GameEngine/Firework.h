#pragma once

#include<GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "DrawUtils.h"
#include "IDrawable.h"


namespace GameEngine
{
    // A single explosion: plays a shared sprite-sheet frame animation (the hit_* frames)
    // layered with bright spark particles that fly outward. Always bright so it stays
    // visible against the dark-blue play area regardless of the destroyed block's color.
    class Firework : public IDrawable
    {
    public:
        Firework(int numParticles = 120);

        // Configure one explosion instance: the shared frame set (owned by the manager),
        // center position, sprite size, and a small start delay (for staggered multi-blasts).
        void Init(const std::vector<GLuint>* frames, int centerX, int centerY, int spriteSize, float startDelay);

        void Update(float deltaTime) override;
        void Draw() override;

        bool IsActive() const { return m_isActive; }
        void SetIsActive(bool isActive);
        void GenerateParticles();
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }

    private:
        struct Particle
        {
            float x, y;   // position
            float vx, vy; // velocity (px/sec)
            float scale;  // draw scale
        };

        void GenerateSet(std::vector<Particle>& set, int count, float maxSpeed);

        // Sprite frame animation (frames are shared and owned by the manager)
        const std::vector<GLuint>* m_frames = nullptr;
        int m_frameIndex = 0;
        float m_animFps = 22.0f; // 12 frames ~= 0.55s
        int m_spriteSize = 110;
        float m_startDelay = 0.0f; // seconds before this blast begins (staggered cascade)
        float m_elapsed = 0.0f;    // time since spawn

        // Bright spark textures + sets
        GLuint m_texWhite = 0;
        GLuint m_texYellow = 0;
        GLuint m_texOrange = 0;
        std::vector<Particle> m_sparksWhite;
        std::vector<Particle> m_sparksYellow;
        std::vector<Particle> m_sparksOrange;

        int m_posX = 0, m_posY = 0;
        int m_particleCount;

        bool m_isActive = false;
        int m_priority = 0; // draw priority
    };
}
