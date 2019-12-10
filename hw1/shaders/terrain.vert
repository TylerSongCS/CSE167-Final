#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 azBuffer;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec2 TexCoord;
out float zBuffer;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    zBuffer = aPos.z;
    TexCoord = vec2(aPos.x, aPos.y);
}