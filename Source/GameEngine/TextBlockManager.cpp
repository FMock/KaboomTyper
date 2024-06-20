#include "TextBlockManager.h"
#include "Color.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>

/// <summary>
/// TextBlockManager manages TextBlock creation and removal
/// </summary>

using namespace GameEngine;

TextBlockManager::TextBlockManager(float spawnIntervalSeconds, std::shared_ptr<InputManager> inputManager)
    : m_running(false), m_spawnInterval(spawnIntervalSeconds), m_inputManager(inputManager), m_elapsedTime(0), m_limitReached(false), m_horizontalMovingBlock(nullptr), m_wordManager(std::make_unique<WordManager>())
{
    m_lastSpawnTime = std::chrono::steady_clock::now();
}

void TextBlockManager::ClearBlockDeque()
{
    // Unregister them from the InputManager
    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        if (m_blockDeque[i])
        {
            m_inputManager->UnregisterObserver(m_blockDeque[i].get());
        }
        else
        {
            std::cerr << "Warning: Attempting to unregister a null pointer" << std::endl;
        }
    }

    // Now delete 'em
    m_blockDeque.clear();
}


void GameEngine::TextBlockManager::GenerateTextBlock()
{
    // Check if there's already an active TextBlock
    auto it = std::find_if(m_blockDeque.begin(), m_blockDeque.end(), [](const std::shared_ptr<TextBlock>& block)
        {
            return block->IsActive();
        });

    if (it != m_blockDeque.end())
    {
        // There's already an active TextBlock, so don't create a new one
        return;
    }

    // Get a string based on the current word category
    std::string text = m_wordManager->GetNextWord();

    Common::SetActiveText(text);

    int xPadding = static_cast<int>(text.size()) * 24; // 24 is the texture width
    auto randomColor = GetRandomColor();

    Common::s_previousColor = Common::s_currentColor;
    Common::s_currentColor = randomColor;

    // Generate Random position x
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> x_dist(Common::EDGE_LEFT, Common::EDGE_RIGHT - xPadding);

    int xPos = x_dist(gen);
    if (xPos + xPadding >= Common::EDGE_RIGHT)
        xPos = Common::EDGE_LEFT;

    int yPos = Common::TEXTBLOCK_VERTICAL_START_POSITION;

    // Create the new TextBlock
    auto newBlock = std::make_shared<TextBlock>(xPos, yPos, text, randomColor);
    newBlock->SetTextBlockWidth(xPadding);

    // Register with the InputManager
    m_inputManager->RegisterObserver(newBlock);
    m_blockDeque.push_back(std::move(newBlock));
    m_blockDeque.back()->Activate();
}

void TextBlockManager::Update(float dt)
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
            if (blockAPosition.second >= Common::FLOOR) // TextBlock landed on the bottom
            {
                HandleLanding(blockA);
                GenerateTextBlock(); // Previous TextBlock reached the bottom so create another one
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
            break;
        }

        // Check for collisions with other TextBlocks
        for (size_t j = 0; j < m_blockDeque.size(); ++j)
        {
            if (i == j)
                continue;

            auto& blockB = m_blockDeque[j];
            HandleCollisions(*blockA, blockAPosition.second, *blockB);
        }

        if (gameOver)
            break;
    }

    if (gameOver)
    {
        UnregisterAllTextBlocks();
    }
    else
    {
        // Set the block that can move horizontally, if there is one
        SetHorizontalMovement(horizontalMovableBlock);
    }

    UpdateTimer(dt);
}


void TextBlockManager::HandleLanding(std::shared_ptr<TextBlock>& block)
{
    block->SetMovingState(false);
    block->SetPosition(block->GetPosition().first, Common::FLOOR);
    block->SetActiveState(false);
    m_inputManager->UnregisterObserver(block.get());
}

void TextBlockManager::UnregisterAllTextBlocks()
{
    for (auto& block : m_blockDeque)
    {
        m_inputManager->UnregisterObserver(block.get());
    }
}

void TextBlockManager::UpdateTimer(float dt)
{
    m_elapsedTime += dt;

    if (m_elapsedTime >= m_spawnInterval)
    {
#if DEBUG
        // Do something
        std::cout << "Timer has elapsed" << std::endl;
#endif
        m_elapsedTime = 0.0f;
    }
}


void TextBlockManager::Draw()
{
    for (const auto& block : m_blockDeque)
    {
        block->Draw();

#if DEBUG
        if (block->m_isHit)
            std::cout << "block hit" << std::endl;
#endif
    }
}

void TextBlockManager::PopTextBlock()
{
    if (!m_blockDeque.empty())
        m_blockDeque.pop_back();
}

void TextBlockManager::ToggleRunning()
{
    m_running = !m_running;
}

bool TextBlockManager::IsRunning()
{
    return m_running;
}

bool GameEngine::TextBlockManager::TextBlockLimitReached()
{
    return m_limitReached;
}

void GameEngine::TextBlockManager::SetTextBlockLimitReached(bool b)
{
    m_limitReached = b;
}

void GameEngine::TextBlockManager::HandleCollisions(TextBlock& blockA, float& blockAYPosition, TextBlock& blockB)
{
    if (blockA.GetMovingState() && !blockB.GetMovingState() && Common::AABBIntersect(blockA.GetBox(), blockB.GetBox()))
    {
        blockA.SetMovingState(false);
        m_inputManager->UnregisterObserver(&blockA); // blockA stacked on top of another TextBlock. Unregister it from m_inputManager

        auto blockBPosition = blockB.GetPosition();
        blockAYPosition = blockBPosition.second - blockA.GetBox().h;
        blockA.SetPosition(blockA.GetPosition().first, blockAYPosition);
        blockA.SetVelocity(0.0f);
        blockA.SetActiveState(false);

        GenerateTextBlock(); // Give user another TextBlock
    }
}

void TextBlockManager::DestroyActiveTextBlock()
{
    if (!m_blockDeque.empty())
    {
        // Find the active TextBlock
        auto it = std::find_if(m_blockDeque.begin(), m_blockDeque.end(), [](const std::shared_ptr<TextBlock>& block)
            {
                return block->IsActive();
            });

        // An active TextBlock was found
        if (it != m_blockDeque.end())
        {
            // Set position for others to use (like Firework)
            Common::s_currentPosition = it->get()->GetPosition();
            // How wide is the TextBlock that's about to be destroyed?
            auto size = it->get()->GetSize();
            Common::s_currentTextBlockWidth = size.first;

            // Unregister the TextBlock from the InputManager
            m_inputManager->UnregisterObserver(it->get());

            // Delete the active TextBlock from the container
            m_blockDeque.erase(it);

            // Give the player a new TextBlock
            GenerateTextBlock();
        }
    }
}

void TextBlockManager::SetHorizontalMovement(TextBlock* block)
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