#pragma once
#include "ChoiceMenu.h"

namespace GameEngine
{
	class AudioChoiceMenu : public ChoiceMenu
	{
	public:
		AudioChoiceMenu(const std::vector<std::string>& audioOptions);
		bool GetIsActive(const std::string& keyName);

	protected:
		void InitializeMenuEntries() override;
		void InitializeMenuEntry(ChoiceMenuItem* choiceMenuItem, const std::string& label, int x, int y, float scale, int color) override;

	private:
		std::vector<std::string> m_audioOptions;
	};
}

