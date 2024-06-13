#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

#include <SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>

class GameAudio
{
public:
    GameAudio();
    ~GameAudio();

    bool Initialize();
    void LoadWAV(const std::string& soundName, const std::string& filePath);
    void PlaySound(const std::string& soundName);
    void StopSound();
    void Cleanup();

private:
    struct AudioData
    {
        Uint8* buffer;
        Uint32 length;
        Uint32 position;
        SDL_AudioSpec spec;
        bool isPlaying;
    };

    std::unordered_map<std::string, AudioData> m_sounds;
    SDL_AudioDeviceID m_deviceId;
    std::mutex m_audioMutex;

    static void AudioCallback(void* userdata, Uint8* stream, int len);
};

#endif // GAMEAUDIO_H
