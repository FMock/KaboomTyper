#include "GameAudio.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>
#include <cstring>

GameAudio::GameAudio() : m_deviceId(0)
{
    // Constructor
}

GameAudio::~GameAudio()
{
    Cleanup();
}

bool GameAudio::Initialize()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16; // 16-bit signed PCM
    desiredSpec.channels = 2;
    desiredSpec.samples = 4096;
    desiredSpec.callback = AudioCallback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;
    m_deviceId = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, &obtainedSpec, 0);
    if (m_deviceId == 0)
    {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        return false;
    }

#if DEBUG_AUDIO
    std::cout << "Audio device initialized successfully." << std::endl;
#endif

    SDL_PauseAudioDevice(m_deviceId, 0); // Start audio playback

    return true;
}

void GameAudio::LoadWAV(const std::string& soundName, const std::string& filePath)
{
    SDL_AudioSpec wavSpec;
    Uint8* wavBuffer;
    Uint32 wavLength;

    if (SDL_LoadWAV(filePath.c_str(), &wavSpec, &wavBuffer, &wavLength) == nullptr)
    {
        std::cerr << "Failed to load WAV file: " << SDL_GetError() << std::endl;
        return;
    }

    AudioData audioData = { wavBuffer, wavLength, 0, wavSpec, false };
    std::lock_guard<std::mutex> lock(m_audioMutex);
    m_sounds[soundName] = audioData;

#if DEBUG_AUDIO 
    std::cout << "Loaded WAV file: " << soundName << std::endl;
#endif
}

void GameAudio::PlaySound(const std::string& soundName)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_sounds.find(soundName) != m_sounds.end())
    {
        m_sounds[soundName].position = 0; // Reset position to start of buffer
        m_sounds[soundName].isPlaying = true; // Set playing flag

#if DEBUG_AUDIO
        std::cout << "Playing sound: " << soundName << std::endl;
#endif

        SDL_PauseAudioDevice(m_deviceId, 0); // Ensure audio device is playing
    }
    else
    {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}

void GameAudio::StopSound()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto& pair : m_sounds)
    {
        pair.second.isPlaying = false;
    }
    SDL_PauseAudioDevice(m_deviceId, 1); // Pause the audio device
}

void GameAudio::Cleanup()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto& sound : m_sounds)
    {
        SDL_FreeWAV(sound.second.buffer);
    }
    m_sounds.clear();

    if (m_deviceId != 0)
    {
        SDL_CloseAudioDevice(m_deviceId);
    }

    SDL_Quit();
}

void GameAudio::AudioCallback(void* userdata, Uint8* stream, int len)
{
    GameAudio* audio = static_cast<GameAudio*>(userdata);
    std::lock_guard<std::mutex> lock(audio->m_audioMutex);

    SDL_memset(stream, 0, len);

    bool dataCopied = false;
    for (auto& pair : audio->m_sounds)
    {
        AudioData& data = pair.second;
        if (!data.isPlaying || data.position >= data.length) continue;

        Uint32 amount = (data.length - data.position > (Uint32)len) ? len : data.length - data.position;
        SDL_MixAudioFormat(stream, data.buffer + data.position, data.spec.format, amount, SDL_MIX_MAXVOLUME);
        data.position += amount;
        dataCopied = true;

        // Loop back to the start if at the end of the buffer
        if (data.position >= data.length)
        {
            data.position = 0;
            data.isPlaying = false; // Stop playing after one loop
        }
    }

    if (!dataCopied)
    {

#if DEBUG_AUDIO
        std::cerr << "No audio data copied in callback." << std::endl;
#endif
    }
    else
    {

#if DEBUG_AUDIO
        std::cout << "Audio data copied in callback." << std::endl;
#endif

    }
}
