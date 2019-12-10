#ifndef _WATER_H_
#define _WATER_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "Object.h"

class Water : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];

	// DUDV Distortion
	GLuint dudv_map;
	float animation_offset;

	// Normal Map
	GLuint normal_map;

	// Frame Buffers
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint refractionFrameBuffer;
	GLuint refractionTexture;

	void bindFrameBuffer(int frameBuffer, int width, int height);
	GLuint createFrameBuffer();
	GLuint createTextureAttachment(int width, int height);
public:
	Water(std::string dudv, std::string normal, int h);
	~Water();

	float height;

	void draw();
	void draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 projection, glm::vec3 eye);
	void update();

	void unbindFrameBuffer(int width, int height);
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
};

#endif