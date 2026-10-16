#include "Input.hpp"

Input::Input()
{
    m_keyboardState = nullptr;
    m_isFocused = true;

    m_keyboardState = SDL_GetKeyboardState(nullptr);
    m_mouseState = SDL_GetMouseState(nullptr, nullptr);
}

void Input::ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                m_shouldClose = true;
                break;

            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                m_isFocused = true;
                break;

            case SDL_EVENT_WINDOW_FOCUS_LOST:
                m_isFocused = false;
                m_mouseDeltaX = 0;
                m_mouseDeltaY = 0;
                break;

            default:
                break;
        }
    }

    int numKeys = 0;
    m_keyboardState = SDL_GetKeyboardState(&numKeys);

    float mouseX = 0.0f;
    float mouseY = 0.0f;

    m_mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    float deltaX = 0.0f;
    float deltaY = 0.0f;

    SDL_GetRelativeMouseState(&deltaX, &deltaY);

    m_mouseDeltaX = static_cast<int>(deltaX);
    m_mouseDeltaY = static_cast<int>(deltaY);
}

bool Input::IsKeyPressed(SDL_Scancode key) const
{
    int numKeys = 0;
    const bool* keyboardState = SDL_GetKeyboardState(&numKeys);

    if (keyboardState == nullptr || key < 0 || key >= numKeys)
    {
        return false;
    }

    return keyboardState[key];
}

bool Input::IsMouseButtonPressed(Uint8 button) const
{
    const SDL_MouseButtonFlags mouseState = SDL_GetMouseState(nullptr, nullptr);

    return (mouseState & SDL_BUTTON_MASK(button)) != 0;
}

SDL_Point Input::GetMouseDelta()
{
    return SDL_Point{ m_mouseDeltaX, m_mouseDeltaY };
}

bool Input::ShouldClose() const
{
    return m_shouldClose;
}