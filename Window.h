#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "OBJ.hpp"

typedef struct point_transelations point_transelations_t;
struct point_transelations {
	glm::mat4 point1;
};

typedef struct point_holders point_holders_t; 
struct point_holders {
	std::vector<glm::vec3> curve_points;
	std::vector<glm::vec3> control_points;
};

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static Cube * cube;
	static Cube * box;

    static OBJ * sphere;

	static OBJ * adj_sphere1;
	static OBJ * adj_sphere2;
	static OBJ * adj_sphere3;
	static OBJ * adj_sphere4;
	static OBJ * adj_sphere5;
	static OBJ * adj_sphere6;
	static OBJ * adj_sphere7;
	static OBJ * adj_sphere8;

	static OBJ * control_sphere11;
	static OBJ * control_sphere12;
	static OBJ * control_sphere21;
	static OBJ * control_sphere22;
	static OBJ * control_sphere31;
	static OBJ * control_sphere32;
	static OBJ * control_sphere41;
	static OBJ * control_sphere42;
	static OBJ * control_sphere51;
	static OBJ * control_sphere52;
	static OBJ * control_sphere61;
	static OBJ * control_sphere62;
	static OBJ * control_sphere71;
	static OBJ * control_sphere72;
	static OBJ * control_sphere81;
	static OBJ * control_sphere82;
	
	static OBJ * control_sphere91;
	static OBJ * control_sphere92;
	static OBJ * control_sphere101;
	static OBJ * control_sphere102;

	static OBJ * control_sphere111;
	static OBJ * control_sphere112;
	static OBJ * control_sphere121;
	static OBJ * control_sphere122;

	static OBJ * control_sphere131;
	static OBJ * control_sphere132;
	static OBJ * control_sphere141;
	static OBJ * control_sphere142;

	static OBJ * control_sphere151;
	static OBJ * control_sphere152;
	static OBJ * control_sphere161;
	static OBJ * control_sphere162;

	static PointCloud * cubePoints;
	static Object * currentObj;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc;
	static GLuint skybox_shader;
	static GLuint curve_shader;
	static GLuint env_shader;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);

    static unsigned int load_cubemap(std::vector<std::string> faces);
	static glm::vec3 trackBallMapping(double x, double y);

	static void draw_skybox();

	static point_holders_t generate_bezier();
	static void change_bezier(char *dir);
	static glm::vec3 generate_point(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);

	static void move_a(int dir);
};

#endif
