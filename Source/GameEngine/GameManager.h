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
		// Destroy the falling block matching the typed word and score it. Returns true on a match.
		bool UserTyped(const std::string& word);
		void BlowUpActiveBlock(); // detonate the active block (no scoring) — used by the "kaboom" word
		void PlayMenuSelectSound(); // click sound for menu selections (wired to UIManager)
		void PlayScoreDing(); // bell ding played once per scored point (wired to UIManager)
		void Render();
		bool GetExitGame() const;
		void SetExitGame(bool exitGame);
		bool GetPlayMusic() const;
		void SetPlayMusic(bool playMusic);
		void ChangeTextBlockFallSpeed(const std::string& speed);
		void SetDifficulty(const std::string& difficulty);

	private:
		GameManager();
		void Initialize();
		void UpdateGameEntities(float deltaTime);
		void GameOver();
		void StartGame();
		void RegisterDrawables();
		void PlayExplosionEffects(); // fireworks + boom sound for a destroyed block

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
		Difficulty m_difficulty = Difficulty::Normal; // current game difficulty (default Normal)
		
		// Audio
		std::shared_ptr<GameAudio> m_explosion;
		std::string m_explosionPath;
		static const int BOOM_VOICES = 4; // pool of explosion voices so rapid blasts overlap
		int m_boomVoice = 0;              // round-robin index into the voice pool
		static const int DING_VOICES = 8; // pool of ding voices so rapid score dings overlap
		int m_dingVoice = 0;              // round-robin index into the ding voice pool

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}
