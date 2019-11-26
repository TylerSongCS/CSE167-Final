#include "Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

int Window::width;
int Window::height;

bool left_mouse_click = false, right_mouse_click = false;
float last_x, last_y;
bool first_trackball = true;

GLuint skybox_VAO, skybox_VBO;
GLuint curve_VAO, curve_VBO;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;


const char* Window::windowTitle = "CSE 167 Final Project Eirik Irvoll";

// skybox
std::vector<std::string> faces = {
	"skybox2/back.jpg",
	"skybox2/front.jpg",
    "skybox2/top_rot.jpg",
	"skybox2/bottom_rot2.jpg",
	"skybox2/right.jpg",
	"skybox2/left.jpg"
};


unsigned int cubemapTexture;

float skyboxVertices[] = {
    // positions
    -500.0f,  500.0f, -500.0f,
    -500.0f, -500.0f, -500.0f,
    500.0f, -500.0f, -500.0f,
    500.0f, -500.0f, -500.0f,
    500.0f,  500.0f, -500.0f,
    -500.0f,  500.0f, -500.0f,
    
    -500.0f, -500.0f,  500.0f,
    -500.0f, -500.0f, -500.0f,
    -500.0f,  500.0f, -500.0f,
    -500.0f,  500.0f, -500.0f,
    -500.0f,  500.0f,  500.0f,
    -500.0f, -500.0f,  500.0f,
    
    500.0f, -500.0f, -500.0f,
    500.0f, -500.0f,  500.0f,
    500.0f,  500.0f,  500.0f,
    500.0f,  500.0f,  500.0f,
    500.0f,  500.0f, -500.0f,
    500.0f, -500.0f, -500.0f,
    
    -500.0f, -500.0f,  500.0f,
    -500.0f,  500.0f,  500.0f,
    500.0f,  500.0f,  500.0f,
    500.0f,  500.0f,  500.0f,
    500.0f, -500.0f,  500.0f,
    -500.0f, -500.0f,  500.0f,
    
    -500.0f,  500.0f, -500.0f,
    500.0f,  500.0f, -500.0f,
    500.0f,  500.0f,  500.0f,
    500.0f,  500.0f,  500.0f,
    -500.0f,  500.0f,  500.0f,
    -500.0f,  500.0f, -500.0f,
    
    -500.0f, -500.0f, -500.0f,
    -500.0f, -500.0f,  500.0f,
    500.0f, -500.0f, -500.0f,
    500.0f, -500.0f, -500.0f,
    -500.0f, -500.0f,  500.0f,
    500.0f, -500.0f,  500.0f
};

// Objects to display.
Cube * Window::cube;
Cube * Window::box;
PointCloud * Window::cubePoints;

OBJ * Window::sphere;

// The object currently displaying.
Object * Window::currentObj;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 50); // Camera position.
glm::vec3 Window::center(-50.0f, -30.0f, -100.0f); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);
glm::vec3 last_camera_position = glm::vec3(Window::eye.x, Window::eye.y, Window::eye.z);

GLuint Window::program; // The shader program id.
GLuint Window::skybox_shader;
GLuint Window::curve_shader;
GLuint Window::env_shader;

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.

bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skybox_shader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	curve_shader = LoadShaders("shaders/curve.vert", "shaders/curve.frag");
	env_shader = LoadShaders("shaders/env.vert", "shaders/env.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

	// skybox
	cubemapTexture = Window::load_cubemap(faces);
	sphere->set_cubemapTexture(cubemapTexture);

	glGenVertexArrays(1, &skybox_VAO);
	glGenBuffers(1, &skybox_VBO);

	glBindVertexArray(skybox_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	return true;
}

bool Window::initializeObjects()
{
    sphere = new OBJ(2.0f, "sphere.obj");
	
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete sphere;

	// Delete the shader program.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}
 
void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0),   
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
    projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1500.0f);
    view = glm::lookAt(Window::eye, Window::eye + Window::center, Window::up);
    
	sphere->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(env_shader);
	GLint curveProjection = glGetUniformLocation(env_shader, "projection");
	GLint curveModelview = glGetUniformLocation(env_shader, "view");
	GLint model_loc = glGetUniformLocation(env_shader, "model");
	GLint cam_loc = glGetUniformLocation(env_shader, "cameraPos");

	glm::mat4 model = sphere->getModel();
	glm::vec3 color = sphere->getColor();
	glUniformMatrix4fv(curveProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(curveModelview, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glUniform3fv(cam_loc, 1, glm::value_ptr(eye));
	
	sphere->draw();

	draw_skybox();
    
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::draw_skybox() {

	glDepthMask(GL_FALSE);
    
    glUseProgram(skybox_shader);
    
    GLint projectionLoc = glGetUniformLocation(skybox_shader, "projection");
    GLint viewLoc  = glGetUniformLocation(skybox_shader,  "view");
    GLint skybox = glGetUniformLocation(skybox_shader,  "skybox");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniform1i(skybox, 0);

    glBindVertexArray(skybox_VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
        }
	}
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        left_mouse_click = true;
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		left_mouse_click = false;
}

void Window::mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {

	if (left_mouse_click) {

		if (first_trackball) {
			last_x = (float)xpos;
			last_y = (float)ypos;
			first_trackball = false;
		}

		float x_offset = xpos - last_x;
		float y_offset = last_y - ypos;

		float sensitivity = 0.2f;
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		yaw += x_offset;
		pitch += y_offset;

        glm::vec3 new_cam = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        );

        Window::center = glm::normalize(new_cam);
	}

	last_x = xpos;
	last_y = ypos;
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	fov += yoffset;
}

glm::vec3 Window::trackBallMapping(double x, double y) {

	glm::vec3 v; // 3D position of mouse on the trackball
	float d; // depth of mouse location

	v.x = (2.0f * x - Window::width) / Window::width; // mouse X pos
	v.y = (Window::height - 2.0f * y) / Window::height; // mouse Y pos
	v.z = 0.0f;

	d = glm::length(v);
	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrtf(1.001f - d * d);

	v = glm::normalize(v);

	return v;
}

unsigned int Window::load_cubemap(std::vector<std::string> faces) {
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    std::cout << "Loading cubemap\n";
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::cout << " " << faces[i].c_str() << "\n";
        
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}
