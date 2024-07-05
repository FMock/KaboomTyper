#include "DecorativeRectangleDrawable.h"
#include "DrawUtils.h"
#include <iostream>
#include <unordered_set>

using namespace GameEngine;
using namespace DrawUtilities;

void DecorativeRectangle::Initialize(int x, int y, int width, int height, Colors rectColor, bool filled)
{
    // Call the base class Initialize
    RectangleDrawable::Initialize(x, y, width, height, rectColor, filled);

    // Call the specialized initialization
    InitializeDecorativeRectangle();
}

void DecorativeRectangle::InitializeDecorativeRectangle()
{
    m_animationTime = 0.0f;
    m_animationSpeed = 1.50f; // Speed of the animation

    // clock-wise animation settings
    m_clockWiseAnimationSpeed = 1.0f; // increase speed to exceed threshold faster and move animation to next frame
    m_clockWiseAnimationThreshold = 1.0f;

    // random animation settings
    m_randomAnimationSpeed = 2.0f; // increase speed to exceed threshold faster and move animation to next frame
    m_randomAnimationThreshold = 1.0f;

    m_threshold = 0.001f;
    m_currentColorIndex = 0;
    m_animate = false;
    m_animateRandom = false;
    m_smallRectangles.clear();
    m_lap = 0;

    auto color_1 = m_colorToStringMap[Colors::BLUE];
    auto color_2 = m_colorToStringMap[Colors::RED];
    auto color_3 = m_colorToStringMap[Colors::YELLOW];
    auto color_4 = m_colorToStringMap[Colors::GREEN];
    auto color_5 = m_colorToStringMap[Colors::ORANGE];
    auto color_6 = m_colorToStringMap[Colors::PURPLE];

    m_blueTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_1.c_str()];
    m_redTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_2.c_str()];
    m_yellowTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_3.c_str()];
    m_greenTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_4.c_str()];
    m_orangeTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_5.c_str()];
    m_purpleTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_6.c_str()];

    m_colorTextures = { m_blueTexture, m_redTexture, m_yellowTexture, m_greenTexture, m_orangeTexture, m_purpleTexture };

    int smallWidth = 10; // width of smaller rectangles
    int smallHeight = 10; // height of smaller rectangles

    // Calculate number of rectangles needed for each side
    int horizontalCount = m_width / smallWidth;
    int verticalCount = m_height / smallHeight;

    // Top side
    for (int i = 0; i < horizontalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y, smallWidth, smallHeight });
    }

    // Right side
    for (int i = 0; i < verticalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + m_width - smallWidth, m_y + i * smallHeight, smallWidth, smallHeight });
    }

    // Bottom side
    for (int i = horizontalCount - 1; i >= 0; --i)
    {
        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y + m_height - smallHeight, smallWidth, smallHeight });
    }

    // Left side
    for (int i = verticalCount - 1; i >= 0; --i)
    {
        m_smallRectangles.push_back({ m_x, m_y + i * smallHeight, smallWidth, smallHeight });
    }

    m_elapsedTime = 0.0f;
    m_isVisible = true;
    m_lastUpdateTime = std::chrono::steady_clock::now();
}

void DecorativeRectangle::Update(float dt)
{
    m_deltaTime = dt; // Update delta time with the passed dt
    m_elapsedTime += dt;


    // Toggle visibility every 0.5 seconds
    if (m_elapsedTime >= 0.5f)
    {
        m_isVisible = !m_isVisible;
        m_elapsedTime = 0.0f;
    }

    if (m_animate)
    {
        m_animationTime += dt * m_animationSpeed;

        // Change color every second (for example)
        if (m_animationTime >= 1.0f)
        {
            m_animationTime = 0.0f;
            m_currentColorIndex = (m_currentColorIndex + 1) % 2; // Alternate between 0 and 1
        }
    }

    if (m_animateClockWise)
    {
        static int currentIndex = 0; // Index of the current small rectangle

    }
}

void DecorativeRectangle::SetAnimate(bool animate)
{
    m_animate = animate;
}

void GameEngine::DecorativeRectangle::SetAnimateClockwise(bool animate)
{
    m_animateClockWise = animate;
}

void GameEngine::DecorativeRectangle::SetAnimateRandom(bool animate)
{
    m_animateRandom = animate;
}

void GameEngine::DecorativeRectangle::Draw()
{
    DrawRectangleWithRectangles();
}

