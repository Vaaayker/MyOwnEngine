#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL_main.h>

class Input 
{
public:
    const bool* m_keyboardState{};
    Uint32 m_mouseState{};

    int m_mouseDeltaX{};
    int m_mouseDeltaY{};

    bool m_shouldClose{};
    bool m_isFocused{};

    Input();


private:
    void ProcessEvents();
    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsMouseButtonPressed(Uint8 button) const;
    SDL_Point GetMouseDelta();
    bool ShouldClose() const;
};