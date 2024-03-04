#pragma once

#include <GL/glew.h>
#include "InputObserver.h"
#include "InputManager.h"
#include <vector>
#include <string>
#include "AABB.h"
#include "ObjectStates.h"


namespace GameEngine
{
	enum class MoveDirection
	{
		NONE = -1, LEFT, RIGHT, UP, DOWN
	};

	class Sprite : public InputObserver
	{
	public:
		Sprite();
		Sprite(int, int, int, int);
		virtual ~Sprite();// Virtual destructor for polymorphism.
		const char* m_type;

		virtual void Update(float deltaTime) = 0; // must be overridden by derived classes
		void Draw();
		void ProcessInput();
		virtual void RespondToObserved(InputManager* InputMgr);

		AABB box;
		AABB& getBox();
		GLuint m_image;

		void setXPos(int);
		void setYPos(int);
		void setWidth(int);
		void setHeight(int);
		int getWidth() const;
		int getHeight() const;
		int getXPos() const;
		int getYPos() const;

		int* m_characterSize;
		float* m_characterPos;
		int change_x;
		int change_y;
		inline State getState() const { return m_state; }
		inline void setState(State newState) { m_state = newState; }

	protected:
		float x;
		float y;
		MoveDirection m_moveDirection;
		std::vector<std::string> m_move;
		State m_state = State::IDLE; // Default state

	private:

	};
}