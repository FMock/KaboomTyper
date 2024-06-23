#include "DrawOrderManager.h"

using namespace GameEngine;

void DrawOrderManager::AddDrawable(std::shared_ptr<IDrawable> drawable)
{
    m_drawables.push_back(drawable);

    std::stable_sort(m_drawables.begin(), m_drawables.end(),
        [](const std::shared_ptr<IDrawable>& a, const std::shared_ptr<IDrawable>& b)
        {
            return a->GetPriority() < b->GetPriority();
        });
}

void DrawOrderManager::RenderAll()
{
    for (const auto& drawable : m_drawables)
    {
        drawable->Draw();
    }
}