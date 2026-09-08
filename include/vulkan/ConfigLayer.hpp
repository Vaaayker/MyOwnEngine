#pragma once
#include <vector>

// Enables validation layers in debug builds
#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

// Validation layers requested by the engine
inline const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};