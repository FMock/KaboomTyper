#pragma once

#include <utility> // for std::pair
#include <vector>
#include <memory>
#include <GL/glew.h>
#include "AABB.h"
#include "Moveable.h"

/// <summary>
/// Sprite is an abstract class for other Sprite types to base off of
/// </summary>

namespace GameEngine
{
	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(float width, float height, float x, float y);
		virtual ~Sprite() = default;
		virtual void Initialize(float width, float height, float x, float y);
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;

		// Getters and setters
		void SetPosition(float x, float y);
		void SetSize(float width, float height);
		std::pair<float, float> GetPosition() const;
		std::pair<float, float> GetSize() const;
		AABB& GetBox();

	protected:
		AABB m_box;
		GLuint m_image;
		std::unique_ptr<Moveable> m_moveable;
		std::pair<float, float> m_size; // width, height
		std::pair<float, float> m_position; // x, y position
		MoveDirection m_moveDirection;
	};
}