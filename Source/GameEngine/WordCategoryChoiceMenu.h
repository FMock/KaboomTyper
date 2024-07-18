#pragma once
#include "ChoiceMenu.h"
#include <vector>
#include <string>

namespace GameEngine
{
	class WordCategoryChoiceMenu : public ChoiceMenu
	{
	public:
		WordCategoryChoiceMenu(const std::vector<std::string>& wordCategories);

	protected:
		void InitializeMenuEntries() override;
		void InitializeMenuEntry(ChoiceMenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) override;

	private:
		std::vector<std::string> m_wordCategories;
	};
}

