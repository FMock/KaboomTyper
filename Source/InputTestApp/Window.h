// Window.h
#pragma once
#include <SDL.h>
#include <string>

class Window
{
public:
    Window()
    {
        SDL_Init(SDL_INIT_VIDEO);

        SDLWindow = SDL_CreateWindow(
            "Hello World",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowWidth, windowHeight, 0
        );

        SDLWindowSurface = SDL_GetWindowSurface(SDLWindow);
    }

    void Update()
    {
        SDL_FillRect(
            SDLWindowSurface,
            nullptr,
            SDL_MapRGB(
                SDLWindowSurface->format,
                bgRed, bgGreen, bgBlue
            )
        );
    }

    void RenderFrame()
    {
        SDL_UpdateWindowSurface(SDLWindow);
    }

    void SetBackgroundColor(int R, int G, int B)
    {
        bgRed = R;
        bgGreen = G;
        bgBlue = B;
    }

    void SetTitle(std::string NewTitle)
    {
        // https://wiki.libsdl.org/SDL_SetWindowTitle
        SDL_SetWindowTitle(
            SDLWindow, NewTitle.c_str()
        );
    }

    void ChangeWindowSize(int Amount)
    {
        // https://wiki.libsdl.org/SDL_SetWindowSize
        SDL_SetWindowSize(
            SDLWindow,
            windowWidth += Amount,
            windowHeight += Amount
        );
    }

    [[nodiscard]]
    int GetWindowWidth() const
    {
        return windowWidth;
    }

    [[nodiscard]]
    int GetWindowHeight() const
    {
        return windowHeight;
    }

    void MoveRelative(int x, int y)
    {
        // https://wiki.libsdl.org/SDL_GetWindowPosition
        int CurrentX; int CurrentY;
        SDL_GetWindowPosition(SDLWindow, &CurrentX, &CurrentY);

        // https://wiki.libsdl.org/SDL_SetWindowPosition
        SDL_SetWindowPosition(SDLWindow, CurrentX + x, CurrentY + y);
    }

    void GrabMouse()
    {
        // https://wiki.libsdl.org/SDL_SetWindowMouseGrab
        SDL_SetWindowMouseGrab(SDLWindow, SDL_TRUE);
    }

    void FreeMouse()
    {
        // https://wiki.libsdl.org/SDL_SetWindowMouseGrab
        SDL_SetWindowMouseGrab(SDLWindow, SDL_FALSE);
    }

private:
    SDL_Window* SDLWindow{ nullptr };
    SDL_Surface* SDLWindowSurface{ nullptr };
    int windowWidth{ 300 };
    int windowHeight{ 300 };

    int bgRed{ 40 };
    int bgGreen{ 40 };
    int bgBlue{ 40 };
};