#pragma once
#include "IDrawable.h"
#include "InputObserver.h"
#include "RectangleDrawable.h"
#include <memory>

namespace GameEngine
{
	class Instructions : public InputObserver, public IDrawable
	{
    public:
        Instructions();
        virtual ~Instructions() = default;
        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
        bool GetIsActive() const;
        void SetIsActive(bool);

    private:
        void RespondToObserved(InputManager* InputMgr) override;
        std::unique_ptr<RectangleDrawable> m_menuBody;
        bool m_isActive;
        int m_priority; // draw priority
        const int X_POSITION = 300;
        const int Y_POSITION = 40;
        const int WIDTH = 400;
        const int HEIGHT = 600;
	};
}