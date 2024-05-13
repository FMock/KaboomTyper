#include "TextBlockGenerator.h"
#include "Colors.h"

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
    static std::uniform_int_distribution<int> y_dist(150, 900 - static_cast<int>(xPadding));

    int randomX = x_dist(gen);
    int randomY = y_dist(gen);

    m_blockDeque.push_back(std::make_unique<TextBlock>(randomX, randomY, text, randomColor));
}


void TextBlockGenerator::Update(float dt)
{
    if (!m_running)
        return;

    m_elapsedTime += dt;

    if (m_elapsedTime >= m_spawnInterval)
    {
        GenerateTextBlock("New TextBlock");
        m_elapsedTime = 0.0f; // Reset elapsed time
    }
}


void TextBlockGenerator::Draw()
{
    // Draw TextBlocks in the stack
    for (const auto& block : m_blockDeque)
    {
        block->Draw();
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
