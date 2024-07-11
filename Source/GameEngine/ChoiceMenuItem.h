#pragma once

#include <iostream>
#include "TextString.h"
#include "Button.h"
#include "Color.h"
#include <memory>
#include <string>

#define DEBUG_ChoiceMenuItem 0

namespace GameEngine 
{
	class ChoiceMenuItem
	{
	public:
		ChoiceMenuItem();
		~ChoiceMenuItem();
		ChoiceMenuItem(std::string text, int x, int y, float labelScaler = 1.0f);
		void Initialize(const std::string& text, int x, int y, float labelScaler = 1.0f);
		void Update(float dt);
		void Draw();
		bool MouseHoverPresent(const int& x, const int& y);
		int GetXPosition() const;
		void SetXPosition(int x);
		int GetYPostion() const;
		void SetYPosition(int y);
		int GetWidth() const;
		int GetHeight() const;
		bool GetIsActive() const;
		void SetIsActive(bool isActive);
		bool GetIsSelected();
		void SetIsSelected(bool isSelected);
		std::string GetName() const;
		void SetName(std::string name);
		void AugmentLabel(int c);
		float GetLableScaler() const;
		void SetAugmentedSize(int s);

	private:
		std::unique_ptr<TextString> m_label;
		std::unique_ptr<TextString> m_augmentedLabel;
		std::unique_ptr<Button> m_clickBox;
		float m_labelScaler;
		int m_augmentSize;
		int m_width;
		int m_height;
		int m_xPos;
		int m_yPos;
		int m_clickBoxX;
		int m_clickBoxY;
		int m_clickBoxWidth;
		bool m_isSelected;
		bool m_isInitialized;
		bool m_active; // true when mouse hover present
		std::string m_selectX = " ";
		std::string m_name;
	};
}
