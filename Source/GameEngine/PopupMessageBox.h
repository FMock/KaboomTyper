#pragma once

#include "IDrawable.h"
#include "InputObserver.h"
#include "TextString.h"
#include "Button.h"
#include "RGBColor.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <utility>

namespace GameEngine
{
    class InputManager; // forward declaration to minimize dependencies

    // A simple modal popup dialog: a box showing one or more lines of text and one or
    // two buttons. Unlike InputMessageBox it has no text-input field, and it draws
    // whenever it is active (NOT gated on GameState), so it can appear over gameplay
    // (e.g. the exit-confirmation prompt) as well as on the menu/idle screen.
    //
    // Build one with AddLine() / AddButton(); the box auto-sizes to fit its content
    // (clamped to the window) and re-centers itself, so text never overflows its bounds.
    class PopupMessageBox : public InputObserver, public IDrawable
    {
    public:
        using Callback = std::function<void()>;

        PopupMessageBox();

        void Update(float dt) override;
        void Draw() override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }

        // Appends a line of (white) text, stacked top-to-bottom. The box grows to fit it.
        void AddLine(const std::string& text);

        // Appends a line composed of colored segments drawn left-to-right (the font is
        // monospaced, so segments align by character position). Lets individual tokens
        // (e.g. "F1") be colored while the rest of the line stays white.
        void AddColoredLine(const std::vector<std::pair<std::string, RGBColor>>& segments);

        // Appends a button to the centered button row at the bottom (left-to-right in
        // call order). The callback fires on a complete mouse click over the button.
        void AddButton(const std::string& label, Callback callback);

        bool GetIsActive() const { return m_isActive; }

        // Showing the popup resets keyboard focus to the default button (the last one
        // added, e.g. "No"/"Close" — the safe choice the user can confirm with Enter).
        void SetIsActive(bool isActive)
        {
            m_isActive = isActive;
            if (isActive)
            {
                m_focusedButton = m_buttons.empty() ? 0 : static_cast<int>(m_buttons.size()) - 1;
                m_justOpened = true; // ignore the (key/click) press that opened us this frame
            }
        }

    protected:
        void RespondToObserved(InputManager* InputMgr) override;

    private:
        struct ButtonEntry
        {
            std::shared_ptr<Button> button;
            std::string label;
            Callback callback;
        };

        // One colored run of text within a line, positioned at charOffset characters in.
        struct Segment
        {
            std::unique_ptr<TextString> text;
            RGBColor color{ 255, 255, 255 };
            int charOffset = 0;
        };
        struct Line
        {
            std::vector<Segment> segments;
            int charCount = 0; // total characters, for width sizing
        };

        void HandleButtonClick(InputManager* InputMgr, ButtonEntry& entry);
        // Arrow keys move the focused button; Enter activates it.
        void HandleKeyboard(InputManager* InputMgr);
        // Recompute box size/position from the current content and reposition all
        // lines and buttons. Called after every AddLine()/AddButton().
        void RecomputeLayout();

        std::vector<Line> m_lines;
        std::vector<ButtonEntry> m_buttons;
        int m_x, m_y, m_width, m_height;
        int m_focusedButton; // index of the keyboard-focused button
        int m_priority;
        bool m_isActive;
        bool m_justOpened;   // true for the first frame after opening (see SetIsActive)

        // Layout constants.
        static constexpr int INTERIOR_PADDING = 25; // left/right padding around text
        static constexpr int TOP_PADDING = 28;      // top of box to first line
        static constexpr int LINE_Y_SPACING = 34;   // vertical step between lines
        static constexpr float LINE_SCALE = 0.55f;
        static constexpr float CHAR_ADVANCE = 24.0f * LINE_SCALE; // monospaced glyph advance (font width 24)
        static constexpr int BUTTON_TOP_GAP = 20;    // gap between last line and button row
        static constexpr int BOTTOM_PADDING = 20;    // button row to bottom of box
        static constexpr int BUTTON_GAP = 25;        // horizontal gap between buttons
        static constexpr float BUTTON_SCALE = 0.6f;
        static constexpr int MIN_WIDTH = 360;
        static constexpr int SCREEN_MARGIN = 30;     // min gap from the window edges
    };
}
