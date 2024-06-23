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
    class Firework : public IDrawable
    {
    public:
        Firework(GLuint texture, int x, int y, int w, int h, int numParticles);

        void Update(float deltaTime) override;
        void Draw() override;

        bool IsActive() const { return m_isActive; }
        void SetIsActive(bool isActive);
        void SetColor(Colors color);
        void SetPosition(int x, int y);
        void GenerateParticles();
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }

    private:
        struct Particle
        {
            int x;
            int y;
            float scaleX;
            float scaleY;
        };

        // Three color textures. Firework explosion is three colors
        GLuint m_tex;
        GLuint m_texSecond;
        GLuint m_texThird;
        std::vector<Particle> m_particles;
        std::vector<Particle> m_particlesSecond;
        std::vector<Particle> m_particlesThird;

        int m_posX, m_posY;
        int m_width, m_height;
        int m_particleCount;

        float m_timer;
        bool m_isActive;
        int m_priority = 0; // draw priority
    };
}