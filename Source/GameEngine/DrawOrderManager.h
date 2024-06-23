/*
* DrawOrderManager draws registered IDrawable objects in order of their priority
*/
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
        // Registers IDrawable objects to draw container in order of their priority
        void AddDrawable(std::shared_ptr<IDrawable> drawable);

        // Draws IDrawable registered objects in order of registered priority
        void RenderAll();

    private:
        std::vector<std::shared_ptr<IDrawable>> m_drawables;
    };
}
