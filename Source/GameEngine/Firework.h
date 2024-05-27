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

        void Update(float deltaTime);
        void Update() override;
        void Draw() override;

        bool IsActive() const { return m_isActive; }
        void SetIsActive(bool isActive);
        void SetColor(Colors color);
        void SetPosition(int x, int y);
        void GenerateParticles();

    private:
        struct Particle
        {
            int x;
            int y;
            float scaleX;
            float scaleY;
        };

        GLuint m_tex;
        int m_posX, m_posY;
        int m_width, m_height;
        int m_particleCount;
        std::vector<Particle> m_particles;
        float m_timer;
        bool m_isActive;
    };
}