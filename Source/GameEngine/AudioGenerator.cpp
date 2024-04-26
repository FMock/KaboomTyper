#include "AudioGenerator.h"
#include "DrawUtils.h"

using namespace GameEngine;
using namespace DrawUtilities;

void AudioGenerator::DrawAudio(GLuint texture, int m_x, int m_y, int textureWidth, int textureHeight, std::vector<float>& audioData)
{
	glDrawAudio(texture, m_x, m_y, textureWidth, textureHeight, audioData);
}

// Function to update the cached sine wave data using move semantics
void AudioGenerator::UpdateCachedSineWave(std::vector<float>&& newData)
{
    m_cachedSineWave = std::move(newData);
}

std::vector<float> AudioGenerator::GenerateSineWave(float frequency, float duration, float sampleRate)
{
    if (m_cachedSineWave.has_value())
    {
        return *m_cachedSineWave; // Return the cached data if available
    }

    size_t numSamples = static_cast<size_t>(duration * sampleRate);
    std::vector<float> audioData(numSamples);

    for (size_t i = 0; i < numSamples; ++i)
    {
        float time = static_cast<float>(i) / sampleRate;
        audioData[i] = sin(2.0f * std::acos(-1.0f) * frequency * time);
    }

    m_cachedSineWave = audioData; // Cache the generated audio data
    return audioData;
}
