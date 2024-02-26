#pragma once
#include "StateMachine.h"

using namespace GameEngine;

void StateMachine::Update()
{
    switch (m_currentState)
    {
    case GameState::MainMenu:
        std::cout << "Main Menu...\n";
        break;
    case GameState::Gameplay:
        std::cout << "Playing the Game...\n";
        // Additional code to handle gameplay actions
        break;
    case GameState::Paused:
        std::cout << "Game is Paused...\n";
        // Additional code to handle pause actions
        break;
    case GameState::GameOver:
        std::cout << "Game Over!\n";
        // Additional code to handle game over actions
        PromptPlayAgainOrQuit();
        break;
    case GameState::Victory:
        std::cout << "Congratulations! You Won!\n";
        // Additional code to handle victory actions
        break;
    case GameState::Loading:
        std::cout << "Loading a level...\n";
        // Additional code to handle loading actions
        break;
    case GameState::MenuSettings:
        std::cout << "Displaying Settings Menu...\n";
        // Additional code to handle settings menu actions
        break;
    case GameState::MenuOptions:
        std::cout << "Displaying Options Menu...\n";
        // Additional code to handle options menu actions
        break;
    case GameState::MenuInstructions:
        std::cout << "Displaying Instructions Menu...\n";
        // Additional code to handle instructions menu actions
        break;
    case GameState::MenuCredits:
        std::cout << "Displaying Credits Menu...\n";
        // Additional code to handle credits menu actions
        break;
    case GameState::QuitConfirmation:
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
        SetCurrentState(GameState::MainMenu);
    }
    else
    {
        SetCurrentState(GameState::QuitConfirmation);
    }
}