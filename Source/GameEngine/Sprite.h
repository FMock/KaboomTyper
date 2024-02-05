#pragma once

#include <GL/glew.h>
#include "InputObserver.h"
#include "InputManager.h"
#include <vector>
#include <string>
#include "AABB.h"


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
		~Sprite();
		const char* m_type;
		void Update();
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
		//int x;
		//int y;
		int* m_characterSize;
		float* m_characterPos;
		int change_x;
		int change_y;
	protected:
		float x;
		float y;
	private:
		MoveDirection m_moveDirection;
		std::vector<std::string> m_move;
	};
}