#pragma once
#include <vector>
#include <memory>
#include "TextString.h"
#include "RectangleDrawable.h"
#include "Color.h"
#include "InputObserver.h"
#include "InputManager.h"
#include "Common.h"
#include "functional"

namespace GameEngine
{
	class InputTextBox : public InputObserver
	{
	public:
		InputTextBox();
		~InputTextBox();
		void InitializeTextBox(int x, int y, int width, int height, Colors rectColor = Colors::DEFAULT_COLOR, bool fillWithColor = false);
		void Update();
		void Draw();
		void AddText(std::string text);
		void RemoveLast();
		void RemoveAll();
		using Callback = std::function<void()>;
		void AddCallback(Callback callback);

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
		Callback m_checkforMatchCallback; // callback called when user presses the Enter Key to submit input text
		void CheckForMatch(); // calls callback funtion

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}