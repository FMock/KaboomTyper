#pragma once
#include <vector>
#include <cmath>
#include <optional> // For std::optional, C++17 or later
#include<GL/glew.h>

namespace GameEngine
{
    class AudioGenerator
    {
    private:
        std::optional<std::vector<float>> m_cachedSineWave;

    public:
        void DrawAudio(GLuint texture, int m_x, int m_y, int textureWidth, int textureHeight, std::vector<float>& audioData);
        std::vector<float> GenerateSineWave(float frequency, float duration, float sampleRate);
        void UpdateCachedSineWave(std::vector<float>&& newData);
    };
}
