#pragma once

#include "InputTextBox.h"
#include "InputManager.h"
#include "TextString.h"
#include "RectangleDrawable.h"
#include <memory>
#include <vector>

namespace GameEngine
{
	class IInputMessageBox
	{
	public:
		virtual ~IInputMessageBox() = default;

		virtual void Update(float dt) = 0;
		virtual void Draw() = 0;

	protected:
		std::vector<std::shared_ptr<InputTextBox>> m_inputTextBoxes;
		std::vector<std::shared_ptr<TextString>> m_textStrings;
		std::shared_ptr<InputManager> m_inputManager;
		std::shared_ptr<RectangleDrawable> m_messageBoxBody;
	};
}
