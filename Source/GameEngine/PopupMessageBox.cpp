#include "PopupMessageBox.h"
#include "InputManager.h"
#include "DrawUtils.h"
#include "Common.h"
#include <SDL.h>
#include <algorithm>

using namespace GameEngine;
using namespace DrawUtilities;

PopupMessageBox::PopupMessageBox()
    : m_x(0), m_y(0), m_width(MIN_WIDTH), m_height(TOP_PADDING + BOTTOM_PADDING),
      m_focusedButton(0), m_priority(14), m_isActive(false), m_justOpened(false)
{
    RecomputeLayout();
}

void PopupMessageBox::Update(float dt)
{
}

void PopupMessageBox::Draw()
{
    if (!m_isActive)
        return;

    // Dark-blue background with a red frame to match the red scoreboard.
    RGBColor background = RGBColor::GetRGBColor(RGBColor::DarkBlue);
    glDrawFilledRectangle(m_x, m_y, m_width, m_height, 1.0f, 1.0f, background);
    glDrawThickRectangleOutline(m_x, m_y, m_width, m_height, RGBColor::GetRGBColor(RGBColor::LightGray), Common::FRAME_THICKNESS);

    for (auto& line : m_lines)
        for (auto& segment : line.segments)
            segment.text->DrawText(LINE_SCALE, 0.0f, segment.color);

    for (auto& entry : m_buttons)
        entry.button->Draw();
}

void PopupMessageBox::AddLine(const std::string& text)
{
    AddColoredLine({ { text, RGBColor::GetRGBColor(RGBColor::White) } });
}

void PopupMessageBox::AddColoredLine(const std::vector<std::pair<std::string, RGBColor>>& segments)
{
    Line line;
    int charOffset = 0;
    for (const auto& seg : segments)
    {
        Segment s;
        // Construct with LINE_SCALE so the monospaced advance matches the draw scale.
        s.text = std::make_unique<TextString>(seg.first.c_str(), 0, 0, LINE_SCALE);
        s.color = seg.second;
        s.charOffset = charOffset;
        line.segments.push_back(std::move(s));
        charOffset += static_cast<int>(seg.first.size());
    }
    line.charCount = charOffset;
    m_lines.push_back(std::move(line));
    RecomputeLayout();
}

void PopupMessageBox::AddButton(const std::string& label, Callback callback)
{
    auto button = std::make_shared<Button>(label, 0, 0, BUTTON_SCALE, Colors::BLUE);
    m_buttons.push_back(ButtonEntry{ button, label, callback });
    RecomputeLayout();
}

void PopupMessageBox::RecomputeLayout()
{
    // Width: the wider of the longest text line and the full button row, plus padding,
    // clamped so the box always fits inside the window with a margin.
    int maxLineWidth = 0;
    for (auto& line : m_lines)
        maxLineWidth = std::max(maxLineWidth, static_cast<int>(line.charCount * CHAR_ADVANCE));

    int buttonRowWidth = 0;
    int buttonHeight = 0;
    for (auto& entry : m_buttons)
    {
        buttonRowWidth += entry.button->GetWidth();
        buttonHeight = std::max(buttonHeight, entry.button->GetHeight());
    }
    if (m_buttons.size() > 1)
        buttonRowWidth += BUTTON_GAP * static_cast<int>(m_buttons.size() - 1);

    const int contentWidth = std::max(maxLineWidth, buttonRowWidth);
    const int maxWidth = Common::WINDOW_WIDTH - 2 * SCREEN_MARGIN;
    m_width = std::min(std::max(contentWidth + 2 * INTERIOR_PADDING, MIN_WIDTH), maxWidth);

    // Height: top padding + the stacked lines + (optional) button row + bottom padding.
    const int numLines = static_cast<int>(m_lines.size());
    const int linesHeight = numLines * LINE_Y_SPACING;
    const int buttonBlock = m_buttons.empty() ? 0 : (BUTTON_TOP_GAP + buttonHeight);
    m_height = TOP_PADDING + linesHeight + buttonBlock + BOTTOM_PADDING;

    // Center the box in the window.
    m_x = (Common::WINDOW_WIDTH - m_width) / 2;
    m_y = (Common::WINDOW_HEIGHT - m_height) / 2;

    // Position the text lines, left-aligned with the interior padding. Each segment is
    // offset by its character position (monospaced font).
    for (int i = 0; i < numLines; ++i)
    {
        const int lineY = m_y + TOP_PADDING + i * LINE_Y_SPACING;
        for (auto& segment : m_lines[i].segments)
        {
            segment.text->SetX(static_cast<float>(m_x + INTERIOR_PADDING + static_cast<int>(segment.charOffset * CHAR_ADVANCE)));
            segment.text->SetY(static_cast<float>(lineY));
        }
    }

    // Position the buttons as a centered row beneath the text.
    int bx = m_x + (m_width - buttonRowWidth) / 2;
    const int by = m_y + TOP_PADDING + linesHeight + BUTTON_TOP_GAP;
    for (auto& entry : m_buttons)
    {
        entry.button->Initialize(entry.label, bx, by, BUTTON_SCALE, Colors::BLUE);
        bx += entry.button->GetWidth() + BUTTON_GAP;
    }
}

