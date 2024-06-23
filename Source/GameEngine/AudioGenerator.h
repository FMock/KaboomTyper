/*
* AudioGenerator can generate audio data and draw it to screen
* 
* 	// Generate audio data and draw it
* 
*   auto m_audioGenerator = std::make_unique<AudioGenerator>();
	float frequency = 440.0f; // 440 Hz (A4)
	float duration = 1.0f;    // 1 second
	float sampleRate = 44100.0f; // CD quality sample rate
	std::vector<float> audioData = m_audioGenerator->GenerateSineWave(frequency, duration, sampleRate);
	m_audioGenerator->DrawAudio(0, 150, 150, 1, 1, audioData);
*/

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
