#pragma once

#include <GL/glew.h>
#include "DrawUtils.h"
#include "TextStringFont.h"
#include "FontParameters.h"

namespace GameEngine
{
    class TextString
    {
    public:
        TextString() = default;

        void Initialize(const char* string, int x, int y);
        void Initialize(std::string& string, int x, int y);

        void DrawText();
        void Update(float dt);
        void MoveRight();
        void MoveLeft();
        void MoveUp();
        void MoveDown();
        void Stop();
        void SetPartOfTextBlock(bool);
        bool GetPartOfTextBlock();

        // Static member for TextStringFont shared across all instances
        static TextStringFont s_font;
        static bool s_fontInitialized;

    private:
        FontParameters m_fontParameters; // used to initialize s_font
        std::string m_string;
        float m_s1, m_s2, m_t1, m_t2; // coords that specify area of image to draw
        int m_x, m_y; // (x, y) = where to start drawing on screen
        float m_changeX, m_changeY;
        float m_speedX, m_speedY;
        //bool m_partOfTextBlock;
    };
}