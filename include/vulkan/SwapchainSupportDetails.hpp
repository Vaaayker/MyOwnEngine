using namespace std;
#include "vector"

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities; // мін/макс розмір swapchain images, currentExtent, кількість images
    vector<VkSurfaceFormatKHR> formats; // формат кольору, наприклад VK_FORMAT_B8G8R8A8_SRGB
    vector<VkPresentModelKHR> presentModes; // FIFO, MAILBOX, IMMEDIATE
};