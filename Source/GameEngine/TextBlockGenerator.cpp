#include "TextBlockGenerator.h"
#include "Colors.h"
#include <iostream>

/// <summary>
/// TextBlockGenerator creates TextBlock objects and checks for collisions
/// </summary>

using namespace GameEngine;

TextBlockGenerator::TextBlockGenerator(float spawnIntervalSeconds)
    : m_running(false), m_spawnInterval(spawnIntervalSeconds), m_elapsedTime(0)
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

    m_blockDeque.push_back(std::make_unique<TextBlock>(randomX, yPos, text, randomColor));
}

void TextBlockGenerator::Update(float dt)
{
    if (!m_running)
        return;

    for (const auto& block : m_blockDeque)
    {
        block->Update(dt);
    }

    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        auto& blockA = m_blockDeque[i];

        auto blockAPosition = blockA->GetPosition();

        if (blockA->GetMovingState() && blockAPosition.second >= Common::FLOOR)
        {
            blockA->SetMovingState(false);
            blockA->SetPosition(blockAPosition.first, Common::FLOOR);
            continue;
        }

        for (size_t j = 0; j < m_blockDeque.size(); ++j)
        {
            if (i == j) continue;

            auto& blockB = m_blockDeque[j];
            HandleCollisions(*blockA, blockAPosition.second, *blockB);
        }
    }

    m_elapsedTime += dt;

    if (m_elapsedTime >= m_spawnInterval)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> num_dist(0, 800);
        int randomNumber = num_dist(gen);

        if (randomNumber > 400)
        {
            GenerateTextBlock("New TextBlock");
        }
        else
        {
            GenerateTextBlock("Cat");
        }

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

void GameEngine::TextBlockGenerator::HandleCollisions(TextBlock& blockA, float& blockAYPosition, TextBlock& blockB)
{
    if (blockA.GetMovingState() && !blockB.GetMovingState() && Common::AABBIntersect(blockA.GetBox(), blockB.GetBox()))
    {
        blockA.SetMovingState(false);
        auto blockBPosition = blockB.GetPosition();
        blockAYPosition = blockBPosition.second - blockA.GetBox().h;
        blockA.SetPosition(blockA.GetPosition().first, blockAYPosition);
        blockA.SetVelocity(0.0f);
    }
}
