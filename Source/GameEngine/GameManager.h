#pragma once
#include "InputObserver.h"
#include "GameStates.h"
#include "StateMachine.h"
#include "TextStringFont.h"
#include "FontParameters.h"
#include "TextBlockParameters.h"
#include "InputTextBox.h"
#include "InputManager.h"
#include "TextBlockManager.h"
#include "Color.h"
#include "DecorativeRectangleDrawable.h"
#include "GameAudio.h"
#include "UIManager.h"
#include "DrawOrderManager.h"
#include "FireworkExplosionManager.h"
#include "ChoiceMenuItem.h"
#include "WordManager.h"
#include <memory>
#include <vector>
#include <map>

/// <summary>
/// GameManager manages the various entities in the game to create a
/// pleasant and cohesive user experience.
/// </summary>

namespace GameEngine 
{
	class GameManager : public InputObserver, public std::enable_shared_from_this<GameManager>
	{
	public:
		static std::shared_ptr<GameManager> Create();
		void Update(float dt);
		void UserScored();
		void Render();
		bool GetExitGame() const;
		void SetExitGame(bool exitGame);
		bool GetPlayMusic() const;
		void SetPlayMusic(bool playMusic);
		void ChangeTextBlockFallSpeed(const std::string& speed);

	private:
		GameManager();
		void Initialize();
		void UpdateGameEntities(float deltaTime);
		void GameOver();
		void StartGame();
		void RegisterDrawables();

		DrawOrderManager m_drawOrderManager;

		std::shared_ptr<StateMachine> m_stateMachine;
		std::shared_ptr<InputManager> m_inputManager; // shares with TextBlockManager
		std::unique_ptr<UIManager> m_uiManager;
		std::shared_ptr<FireworkExplosionManager> m_fireworkExplosionManager;
		std::shared_ptr<DecorativeRectangle> m_rectangleOfRectangles;
		std::shared_ptr<TextBlockManager> m_textblockManager;
		std::shared_ptr<WordManager> m_wordManager; // shares with TextBlockManager
		bool m_playMusic;
		bool m_exitGame;
		
		// Audio
		std::shared_ptr<GameAudio> m_explosion;
		std::string m_explosionPath;
		static const int BOOM_VOICES = 4; // pool of explosion voices so rapid blasts overlap
		int m_boomVoice = 0;              // round-robin index into the voice pool

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}
