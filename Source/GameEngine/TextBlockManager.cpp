#include "TextBlockManager.h"
#include "Color.h"
#include "GlobalPreprocessorFlags.h"
#include "Common.h"
#include "PhysicsWorld.h"
#include <iostream>
#include <algorithm>

/// <summary>
/// TextBlockManager manages TextBlock creation and removal
/// </summary>

using namespace GameEngine;

TextBlockManager::TextBlockManager(float spawnIntervalSeconds, std::shared_ptr<InputManager> inputManager, std::shared_ptr<WordManager> wordManager)
    : m_physics(std::make_unique<PhysicsWorld>()),
      m_running(false), m_spawnInterval(spawnIntervalSeconds), m_inputManager(inputManager), m_elapsedTime(0),
      m_limitReached(false), m_horizontalMovingBlock(nullptr), m_wordManager(wordManager), m_priority(0)
{
    m_lastSpawnTime = std::chrono::steady_clock::now();

    // Static play-area bounds. Landed blocks rest with their *top* at Common::FLOOR (matching the
    // original game), so the physical floor sits one block-height lower, under their bottoms.
    // Gravity here is the *physics* gravity (m/s^2) for settled/toppling blocks, independent of
    // Common::GRAVITY (which is the tiny per-pixel descent rate of the player-controlled block).
    m_physics->CreateBounds(static_cast<float>(Common::EDGE_LEFT),
                            static_cast<float>(Common::EDGE_RIGHT),
                            static_cast<float>(Common::FLOOR + Common::FONT_HEIGHT),
                            0.0f);
}

TextBlockManager::~TextBlockManager() = default;

void TextBlockManager::ClearBlockDeque()
{
    // Unregister them from the InputManager and remove any rigid bodies from the simulation.
    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        if (m_blockDeque[i])
        {
            m_inputManager->UnregisterObserver(m_blockDeque[i].get());
            m_physics->DestroyBody(m_blockDeque[i]->GetBody());
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

    // There is only ever one player-controlled block falling; everything else is owned by Box2D.
    std::shared_ptr<TextBlock> falling;
    for (auto& b : m_blockDeque)
    {
        if (!b->IsPhysicsControlled())
        {
            falling = b;
            break;
        }
    }

    if (falling)
    {
        falling->Update(dt); // custom controlled descent

        // Once it reaches the floor or the top of the settled stack, hand it to the physics
        // engine, which settles or topples it realistically, and give the player the next word.
        AABB& box = falling->GetBox();
        if (box.y + box.h >= FindSupportTopY(falling.get()))
        {
            HandOffToPhysics(falling);
            GenerateTextBlock();
            falling = nullptr;
        }
    }

    // Advance the rigid-body simulation, then mirror each body back onto its block.
    m_physics->Step(dt);

    bool gameOver = false;
    for (auto& b : m_blockDeque)
    {
        if (!b->IsPhysicsControlled())
            continue;

        b->SyncFromBody();
        if (b->GetBox().y <= Common::CEILING) // the stack has reached the top -> game over
            gameOver = true;
    }

    if (gameOver)
    {
        ToggleRunning();
        m_limitReached = true;
        UnregisterAllTextBlocks();
    }
    else if (falling && falling->IsActive() && falling->GetMovingState())
    {
        SetHorizontalMovement(falling.get());
    }
    else
    {
        SetHorizontalMovement(nullptr);
    }

    UpdateTimer(dt);
}


// Convert a falling block into a dynamic Box2D body, carrying over its current downward speed so
// the motion is seamless, and remove it from player control.
void TextBlockManager::HandOffToPhysics(std::shared_ptr<TextBlock>& block)
{
    AABB& box = block->GetBox();
    b2Body* body = m_physics->CreateBlockBody(block->GetPosition().first,
                                              block->GetPosition().second,
                                              static_cast<float>(box.w),
                                              static_cast<float>(box.h),
                                              block->GetVelocity());
    block->SetActiveState(false);
    m_inputManager->UnregisterObserver(block.get());
    block->SetBody(body); // marks the block physics-controlled and stops its custom motion
}

// Highest surface (smallest y) the given block could land on: the floor by default, or the top of
// any already-settled block it overlaps horizontally.
float TextBlockManager::FindSupportTopY(TextBlock* block) const
{
    const AABB& a = block->GetBox();
    // The player-controlled fall stops the block's *top* at Common::FLOOR, so its bottom lands at
    // FLOOR + a.h. Using the block's own height (not Common::FONT_HEIGHT) keeps the hand-off
    // threshold exactly aligned with that stop point, so a block reliably reaches it.
    float surface = static_cast<float>(Common::FLOOR + a.h); // where the block's bottom rests on the floor

    for (const auto& other : m_blockDeque)
    {
        if (other.get() == block || !other->IsPhysicsControlled())
            continue;

        const AABB& b = other->GetBox();
        const bool overlapX = (a.x < b.x + b.w) && (a.x + a.w > b.x);
        if (overlapX && b.y >= a.y && b.y < surface)
            surface = static_cast<float>(b.y);
    }

    return surface;
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
            Common::s_currentTextBlockWidth = static_cast<int>(size.first);

            // Unregister the TextBlock from the InputManager and drop any rigid body it owns
            m_inputManager->UnregisterObserver(it->get());
            m_physics->DestroyBody(it->get()->GetBody());

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