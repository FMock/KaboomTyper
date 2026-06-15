#include "TextBlockManager.h"
#include "Color.h"
#include "GlobalPreprocessorFlags.h"
#include "Common.h"
#include "PhysicsWorld.h"
#include <iostream>
#include <algorithm>
#include <random>

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

    // Reset the wave scheduler so a fresh game starts cleanly.
    m_pendingExtraSpawns = 0;
    m_staggerTimer = 0.0f;
}


void GameEngine::TextBlockManager::GenerateTextBlock()
{
    // Spawning is governed by the difficulty scheduler in Update(); this method always creates a
    // block. The newest block becomes the active (arrow-controllable) one via Activate().

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

    // Update every player-controlled (non-physics) falling block. Several may be falling at once
    // depending on the difficulty. Each is handed off to Box2D the moment it reaches the floor or
    // the top of the settled stack. HandOffToPhysics only flips the block's state (it does not
    // modify the deque), so iterating here is safe.
    for (auto& b : m_blockDeque)
    {
        if (b->IsPhysicsControlled())
            continue;

        b->Update(dt); // custom controlled descent

        AABB& box = b->GetBox();
        if (box.y + box.h >= FindSupportTopY(b.get()))
        {
            HandOffToPhysics(b);
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
        return;
    }

    // Difficulty-driven spawn scheduler: always keep at least one block falling, and (for
    // Normal/Hard) stagger additional blocks ~1s apart so they don't obscure one another. A new
    // wave begins only once every falling block from the previous wave has landed or been typed.
    const int fallingCount = CountFallingBlocks();
    const int maxConcurrent = (m_difficulty == Difficulty::Easy)   ? 1
                            : (m_difficulty == Difficulty::Normal) ? 2
                                                                   : 3;

    if (fallingCount == 0)
    {
        GenerateTextBlock();               // lead block of a new wave
        m_staggerTimer = 0.0f;
        m_pendingExtraSpawns = ComputeExtraSpawns();
    }
    else if (m_pendingExtraSpawns > 0 && fallingCount < maxConcurrent)
    {
        m_staggerTimer += dt;
        if (m_staggerTimer >= STAGGER_DELAY)
        {
            m_staggerTimer = 0.0f;
            GenerateTextBlock();
            --m_pendingExtraSpawns;
        }
    }

    // Arrow-key horizontal control follows the single active falling block (the newest spawned).
    TextBlock* activeFalling = nullptr;
    for (auto& b : m_blockDeque)
    {
        if (!b->IsPhysicsControlled() && b->IsActive() && b->GetMovingState())
        {
            activeFalling = b.get();
            break;
        }
    }
    SetHorizontalMovement(activeFalling);

    UpdateTimer(dt);
}

// Number of blocks still under player control (i.e. falling, not yet owned by Box2D).
int TextBlockManager::CountFallingBlocks() const
{
    int count = 0;
    for (const auto& b : m_blockDeque)
        if (!b->IsPhysicsControlled())
            ++count;
    return count;
}

// Extra blocks (beyond the lead) to queue at the start of a wave, per difficulty.
int TextBlockManager::ComputeExtraSpawns()
{
    switch (m_difficulty)
    {
    case Difficulty::Normal:
    {
        // 50% chance a single second block joins the wave ~1s later.
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> coin(0, 1);
        return coin(gen);
    }
    case Difficulty::Hard:
        return 2; // lead + 2 = up to three falling at once
    case Difficulty::Easy:
    default:
        return 0;
    }
}

// After a falling block is destroyed, ensure a remaining falling block holds "active" status so
// arrow-key control and the active word keep working.
void TextBlockManager::ReactivateFallingBlock()
{
    const bool anyActive = std::any_of(m_blockDeque.begin(), m_blockDeque.end(),
        [](const std::shared_ptr<TextBlock>& b) { return b->IsActive(); });
    if (anyActive)
        return;

    for (auto it = m_blockDeque.rbegin(); it != m_blockDeque.rend(); ++it)
    {
        if (!(*it)->IsPhysicsControlled())
        {
            (*it)->Activate();
            Common::SetActiveText((*it)->GetWord());
            break;
        }
    }
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

            // Hand "active" status to a remaining falling block (the scheduler refills new waves).
            ReactivateFallingBlock();
        }
    }
}

// Destroy the falling block whose word matches the typed text. Only blocks still under player
// control (not settled) can be typed. Returns true if a matching block was found and removed.
bool TextBlockManager::DestroyMatchingTextBlock(const std::string& word)
{
    auto it = std::find_if(m_blockDeque.begin(), m_blockDeque.end(),
        [&word](const std::shared_ptr<TextBlock>& block)
        {
            return !block->IsPhysicsControlled() && block->GetWord() == word;
        });

    if (it == m_blockDeque.end())
        return false;

    // Set position/size for others to use (like Firework).
    Common::s_currentPosition = it->get()->GetPosition();
    auto size = it->get()->GetSize();
    Common::s_currentTextBlockWidth = static_cast<int>(size.first);

    m_inputManager->UnregisterObserver(it->get());
    m_physics->DestroyBody(it->get()->GetBody()); // null-safe; a falling block owns no body
    m_blockDeque.erase(it);

    ReactivateFallingBlock();
    return true;
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