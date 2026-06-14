#pragma once

#include "RectangleDrawable.h"
#include "IDrawable.h"
#include "TextString.h"
#include "Color.h"
#include <string>
#include <vector>
#include <memory>
#include <utility> // Required for std::forward

namespace GameEngine 
{
	class MessageBox : public IDrawable
	{
	public:
		MessageBox();
		MessageBox(int x, int y, int width, int height, Colors color = Colors::DEFAULT_COLOR);
		void Initialize(int x, int y, int width, int height, Colors color = Colors::DEFAULT_COLOR);
		void Update(float dt) override;
		void Draw() override;
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }

		// Sets the lines shown in the box. Accepts any number of strings; each
		// non-empty one becomes a line, stacked top-to-bottom. Up to four lines fit
		// the box at LINE_SCALE. Clears any banner (see SetBanner).
		template<typename... Args>
		void ChangeMessage(Args&&... args);

		// Large stand-alone message drawn in the empty space on the right side of the
		// box (e.g. "GAME OVER"). Pass "" to clear it. ChangeMessage also clears it.
		void SetBanner(const std::string& text);

	private:
		// Line layout (relative to the box origin). Sized so four lines fit between the
		// decorative boxes on the top and bottom borders of the message area.
		static constexpr int LINE_X_PADDING = 20;
		static constexpr int LINE_Y_START = 21;   // top of the first line (clears the top border boxes)
		static constexpr int LINE_Y_SPACING = 18; // vertical step between lines
		static constexpr float LINE_SCALE = 0.50f;

		// Banner layout (relative to the box origin): right of the text column.
		static constexpr int BANNER_X = 305;
		static constexpr int BANNER_Y = 36;
		static constexpr float BANNER_SCALE = 0.90f;

		// Banner flash: the banner toggles visibility every BANNER_FLASH_INTERVAL seconds
		// so messages like "GAME OVER" blink slowly (driven by Update()).
		static constexpr float BANNER_FLASH_INTERVAL = 0.6f;

		RectangleDrawable m_body;
		std::unique_ptr<TextString> m_defaultMessage;      // shown when no lines are set
		std::vector<std::unique_ptr<TextString>> m_lines;  // current message, one entry per line
		std::unique_ptr<TextString> m_banner;              // optional large right-side message
		float m_bannerFlashElapsed = 0.0f;                 // time accumulator for the banner flash
		bool m_bannerVisible = true;                       // current banner blink state
		int m_x, m_y, m_width, m_height;
		int m_priority = 1; // draw priority
		bool m_initialized;
	};

	template<typename... Args>
	void MessageBox::ChangeMessage(Args&&... args)
	{
		std::vector<std::string> messages = { std::forward<Args>(args)... };

		m_banner.reset(); // a new message replaces any banner; re-set it afterwards if needed
		m_lines.clear();
		int line = 0;
		for (const std::string& message : messages)
		{
			if (message.empty())
				continue; // skip blank slots so the remaining lines pack upward

			int y = m_y + LINE_Y_START + line * LINE_Y_SPACING;
			m_lines.push_back(std::make_unique<TextString>(message.c_str(), m_x + LINE_X_PADDING, y));
			++line;
		}
	}
}
