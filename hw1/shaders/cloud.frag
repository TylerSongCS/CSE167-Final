#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec3 TexCoord;

uniform sampler2D ourTexture;
uniform float Time;

void main()
{
    vec4 texColor = texture(ourTexture, vec2(TexCoord.x * 1.5 + Time, TexCoord.y * 1.5));
    if (TexCoord.z > 0)
        discard;
    FragColor = texColor;
    FragColor.a = FragColor.a * (1 - (TexCoord.z + 1));
}