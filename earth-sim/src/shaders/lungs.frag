#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "global_definition.glsl.h"

layout(location = 0) out vec4 outColor;

void main() {

    vec3 color = vec3(1, 0, 0);
    outColor = vec4(color, 1.0f);
}