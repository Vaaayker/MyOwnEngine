#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void funcinit()
{
   SDL_Init(SDL_INIT_VIDEO);

   SDL_Window* window = SDL_CreateWindow("Engine", 800, 1200, 0);
   SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

   SDL_Event event;

   if(event.type == SDL_EVENT_QUIT)
   {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
   }
}

int main()
{
    funcinit();
    return 0;
}