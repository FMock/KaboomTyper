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
#include "Cursor.h"
#include "KeypressHandler.h"

namespace GameEngine
{
	class InputTextBox : public InputObserver, public IDrawable
	{
	public:
		InputTextBox();
		InputTextBox(int x, int y, int width, int height, Colors rectColor = Colors::DEFAULT_COLOR, bool fillWithColor = false);
		void InitializeTextBox(int x, int y, int width, int height, Colors rectColor = Colors::DEFAULT_COLOR, bool fillWithColor = false);
		void Update(float dt) override;
		void Draw() override;
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }
		void AddText(std::string text);
		void RemoveLast();
		void RemoveAll();
		void MoveCursorForward();
		void MoveCursorBack();
		using Callback = std::function<void()>;
		void AddCallback(Callback callback);
		std::string GetTextBoxContentsAsString();
		void RespondToObserved(InputManager* InputMgr) override;
		void SetCursorXPosition(int x);
		void SetCursorYPosition(int y);
		void ClearInputText();
		int GetCursorStartingXPosition();
		int GetCursorStartingYPosition();
		void SetIsActive(bool isActive);
		bool GetIsActive() const;

	private:
		int m_cursorXPos;
		int m_cursorYPos;
		int m_startCursorXPos;
		int m_startCursorYPos;
		int m_fontWidth;
		void Initialize();
		std::unique_ptr<RectangleDrawable> m_textBox;
		std::unique_ptr<RectangleDrawable> m_cursor;
		std::vector<std::unique_ptr<TextString>> m_inputText;
		bool m_initialized;
		bool m_full;
		int m_maxCharacters;
		Callback m_checkforMatchCallback; // callback called when user presses the Enter Key to submit input text
		void CheckForMatch(); // calls callback funtion
		bool m_isActive;
		int m_priority; // draw priority
		std::shared_ptr<KeyPressHandler> m_keyPressHandler;
		KeyPressHandler::KeyPressCallback m_keyPressCallback;
		KeyPressHandler::EnterKeyCallback m_enterCallback;
		KeyPressHandler::BackSpaceCallback m_backSpaceCallback;
		void OnEnter();
		void OnBackspace();
		void OnKeyPress(std::string);
	};
}