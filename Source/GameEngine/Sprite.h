#pragma once

#include <utility> // for std::pair
#include <GL/glew.h>
#include "InputObserver.h"
#include "InputManager.h"
#include <vector>
#include <string>
#include <memory>
#include "AABB.h"
#include "ObjectStates.h"
#include "Common.h"
#include "Moveable.h"


namespace GameEngine
{
	class Sprite : public InputObserver
	{
	public:
		Sprite();
		Sprite(float width, float height, int x, int y);
		virtual ~Sprite();// Virtual destructor for polymorphism.
		void Initialize(float, float, int, int);
		void ProcessInput();
		void setXPos(float);
		void setYPos(float);
		void setWidth(int);
		void setHeight(int);
		int getWidth() const;
		int getHeight() const;
		float getXPos() const;
		float getYPos() const;

		std::pair<int, int> getSize() const;
		std::pair<float, float> getPosition() const;

		inline State getState() const { return m_state; }
		inline void setState(State newState) { m_state = newState; }

	protected:
		AABB m_box;
		AABB& getBox();
		GLuint m_image;
		std::unique_ptr<Moveable> m_moveable;

		std::pair<int, int> m_size; // width, height
		std::pair<float, float> m_position; // x, y position

		MoveDirection m_moveDirection;
		std::vector<std::string> m_move;
		State m_state = State::IDLE; // Default state
		virtual void RespondToObserved(InputManager* InputMgr);
		virtual void Update(float deltaTime);
		virtual void Draw();
	};
}