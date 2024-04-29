#pragma once
#include <vector>
#include <memory>
#include "TextString.h"
#include "RectangleDrawable.h"
#include "Colors.h"
#include "InputObserver.h"
#include "InputManager.h"

namespace GameEngine
{
	class InputTextBox : public InputObserver
	{
	public:
		InputTextBox();
		~InputTextBox();
		void InitializeTextBox(int x, int y, int width, int height, Colors rectColor = Colors::DEFAULT_COLOR);
		void Update();
		void Draw();
		void AddText(std::string text);
		void RemoveLast();
		void RemoveAll();
		static std::string getTextboxText();

	private:
		int m_cursorXPos;
		int m_cursorYPos;
		int m_startCursorXPos;
		int m_startCursorYPos;
		int m_fontWidth;
		void MoveCursorForward();
		void MoveCursorBack();
		std::string GetTextBoxContentsAsString();
		void Initialize();
		std::unique_ptr<RectangleDrawable> m_textBox;
		std::vector<std::unique_ptr<TextString>> m_inputText;
		bool m_initialized;
		bool m_full;
		int m_maxCharacters;
		static std::string s_textboxText;

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}