#version 330 core

layout (location = 0) in vec3 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 cameraVec;
out vec3 lightVec;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 eye;
uniform vec3 light;


void main() {
	vec4 worldPos = model * vec4(position, 1.0);
	cameraVec = eye - worldPos.xyz;
	gl_Position = projection * view * model * vec4(position, 1.0);
	clipSpace = gl_Position;
	textureCoords = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) / 2.0;
	lightVec = worldPos.xyz - light;
}