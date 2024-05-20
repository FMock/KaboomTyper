#pragma once

#include "TextBlock.h"
#include "InputManager.h"
#include <string>
#include <deque>
#include <chrono>
#include <memory>

namespace GameEngine
{
    class TextBlockGenerator
    {
    private:
        std::deque<std::unique_ptr<TextBlock>> m_blockDeque;
        std::chrono::steady_clock::time_point m_lastSpawnTime;
        bool m_running;
        bool m_limitReached;
        float m_spawnInterval; // Time interval between spawning TextBlocks
        float m_elapsedTime = 0;
        std::shared_ptr<InputManager> m_inputManager;
        void SetHorizontalMovement(TextBlock* block);
        TextBlock* m_horizontalMovingBlock; // Pointer to the block that can move horizontally

    public:
        TextBlockGenerator(float spawnIntervalSeconds, std::shared_ptr<InputManager> inputManager);
        void ClearBlockDeque();
        void GenerateTextBlock(std::string text);
        void Update(float dt);
        void Draw();
        void PopTextBlock();
        void ToggleRunning();
        bool IsRunning();
        bool TextBlockLimitReached();
        void SetTextBlockLimitReached(bool b);
        void HandleCollisions(TextBlock& blockA, float& blockAYPosition, TextBlock& blockB);
    };
}