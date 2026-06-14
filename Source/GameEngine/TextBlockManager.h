#pragma once

#include "TextBlock.h"
#include "InputManager.h"
#include "WordManager.h"
#include "IDrawable.h"
#include <string>
#include <deque>
#include <chrono>
#include <memory>

namespace GameEngine
{
    class PhysicsWorld; // owned via unique_ptr; full definition lives in the .cpp

    class TextBlockManager : public IDrawable
    {
    private:
        std::unique_ptr<PhysicsWorld> m_physics; // rigid-body simulation for settled/toppling blocks
        std::deque<std::shared_ptr<TextBlock>> m_blockDeque;
        std::chrono::steady_clock::time_point m_lastSpawnTime;
        bool m_running;
        bool m_limitReached;
        float m_spawnInterval; // Time interval between spawning TextBlocks
        float m_elapsedTime = 0;
        std::shared_ptr<InputManager> m_inputManager;
        std::shared_ptr<WordManager> m_wordManager;
        void SetHorizontalMovement(TextBlock* block);
        TextBlock* m_horizontalMovingBlock; // Pointer to the block that can move horizontally
        void HandOffToPhysics(std::shared_ptr<TextBlock>& block); // convert a landed block into a rigid body
        float FindSupportTopY(TextBlock* block) const;            // highest surface the block can rest on
        void UnregisterAllTextBlocks();
        void UpdateTimer(float dt);
        int m_priority;// draw priority

    public:
        TextBlockManager(float spawnIntervalSeconds, std::shared_ptr<InputManager> inputManager, std::shared_ptr<WordManager> wordManager);
        ~TextBlockManager(); // defined in .cpp where PhysicsWorld is complete
        void ClearBlockDeque();
        void GenerateTextBlock();
        void Update(float dt) override;
        void Draw() override;
        void PopTextBlock();
        void ToggleRunning();
        bool IsRunning();
        bool TextBlockLimitReached();
        void SetTextBlockLimitReached(bool b);
        void DestroyActiveTextBlock();
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
    };
}