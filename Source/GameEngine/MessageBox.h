#pragma once

#include "RectangleDrawable.h"
#include "TextString.h"
#include "Color.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace GameEngine 
{
	class MessageBox
	{
	public:
		MessageBox();
		MessageBox(int x, int y, int width, int height, Colors color = Colors::DEFAULT_COLOR);
		~MessageBox();

		void Initialize(int x, int y, int width, int height, Colors color = Colors::DEFAULT_COLOR);
		void ChangeMessage(std::string message);
		void Draw();

	private:
		RectangleDrawable m_body;
		std::unordered_map<std::string, std::unique_ptr<TextString>> m_messageMap;
		std::string m_currentMessage;
		int m_x, m_y, m_width, m_height;
		bool m_initialized;
	};
}
