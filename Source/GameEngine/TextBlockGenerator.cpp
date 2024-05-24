#include "TextBlockGenerator.h"
#include "Colors.h"
#include <iostream>

/// <summary>
/// TextBlockGenerator manages TextBlock creation and removal
/// </summary>

using namespace GameEngine;

TextBlockGenerator::TextBlockGenerator(float spawnIntervalSeconds, std::shared_ptr<InputManager> inputManager)
    : m_running(false), m_spawnInterval(spawnIntervalSeconds), m_inputManager(inputManager), m_elapsedTime(0), m_limitReached(false), m_horizontalMovingBlock(nullptr)
{
    m_lastSpawnTime = std::chrono::steady_clock::now();
}

void TextBlockGenerator::ClearBlockDeque()
{
    // they're going bye, bye so unregister them
    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        m_inputManager->UnregisterObserver(m_blockDeque[i].get());
    }

    // now delete 'em
    m_blockDeque.clear();
}

void GameEngine::TextBlockGenerator::GenerateTextBlock()
{
    // Get a string based on the current category
    std::string text = "New String";

    Common::SetActiveText(text);

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
    m_blockDeque.back()->Activate();
}

void TextBlockGenerator::Update(float dt)
{
    if (!m_running)
        return;

    TextBlock* horizontalMovableBlock = nullptr;
    bool gameOver = false;

    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        auto& blockA = m_blockDeque[i];
        blockA->Update(dt);
        auto blockAPosition = blockA->GetPosition();

        if (blockA->GetMovingState())
        {
            if (blockAPosition.second >= Common::FLOOR)
            {
                blockA->SetMovingState(false);
                blockA->SetPosition(blockAPosition.first, Common::FLOOR);
                m_inputManager->UnregisterObserver(blockA.get());

                GenerateTextBlock(); // Previous TextBlock reached the botton so create another one
            }
            else
            {
                horizontalMovableBlock = blockA.get();
            }
        }
        else if (blockAPosition.second <= Common::CEILING)
        {
            ToggleRunning(); // Game Over
            m_limitReached = true;
            gameOver = true;
        }

        if (gameOver)
            break;

        // Check for collisions with other TextBlocks
        for (size_t j = 0; j < m_blockDeque.size(); ++j)
        {
            if (i == j)
                continue;

            auto& blockB = m_blockDeque[j];
            HandleCollisions(*blockA, blockAPosition.second, *blockB);
        }
    }

    if (gameOver)
    {
        // Unregister all TextBlocks from the InputManager
        for (auto& block : m_blockDeque)
        {
            m_inputManager->UnregisterObserver(block.get());
        }
    }
    else
    {
        // Set the block that can move horizontally, if there is one
        SetHorizontalMovement(horizontalMovableBlock);
    }

    m_elapsedTime += dt;

    if (m_elapsedTime >= m_spawnInterval)
    {
        // Do something
        std::cout << "Timer has elapsed" << std::endl;

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
        m_inputManager->UnregisterObserver(&blockA); // blockA stacked on top of another TextBlock. Unregister it from m_inputManager

        auto blockBPosition = blockB.GetPosition();
        blockAYPosition = blockBPosition.second - blockA.GetBox().h;
        blockA.SetPosition(blockA.GetPosition().first, blockAYPosition);
        blockA.SetVelocity(0.0f);

        GenerateTextBlock(); // Give user another TextBlock
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