void DecorativeRectangle::DrawRectangleWithRectangles()
{
    static int currentIndex = 0; // Index of the current small rectangle
    static std::vector<int> randomIndices(10, 0);   // TODO: TURN THESE INTO STATIC MEMBERS AND PLACE UPDATE LOGIC IN UPDATE METHOD
    static std::vector<GLuint> randomColorTextures(10);   // TODO: TURN THESE INTO STATIC MEMBERS AND PLACE UPDATE LOGIC IN UPDATE METHOD
    static float elapsedTimeRandom = 0.0f; // Time elapsed since the last change   // TODO: TURN THESE INTO STATIC MEMBERS AND PLACE UPDATE LOGIC IN UPDATE METHOD
    static float elapsedTimeClockwise = 0.0f; // Time elapsed since the last change  // TODO: TURN THESE INTO STATIC MEMBERS AND PLACE UPDATE LOGIC IN UPDATE METHOD

    if (m_animateClockWise)
    {
        elapsedTimeClockwise += m_deltaTime * m_clockWiseAnimationSpeed;

        if (elapsedTimeClockwise >= m_clockWiseAnimationThreshold) // if the elapsed time is greater than the threshold
        {
            elapsedTimeClockwise -= m_clockWiseAnimationThreshold; // Subtract the threshold to handle any overflow
            currentIndex = (currentIndex + 1) % m_smallRectangles.size();

            // Check if we've completed a full lap
            if (currentIndex == 0)
            {
                ++m_lap;
            }
        }

        // Draw the current rectangle with a different color
        const auto& rect = m_smallRectangles[currentIndex];
        DrawUtilities::glDrawTexture(m_yellowTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);

        for (const auto& rect : m_smallRectangles)
        {
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
    else if(m_animate)
    {
        for (const auto& rect : m_smallRectangles)
        {
            GLuint textureToUse = (m_currentColorIndex == 0) ? m_blueTexture : m_redTexture;

            DrawUtilities::glDrawTexture(textureToUse, rect.x, rect.y, (float)rect.width, (float)rect.height);
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
    else if (m_animateRandom) // Animate drawing the colored squares
    {
        // Draw the frame of small rectangles
        for (const auto& rect : m_smallRectangles)
        {
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }

        // Draw the randomly colored rectangles at random positions
        elapsedTimeRandom += m_deltaTime * m_randomAnimationSpeed;

        if (elapsedTimeRandom >= m_randomAnimationThreshold)
        {
            elapsedTimeRandom -= m_randomAnimationThreshold; // subtract the threshold to handle any overflow

            UpdateRandomIndicesAndColors(randomIndices, randomColorTextures, m_smallRectangles, m_colorTextures);
        }

        // Draw 15 small rectangles at random positions and random colors
        for (int i = 0; i < 10; ++i)
        {
            const auto& rect = m_smallRectangles[randomIndices[i]];
            DrawUtilities::glDrawTexture(randomColorTextures[i], rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
    else
    {
        // Draw rectangles with outline only
        for (const auto& rect : m_smallRectangles)
        {
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }

        if (m_isVisible) // Set for every 1/2 second in Update
        {
            // Do something evey 1/2 second
        }
    }
}

GLuint DecorativeRectangle::GetRandomTexture(const std::vector<GLuint>& textures)
{
    // Seed with a real random value, if available
    std::random_device rd;

    // Choose a random number between 0 and textures.size() - 1
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, static_cast<int>(textures.size()) - 1);

    // Return the randomly chosen texture
    return textures[dis(gen)];
}

int DecorativeRectangle::GetRandomIndex(const std::vector<Rectangle>& rectangles)
{
    if (rectangles.empty())
    {
        throw std::out_of_range("DecorativeRectangle::GetRandomIndex() The Rectangle vector is empty.");
    }

    std::uniform_int_distribution<> dis(0, static_cast<int>(rectangles.size()) - 1);
    return dis(GetRng());
}

std::mt19937& DecorativeRectangle::GetRng()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

void DecorativeRectangle::UpdateRandomIndicesAndColors(std::vector<int>& randomIndices, std::vector<GLuint>& randomColorTextures, 
                                                       const std::vector<Rectangle>& m_smallRectangles, const std::vector<GLuint>& textures)
{
    std::unordered_set<int> usedIndices;  // To track already used indices
    int previousIndex = -2;  // Initialize with a value that cannot be adjacent

    for (int i = 0; i < 10; ++i)
    {
        int newIndex;
        do
        {
            newIndex = GetRandomIndex(m_smallRectangles);
        } 
        while (usedIndices.count(newIndex) || std::abs(newIndex - previousIndex) == 1);

        randomIndices[i] = newIndex;
        randomColorTextures[i] = GetRandomTexture(textures);
        usedIndices.insert(newIndex);
        previousIndex = newIndex;
    }
}