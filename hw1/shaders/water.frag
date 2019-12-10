#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 cameraVec;
in vec3 lightVec;

out vec4 FragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalTexture;

uniform float time;

void main() {

	vec2 norm = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 refractTexCoords = vec2(norm.x, norm.y);
	vec2 reflectTexCoords = vec2(norm.x, -norm.y);

	vec2 dist = texture(dudvTexture, vec2(textureCoords.x + time, textureCoords.y)).rg * 0.1;

	dist = textureCoords + vec2(dist.x, dist.y + time);
	vec2 dist2 = texture(dudvTexture, dist).rg * 2.0 - 1.0;

	refractTexCoords += (dist2) * 0.02;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	reflectTexCoords += (dist2) * 0.02;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords );
	vec4 refractColor = texture(refractionTexture, refractTexCoords );

	vec3 viewVec = normalize(cameraVec);
	float refractFactor = dot(viewVec, vec3(0.0, 1.0, 0.0));

	vec4 normalColor = texture(normalTexture, dist);
	vec3 normal = vec3(normalColor.r * 2.0 - 1.0, normalColor.b, normalColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 reflected = reflect(normalize(lightVec), normal);
	float spec = max(dot(reflected, viewVec), 0.0);
	spec = pow(spec, 100);
	vec3 spec2 = vec3(255.0, 255.0, 255.0) * spec * 0.6;

    //refractFactor = pow(refractFactor, 10.0);
	FragColor = mix(reflectColor, refractColor, refractFactor);
	FragColor = mix(FragColor, vec4(0.0, 0.3, 0.5, 0.1), 0.2) + vec4(spec2, 0.0);
}
