#version 330 core

in vec2 TexCoord;
in float zBuffer;

out vec4 fragColor;

uniform vec3 color;
uniform sampler2D sand_texture;
uniform sampler2D grass_texture;
uniform sampler2D snow_texture;
uniform sampler2D rock_texture;

void main()
{
    float z_value = zBuffer;
    int threshold_grass = 1;
    int threshold_rock = 8;
    int threshold_snow = 11;
    int threshold_max = 100;
    
    if (z_value > threshold_grass && z_value < threshold_rock) {

        fragColor = texture(grass_texture, TexCoord);

    } else if (z_value > threshold_rock && z_value < threshold_snow) {

        fragColor = texture(rock_texture, TexCoord);        

    } else if (z_value > threshold_snow && z_value < threshold_max) {

        fragColor = texture(snow_texture, TexCoord);

    } else {
        fragColor = texture(sand_texture, TexCoord);
    }

    // 
    if (z_value > 0 && z_value < 1)
        fragColor = mix(texture(grass_texture, TexCoord), texture(sand_texture, TexCoord), 0.4);
    if (z_value > 1 && z_value < 2)
        fragColor = mix(texture(grass_texture, TexCoord), texture(sand_texture, TexCoord), 0.2);


    if (z_value > 6 && z_value < 7)
        fragColor = mix(texture(grass_texture, TexCoord), texture(rock_texture, TexCoord), 0.4);
    if (z_value > 7 && z_value < 8)
        fragColor = mix(texture(grass_texture, TexCoord), texture(rock_texture, TexCoord), 0.6);

    if (z_value > 9 && z_value < 10)
        fragColor = mix(texture(rock_texture, TexCoord), texture(snow_texture, TexCoord), 0.3);
    if (z_value > 10 && z_value < 11)
        fragColor = mix(texture(rock_texture, TexCoord), texture(snow_texture, TexCoord), 0.6);
    if (z_value > 11 && z_value < 12)
        fragColor = mix(texture(rock_texture, TexCoord), texture(snow_texture, TexCoord), 0.8);
}