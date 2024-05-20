#include "TextBlockGenerator.h"
#include "Colors.h"
#include <iostream>

/// <summary>
/// TextBlockGenerator creates TextBlock objects and checks for collisions
/// </summary>

using namespace GameEngine;

TextBlockGenerator::TextBlockGenerator(float spawnIntervalSeconds, std::shared_ptr<InputManager> inputManager)
    : m_running(false), m_spawnInterval(spawnIntervalSeconds), m_inputManager(inputManager), m_elapsedTime(0), m_limitReached(false)
{
    m_lastSpawnTime = std::chrono::steady_clock::now();
}

void TextBlockGenerator::ClearBlockDeque()
{
    m_blockDeque.clear();
}

void TextBlockGenerator::GenerateTextBlock(std::string text)
{
    auto xPadding = text.size() * 24;
    auto randomColor = GetRandomColor();

    // Generate Random position x
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> x_dist(0, 800 - static_cast<int>(xPadding));

    int randomX = x_dist(gen);
    int yPos = 50;

    // Create the new TextBlock
    auto newBlock = std::make_unique<TextBlock>(randomX, yPos, text, randomColor);

    // Register with the InputManager
    m_inputManager->RegisterObserver(newBlock.get());
    m_blockDeque.push_back(std::move(newBlock));
}

void TextBlockGenerator::Update(float dt)
{
    if (!m_running)
        return;

    TextBlock* horizontalMovableBlock = nullptr;

    for (const auto& block : m_blockDeque)
    {
        block->Update(dt);

        if (block->GetMovingState() && block->GetPosition().second >= Common::FLOOR)
        {
            block->SetMovingState(false);
            block->SetPosition(block->GetPosition().first, Common::FLOOR);
            m_inputManager->UnregisterObserver(block.get());
        }
        else if (block->GetMovingState())
        {
            horizontalMovableBlock = block.get();
        }
    }

    // Set the block that can move horizontally, if there is one
    SetHorizontalMovement(horizontalMovableBlock);

    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        auto& blockA = m_blockDeque[i];
        auto blockAPosition = blockA->GetPosition();

        if (!blockA->GetMovingState() && blockAPosition.second <= Common::CEILING)
        {
            ToggleRunning(); // Game Over
            m_limitReached = true;
            
            // Unregister the TextBlocks from the InputManager
            for (size_t j = 0; j < m_blockDeque.size(); ++j)
            {
                m_inputManager->UnregisterObserver(m_blockDeque[j].get());
            }

            break;
        }

        // Check for collisions with other TextBlocks
        for (size_t j = 0; j < m_blockDeque.size(); ++j)
        {
            if (i == j) continue;

            auto& blockB = m_blockDeque[j];
            HandleCollisions(*blockA, blockAPosition.second, *blockB);

            // Ensure only one block can move horizontally
            if (blockA->GetMovingState() && !blockB->GetMovingState() && Common::AABBIntersect(blockA->GetBox(), blockB->GetBox()))
            {
                SetHorizontalMovement(blockA.get());
            }
        }
    }

    m_elapsedTime += dt;

    if (m_elapsedTime >= m_spawnInterval)
    {
        GenerateTextBlock("New TextBlock");
        m_elapsedTime = 0.0f;
    }
}

void TextBlockGenerator::Draw()
{
    for (const auto& block : m_blockDeque)
    {
        block->Draw();
        if (block->m_isHit)
            std::cout << "block hit" << std::endl;
    }
}

void TextBlockGenerator::PopTextBlock()
{
    if (!m_blockDeque.empty())
        m_blockDeque.pop_back();
}

void TextBlockGenerator::ToggleRunning()
{
    m_running = !m_running;
}

bool TextBlockGenerator::IsRunning()
{
    return m_running;
}

bool GameEngine::TextBlockGenerator::TextBlockLimitReached()
{
    return m_limitReached;
}

void GameEngine::TextBlockGenerator::SetTextBlockLimitReached(bool b)
{
    m_limitReached = b;
}

void GameEngine::TextBlockGenerator::HandleCollisions(TextBlock& blockA, float& blockAYPosition, TextBlock& blockB)
{
    if (blockA.GetMovingState() && !blockB.GetMovingState() && Common::AABBIntersect(blockA.GetBox(), blockB.GetBox()))
    {
        blockA.SetMovingState(false);
        m_inputManager->UnregisterObserver(&blockA); // blockA landed on another TextBlock. Unregister it from m_inputManager

        auto blockBPosition = blockB.GetPosition();
        blockAYPosition = blockBPosition.second - blockA.GetBox().h;
        blockA.SetPosition(blockA.GetPosition().first, blockAYPosition);
        blockA.SetVelocity(0.0f);
    }
}

void TextBlockGenerator::SetHorizontalMovement(TextBlock* block)
{
    // Set m_canMoveHorizontal to false for all blocks
    for (const auto& b : m_blockDeque)
    {
        b->SetCanMoveHorizontal(false);
    }

    // Set m_canMoveHorizontal to true for the specified block
    if (block != nullptr)
    {
        block->SetCanMoveHorizontal(true);
        m_horizontalMovingBlock = block;
    }
    else
    {
        m_horizontalMovingBlock = nullptr;
    }
}
