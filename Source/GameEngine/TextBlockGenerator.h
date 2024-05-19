#pragma once

#include "TextBlock.h"
#include <string>
#include <deque>
#include <chrono>

namespace GameEngine
{
    class TextBlockGenerator
    {
    private:
        std::deque<std::unique_ptr<TextBlock>> m_blockDeque;
        std::chrono::steady_clock::time_point m_lastSpawnTime;
        bool m_running;
        float m_spawnInterval; // Time interval between spawning TextBlocks
        float m_elapsedTime = 0;

    public:
        TextBlockGenerator(float spawnIntervalSeconds);
        void ClearBlockDeque();
        void GenerateTextBlock(std::string text);
        void Update(float dt);
        void Draw();
        void PopTextBlock();
        void ToggleRunning();
        bool IsRunning();
        void HandleCollisions(TextBlock& blockA, float& blockAYPosition, TextBlock& blockB);
    };
}