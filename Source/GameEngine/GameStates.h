#pragma once

/*
    SplashScreen: Initial screen that appears breifly, displays game title, art, and more. 
    MainMenu: The screen where players can start or exit the game.
    Gameplay: The state where the game is actively being played.
    Paused: The game has been temporarily stopped by the player or game logic.
    GameOver: The state indicating the player has lost the game and the game is over.
    Victory: The state indicating the player has won the game.
    Loading: The state indicating the game is currently loading a level or data.
    MenuSettings: The state where players can adjust the game's settings.
    MenuOptions: The state where players can customize their game experience.
    MenuInstructions: The state where players can learn how to play the game.
    MenuCredits: The state where players can view the game's credits.
    QuitConfirmation: The state where players are prompted to confirm quitting the game.
*/

enum class GameState
{
    SplashScreen,
    MainMenu,
    Gameplay,
    Paused,
    GameOver,
    Victory,
    Loading,
    MenuSettings,
    MenuOptions,
    MenuInstructions,
    MenuCredits,
    QuitConfirmation,
    NumStates // Keep this as the last enum value for easy counting
};
