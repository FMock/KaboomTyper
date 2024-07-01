#pragma once

#include "InputManager.h"
#include <functional>
#include <string>

namespace GameEngine
{
	class KeyPressHandler
	{
	public:
		using EnterKeyCallback = std::function<void()>;
		using BackSpaceCallback = std::function<void()>;
		using KeyPressCallback = std::function<void(std::string)>;
		void HandleKeyPresses(InputManager* InputMgr, KeyPressCallback keyPressCallback, EnterKeyCallback enterKeyCallback, BackSpaceCallback backSpaceCallback = nullptr);
	};
}
