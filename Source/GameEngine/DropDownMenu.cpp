#include "DropDownMenu.h"

namespace GameEngine
{
	bool DropDownMenu::GetIsActive() const
	{
		return m_isActive;
	}

	void DropDownMenu::SetIsActive(bool isActive)
	{
		m_isActive = isActive;
	}
}
