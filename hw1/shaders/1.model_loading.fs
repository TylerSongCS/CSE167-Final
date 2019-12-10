#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 light_position_world;
uniform float shininess;
uniform float alpha;
uniform sampler2D texture_diffuse1;
uniform vec3 lookPos;
/*void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}*/

 
 
void main()
{
    //ambient lighting code begin
    vec3 result;
    //vec3 lookPos = vec3(0,0,0); //assume the look position is at origin for now
    float distanceToLight = sqrt(pow(lookPos.x - light_position_world.x, 2) + pow(lookPos.y - light_position_world.y, 2) + pow(lookPos.z - light_position_world.z, 2));
    float attenuationFactor = 1.0f/200.0f;
    float attenuation = 1.0 / (1.0 + attenuationFactor * pow(distanceToLight, 2));
    vec4 surfaceColor = texture(texture_diffuse1, TexCoords);
    vec3 ambient = lightColor * Ambient.rgb;
    ambient = lightColor * surfaceColor.rgb;
    //ambient light code end
    
    //diffuse lighting code begin
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position_world - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * Diffuse.rgb);
    diffuse = lightColor * (diff * surfaceColor.rgb);
    //diffuse lighting code end
    
    //specular lighting code begin
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = lightColor * (spec * Specular.rgb);
    specular = lightColor * (spec * surfaceColor.rgb);
    //specular lighting code end
    
    result = ambient + diffuse + specular;
    FragColor = vec4(result ,alpha);
}
