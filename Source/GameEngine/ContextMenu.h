#pragma once

#include <iostream>
#include <map>
#include <string>
#include "InputObserver.h"

namespace GameEngine
{
    class ContextMenu : public InputObserver
    {
    public:
        // friendler alias
        using MenuChoices = std::map<int, std::string>;

        virtual MenuChoices GetMenuChoices() const = 0;

        virtual ~ContextMenu() = default;

    protected:
        bool m_isActive;
        inline void RespondToObserved(InputManager* InputMgr) override {};// Do nothing. Derived classes will decide
    };
}
