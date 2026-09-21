#include "vulkan/vulkan.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include "VulkanContext.hpp"
#include "Swapchain.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

int main()
{
    vk::detail::DynamicLoader dl;

    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Engine", 800, 800, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    VulkanContext context;
    context.Create(window);

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

        context.MakeDraw(window);
    }

    context.Destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

