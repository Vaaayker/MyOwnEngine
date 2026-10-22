#pragma once

#include "mat4.hpp"

struct UniformBufferObject
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
};