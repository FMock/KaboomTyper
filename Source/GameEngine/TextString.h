#pragma once

#include <GL/glew.h>
#include <memory>
#include <unordered_map>
#include "DrawUtils.h"
#include "TextStringFont.h"
#include "FontParameters.h"
#include "Moveable.h"

namespace GameEngine
{
    class TextString
    {
    public:
        TextString();
        TextString(std::string text, int x, int y);
        TextString(const char* text, int x, int y);
        void Initialize(const char* string, int x, int y);
        void Initialize(std::string& string, int x, int y);
        size_t GetTextSize();
        size_t GetFontWidth();
        size_t GetFontHeight();
        void DrawText(float scaleFactor, float angle = 0.0f);
        void Update(float dt);
        std::string GetText() const;
        void SetVelocity(float velocity);
        float GetVelocity() const;
        void SetAngle(float angle);
        float GetAngle() const;
        void SetX(float x);
        void SetY(float y);
        void MoveHorizontal(float n);

        // Static member for TextStringFont shared across all instances
        static TextStringFont s_font;
        static bool s_fontInitialized;

    private:
        static void InitializeFont(FontParameters& fontParameters); // used to initialize s_font
        FontParameters m_fontParameters; // used to initialize s_font
        std::string m_string;
        int m_textSize;
        float m_x, m_y; // (x, y) = where to start drawing on screen
        std::unique_ptr<Moveable> m_moveable;
        float m_velocity;
        float m_angle;

        // Cache for storing precomputed values for each ASCII character
        struct CharMetrics
        {
            float s1, s2, t1, t2;
            short currentCol, currentRow;
        };
        static std::unordered_map<short, CharMetrics> s_charMetricsCache;
    };
}
