#pragma once

#include "ChoiceMenu.h"

namespace GameEngine
{
	class WordSpeedChoiceMenu : public ChoiceMenu
	{
	public:
		WordSpeedChoiceMenu(const std::vector<std::string>& speedOptions);
		bool GetIsActive(const std::string& keyName);

	protected:
		void InitializeMenuEntries() override;
		void InitializeMenuEntry(ChoiceMenuItem* choiceMenuItem, const std::string& label, int x, int y, float scale, int color) override;

	private:
		std::vector<std::string> m_speedOptions;
	};
}
