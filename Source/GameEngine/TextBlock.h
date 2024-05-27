#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>
#include <sstream>
#include <memory>
#include <ctime>
#include "game_data.h"
#include "sprite.h"
#include "TextString.h"
#include "TextStringFont.h"
#include "InputObserver.h"
#include "InputManager.h"
#include "Color.h"

/// <summary>
/// A TextBlock is a composite of colored blocks with a TextString 
/// </summary>

namespace GameEngine
{
	class TextBlock : public Sprite, public InputObserver
	{
	public:
		TextBlock();
		TextBlock(int x, int y, std::string, Colors color = Colors::DEFAULT_COLOR);
		~TextBlock();
		void InitializeTextBlock(float x, float y, std::string str, Colors color = Colors::DEFAULT_COLOR);

		std::unique_ptr<TextString> m_textString; // TextString that's drawn over the TextBlock
		std::string m_colorStr;

		int m_prev_change_x;
		int m_prev_change_y;
		bool m_collided;
		bool m_isHit; // True when hit by any enemy
		bool m_remove; // If True remove TextBlock from game

		std::string to_string() const;
		void Update(float deltaTime) override;
		void Draw() override;
		void Collision(Sprite& sprite);
		void SetMovingState(bool state);
		bool GetMovingState();
		void SetVelocity(float velocity);
		float GetVelocity() const;
		void SetCanMoveHorizontal(bool canMoveHorizontal);
		void Activate();
		bool IsActive() const;
		void SetActiveState(bool isActive);

	private:
		std::unique_ptr<Color> m_color; // TextBlock colored body
		GLuint m_texture;
		float m_scaleFactor;
		int m_adjustedTextblockWidth;
		float m_fontHeight;
		bool m_isMoving;
		float m_velocity;
		float m_angle;
		bool m_canMoveHorizontal;
		void Initialize(float x, float y, std::string str, Colors color);
		int ScaleTextBlockWidth(int textSize, int blockWidth);
		float GetNormalizedSize(float textSize);
		static TextBlock* s_activeTextBlock;
		bool m_active;

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}
