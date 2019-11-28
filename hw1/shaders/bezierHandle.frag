#version 330 core
out vec4 FragColor;


in vec3 interpColor;

void main()
{
    FragColor = vec4(interpColor, 1.0);
}

