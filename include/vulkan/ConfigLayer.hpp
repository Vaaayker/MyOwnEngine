#pragma once
#include <vector>
using namespace std;

#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

inline const vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};