#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

int main()
{
   SDL_Init(SDL_INIT_VIDEO);

   SDL_Window* window = SDL_CreateWindow("Engine", 1200, 800, SDL_WINDOW_VULKAN);
   SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

   bool StillRunning = true;

   while(StillRunning)
   {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                StillRunning = false;  
            }
        }

        SDL_SetRenderDrawColor(renderer, 100, 50, 50, 255); 
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
   }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}