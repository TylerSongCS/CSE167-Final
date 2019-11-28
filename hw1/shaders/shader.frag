#version 330 core
// This is a sample fragment shader.
  

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 interpColor;
in vec3 normal;
in vec3 fragPos;


uniform vec3 light_position_world;
uniform vec3 lightColor;
uniform vec3 viewPos;

/*uniform vec3 material_amb;
uniform vec3 material_diff;
uniform vec3 material_spec;
uniform float material_shin;*/
uniform struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int isPhong;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    vec3 result;
    vec3 lookPos = vec3(0,0,0); //assume the look position is at origin for now
    float distanceToLight = sqrt(pow(lookPos.x - light_position_world.x, 2) + pow(lookPos.y - light_position_world.y, 2) + pow(lookPos.z - light_position_world.z, 2));
    float attenuationFactor = 1.0f/200.0f;
    float attenuation = 1.0 / (1.0 + attenuationFactor * pow(distanceToLight, 2));
    
    if(isPhong == 1){
        //ambient lighting code begin
        
        vec3 ambient = ambientStrength * lightColor * material.ambient;
        //ambient light code end
        
        //diffuse lighting code begin
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(light_position_world - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffuseStrength * lightColor * (diff * material.diffuse);
        //diffuse lighting code end
        
        //specular lighting code begin
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lightColor * (specularStrength * spec * material.specular);
        //specular lighting code end
        
        result = ambient + attenuation * (diffuse + specular);
        
    }else{
        vec3 norm = normalize(normal);
        vec3 tempColor = vec3((norm.x + 1)/2 , (norm.y + 1)/2, (norm.z + 1)/2);
        result = interpColor;
        result = tempColor;
    }
	fragColor = vec4(result, 1.0);
    
}
