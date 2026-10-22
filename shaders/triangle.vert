#version 450

// mat4 on CPU is stored as mat[row][column], so matrices are row_major here
layout(set = 0, binding = 0, row_major) uniform UniformBufferObject
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 fragmentColor;

void main()
{
    gl_Position = ubo.Projection * ubo.View * ubo.Model * vec4(inPosition, 1.0);
    fragmentColor = inColor;
}
