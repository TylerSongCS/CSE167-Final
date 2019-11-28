#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>


#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> randomPoints;
    std::vector<glm::vec3> midPoints;
    float weight;
	GLuint vao, vbo, colorbuffer, normalbuffer;
	GLfloat pointSize;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();
    void lerp(float weight);
    void beginSquareAnim();
	void updatePointSize(GLfloat size);
    void updateScale(float size);
    void updateRotation(glm::vec3 rotAxis, float rot_angle);
	void spin(float deg);
};

#endif

