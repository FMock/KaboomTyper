#pragma once

#include "RectangleDrawable.h"
#include "IDrawable.h"
#include "TextString.h"
#include "Color.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
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

		template<typename... Args>
		void ChangeMessage(Args&&... args);

	private:
		RectangleDrawable m_body;
		std::unordered_map<std::string, std::unique_ptr<TextString>> m_messageMap;
		std::string m_message_01;
		std::string m_message_02;
		std::string m_message_03;
		int m_x, m_y, m_width, m_height;
		int m_priority = 1; // draw priority
		bool m_initialized;
	};

	template<typename... Args>
	void MessageBox::ChangeMessage(Args&&... args)
	{
		std::vector<std::string> messages = { std::forward<Args>(args)... };

		m_message_01 = messages.size() > 0 ? messages[0] : "";
		m_message_02 = messages.size() > 1 ? messages[1] : "";
		m_message_03 = messages.size() > 2 ? messages[2] : "";

		if (!m_message_01.empty())
		{
			m_messageMap[m_message_01] = std::make_unique<TextString>(m_message_01.c_str(), m_x + 20, m_y + 16); // TODO: GET RID OF MAGIC NUMBERS
		}
		else
		{
			m_messageMap.erase(m_message_01);
		}

		if (!m_message_02.empty())
		{
			m_messageMap[m_message_02] = std::make_unique<TextString>(m_message_02.c_str(), m_x + 20, m_y + 40); // TODO: GET RID OF MAGIC NUMBERS
		}
		else
		{
			m_messageMap.erase(m_message_02);
		}

		if (!m_message_03.empty())
		{
			m_messageMap[m_message_03] = std::make_unique<TextString>(m_message_03.c_str(), m_x + 20, m_y + 66); // TODO: GET RID OF MAGIC NUMBERS
		}
		else
		{
			m_messageMap.erase(m_message_03);
		}
	}
}
