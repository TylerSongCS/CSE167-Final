#ifndef _CLOUD_H_
#define _CLOUD_H_

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
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <time.h>

#include "Object.h"

class Cloud : public Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normal_vertices;
	std::vector<glm::ivec3> indices;

	GLuint vao, vbos[3];

	// Cloud texture maps
	float map32[32 * 32];
	float map256[256 * 256];

	// Timer for animation for the clouds
	float timeAnimation;

public:
	Cloud(std::string objFilename);
	~Cloud();

	void draw();
	void draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 projection, float speed);
	void update();

	void setNoise(float* map);
	void expFilter(float* map);
	void overlapOctaves(float* map32, float* map256);
	float interpolate(float x, float y, float* map);
	float noise(int x, int y, int random);
    void changeRandom();
};

#endif

