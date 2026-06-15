#pragma once

#include "TextBlock.h"
#include "InputManager.h"
#include "WordManager.h"
#include "IDrawable.h"
#include "Common.h" // for Difficulty
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

        // Difficulty-driven spawning: how many blocks may fall at once and how their extra
        // blocks are staggered in. Easy=1, Normal=up to 2 (50% chance), Hard=up to 3.
        Difficulty m_difficulty = Difficulty::Normal;
        float m_staggerTimer = 0.0f;       // time since the last staggered spawn in the current wave
        int m_pendingExtraSpawns = 0;      // extra blocks still queued for the current wave
        static constexpr float STAGGER_DELAY = 1.0f; // seconds between staggered spawns
        int CountFallingBlocks() const;    // blocks still under player control (not physics)
        int ComputeExtraSpawns();          // extra blocks to queue for a new wave, per difficulty
        void ReactivateFallingBlock();     // give "active" status to a remaining falling block

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
        void SetDifficulty(Difficulty d) { m_difficulty = d; }
        void DestroyActiveTextBlock();
        // Destroy the falling block whose word matches; returns true if one was found.
        bool DestroyMatchingTextBlock(const std::string& word);
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
    };
}