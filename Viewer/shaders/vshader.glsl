#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform int projection_type;
uniform float projection_scale;
uniform float projection_offset;
uniform bool apply_texture_mapping_status;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 origFragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
    origFragPos = pos;
    vec4 transform = model * vec4(pos, 1.0f);
    fragPos = vec3(transform) / transform.w;
    fragNormal = mat3(view * model) * normal;

    if(apply_texture_mapping_status)
    {
       fragTexCoords.x =  acos(origFragPos.y / sqrt(origFragPos.x * origFragPos.x + origFragPos.y * origFragPos.y + origFragPos.z * origFragPos.z));
       fragTexCoords.y =  acos(origFragPos.x / sqrt(origFragPos.x * origFragPos.x + origFragPos.z * origFragPos.z));
    }
    else
    {
       fragTexCoords = texCoords;
    }
 
    gl_Position = projection * view * model * vec4(pos, 1.0f);
}