#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "IDrawable.h"

namespace GameEngine
{
    class DrawOrderManager
    {
    public:
        void AddDrawable(std::shared_ptr<IDrawable> drawable);
        void RenderAll();

    private:
        std::vector<std::shared_ptr<IDrawable>> m_drawables;
    };
}