void PopupMessageBox::HandleButtonClick(InputManager* InputMgr, ButtonEntry& entry)
{
    int mouseX, mouseY;
    InputMgr->GetMousePosition(&mouseX, &mouseY);

    Button* button = entry.button.get();
    bool over = button->IsMouseOverButton(mouseX, mouseY);
    button->SetIsActive(over); // highlight on hover

    // Fire on a full click (press then release) over the button.
    if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && over)
    {
        button->SetButtonColor(Colors::DEFAULT_COLOR);
        if (entry.callback)
            entry.callback();
    }
    else if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && over)
    {
        button->SetButtonColor(Colors::DARK_GRAY); // pressed feedback
    }
}

void PopupMessageBox::HandleKeyboard(InputManager* InputMgr)
{
    const int n = static_cast<int>(m_buttons.size());
    if (n == 0)
        return;

    // Edge-detected key press (down this frame, up last frame).
    auto justPressed = [InputMgr](SDL_Scancode sc)
    {
        return InputMgr->m_kbState[sc] && !InputMgr->m_kbPrevState[sc];
    };

    const bool prev = justPressed(SDL_SCANCODE_LEFT) || justPressed(SDL_SCANCODE_UP);
    const bool next = justPressed(SDL_SCANCODE_RIGHT) || justPressed(SDL_SCANCODE_DOWN);
    if (n > 1)
    {
        if (prev) m_focusedButton = (m_focusedButton - 1 + n) % n;
        if (next) m_focusedButton = (m_focusedButton + 1) % n;
    }

    if (justPressed(SDL_SCANCODE_RETURN) || justPressed(SDL_SCANCODE_KP_ENTER))
    {
        if (m_focusedButton >= 0 && m_focusedButton < n && m_buttons[m_focusedButton].callback)
            m_buttons[m_focusedButton].callback();
    }
}

void PopupMessageBox::RespondToObserved(InputManager* InputMgr)
{
    if (!m_isActive)
        return;

    // Show the keyboard focus as a persistent highlight on the focused button.
    for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
        m_buttons[i].button->SetSelected(i == m_focusedButton);

    // Skip the input that opened us (e.g. the Enter/click on the menu item), so that
    // same press doesn't immediately activate the focused button.
    if (m_justOpened)
    {
        m_justOpened = false;
        return;
    }

    HandleKeyboard(InputMgr);
    if (!m_isActive)
        return; // an Enter activation may have closed this popup

    // Iterate by index: a callback may deactivate/close this popup, after which we stop.
    for (size_t i = 0; i < m_buttons.size() && m_isActive; ++i)
        HandleButtonClick(InputMgr, m_buttons[i]);
}
