#pragma once

#include <GL/glew.h>
#include <memory>
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
        void DrawText();
        void Update(float dt);

        // Static member for TextStringFont shared across all instances
        static TextStringFont s_font;
        static bool s_fontInitialized;

    private:
        FontParameters m_fontParameters; // used to initialize s_font
        std::string m_string;
        int m_textSize;
        float m_s1, m_s2, m_t1, m_t2; // coords that specify area of image to draw
        int m_x, m_y; // (x, y) = where to start drawing on screen
        std::unique_ptr<Moveable> m_moveable;
    };
}