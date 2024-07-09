#pragma once
#include "ChoiceMenu.h"

namespace GameEngine
{
	class WordCategoryChoiceMenu : public ChoiceMenu
	{
	public:
		WordCategoryChoiceMenu();

	protected:
		void InitializeMenuEntries() override;
		void InitializeMenuEntry(ChoiceMenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) override;
	};
}

