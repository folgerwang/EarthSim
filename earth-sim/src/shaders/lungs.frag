#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "global_definition.glsl.h"

layout(set = PBR_MATERIAL_PARAMS_SET, binding = DIFFUSE_TEX_INDEX) uniform sampler2D diffuse_tex;
layout(set = PBR_MATERIAL_PARAMS_SET, binding = NORMAL_TEX_INDEX) uniform sampler2D normal_tex;
layout(set = PBR_MATERIAL_PARAMS_SET, binding = GLOSSNESS_TEX_INDEX) uniform sampler2D glossness_tex;
layout(set = PBR_MATERIAL_PARAMS_SET, binding = SPECULAR_TEX_INDEX) uniform sampler2D specular_tex;

layout(location = 0) in VsPsData {
    vec3 vertex_position;
    vec2 vertex_tex_coord;
    vec3 vertex_normal;
} in_data;

layout(location = 0) out vec4 outColor;

void main() {

    //vec3 color = in_data.vertex_normal * 0.5 + 0.5;
    vec3 color = texture(diffuse_tex, in_data.vertex_tex_coord).rgb;
    outColor = vec4(color, 1.0f);
}