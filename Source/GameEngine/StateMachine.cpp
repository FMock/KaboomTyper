#pragma once
#include "StateMachine.h"

using namespace GameEngine;

void StateMachine::Update()
{
    switch (m_currentState)
    {
    case GameState::MAIN_MENU:
        std::cout << "Main Menu...\n";
        break;
    case GameState::GAME_PLAY:
        std::cout << "Playing the Game...\n";
        // Additional code to handle gameplay actions
        break;
    case GameState::PAUSED:
        std::cout << "Game is Paused...\n";
        // Additional code to handle pause actions
        break;
    case GameState::GAME_OVER:
        std::cout << "Game Over!\n";
        // Additional code to handle game over actions
        PromptPlayAgainOrQuit();
        break;
    case GameState::VICTORY:
        std::cout << "Congratulations! You Won!\n";
        // Additional code to handle victory actions
        break;
    case GameState::LOADING:
        std::cout << "Loading a level...\n";
        // Additional code to handle loading actions
        break;
    case GameState::MENU_SETTINGS:
        std::cout << "Displaying Settings Menu...\n";
        // Additional code to handle settings menu actions
        break;
    case GameState::MENU_OPTIONS:
        std::cout << "Displaying Options Menu...\n";
        // Additional code to handle options menu actions
        break;
    case GameState::MENU_INSTRUCTIONS:
        std::cout << "Displaying Instructions Menu...\n";
        // Additional code to handle instructions menu actions
        break;
    case GameState::MENU_CREDITS:
        std::cout << "Displaying Credits Menu...\n";
        // Additional code to handle credits menu actions
        break;
    case GameState::QUIT_CONFIRMATION:
        std::cout << "Quitting the Game...\n";
        // Additional code to handle quit confirmation actions
        break;
    default:
        std::cout << "Invalid state!\n";
        break;
    }
}

void StateMachine::SetCurrentState(GameState newState) 
{ 
    m_previousState = m_currentState;
    m_currentState = newState; 
}

void StateMachine::PromptPlayAgainOrQuit()
{
    std::cout << "Play Again (Y/N)? ";
    char choice;
    std::cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        // Reset the game state to the initial state
        SetCurrentState(GameState::MAIN_MENU);
    }
    else
    {
        SetCurrentState(GameState::QUIT_CONFIRMATION);
    }
}