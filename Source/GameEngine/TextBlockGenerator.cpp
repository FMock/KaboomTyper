#include "TextBlockGenerator.h"
#include "Colors.h"
#include <iostream>

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

    // Check for collisions
    for (size_t i = 0; i < m_blockDeque.size(); ++i)
    {
        for (size_t j = i + 1; j < m_blockDeque.size(); ++j)
        {
            if (Common::AABBIntersect(m_blockDeque[i]->GetBox(), m_blockDeque[j]->GetBox()))
            {
                m_blockDeque[i]->m_isHit = true;
                m_blockDeque[j]->m_isHit = true;
            }
        }
    }

    m_elapsedTime += dt;

    if (m_elapsedTime >= m_spawnInterval)
    {
        // Generate Random Number
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

        m_elapsedTime = 0.0f; // Reset elapsed time
    }
}


void TextBlockGenerator::Draw()
{
    // Draw TextBlocks in the stack
    for (const auto& block : m_blockDeque)
    {
        block->Draw();
        if(block->m_isHit)
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
