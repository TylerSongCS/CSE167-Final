#include "Window.h"
#include <list>
#include <cmath>
#include "model.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang.h>
using namespace irrklang;
using namespace std;

ISoundEngine *SoundEngine;
float alpha = 0.0f;
int animationIndex = 0;
vector<bool> animationBools(100);

vec3 gradientColor1 = vec3(0,0,0);

Window::Material Window::material;
float Window::ambientStrength;
float Window::diffuseStrength;
float Window::specularStrength;
int Window::isPhong;
glm::vec3 Window::lightColor = glm::vec3(1, 1, 0);


float Window::interpFactor = 0;

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

//Trackball variables
bool Window::is_button_down = false;
glm::vec3 Window::lastPoint;
glm::vec3 Window::currPoint;

Frustum * frust;

// Cube faces
vector<string> faces{
    "resources/textures/skybox/right.jpg",
    "resources/textures/skybox/left.jpg",
    "resources/textures/skybox/top.jpg",
    "resources/textures/skybox/bottom.jpg",
    "resources/textures/skybox/front.jpg",
    "resources/textures/skybox/back.jpg"
};
unsigned int cubemapTexture;
Skybox* skybox;

//Transforms
Transform* world;
Transform* carTransform;
vector<Transform*> ctrlPointTransforms;

//Geometries
Geometry* car;
vector<Geometry*> ctrlPointGeometry;

vec3 carTransformFactor = vec3(0.4, 0.4, 0.4);

// Object file names
string sphereFileName = "sphere.obj";

//Bezier Curves
BezierCurve* curve;
BezierCurve* curve2;
BezierCurve* curve3;
BezierCurve* curve4;
BezierCurve* curve5;

//initial control point values
vec3 temp1 = vec3(0,0,10);
vec3 temp2 = vec3(0,-5,6.7);
vec3 temp3 = vec3(0,5,3.3);
vec3 temp4 = vec3(0,1,1);

vec3 temp5 = vec3(-2,0.5,1);
vec3 temp6 = vec3(-2,0.5,0.33);
vec3 temp7 = vec3(-2,0.5,-0.33);
vec3 temp8 = vec3(-2,0.5,-1);

vec3 temp9 = vec3(-1,0.5,-2.5);
vec3 temp10 = vec3(-0.33,0.5,-2.5);
vec3 temp11 = vec3(0.33,0.5,-2.5);
vec3 temp12 = vec3(1,0.5,-2.5);

vec3 temp13 = vec3(2,0.5,-1);
vec3 temp14 = vec3(2,0.5,-0.33);
vec3 temp15 = vec3(2,0.5,0.33);
vec3 temp16 = vec3(2,0.5,1);

vec3 temp17 = vec3(0,2,1);
vec3 temp18 = vec3(0,2,0.33);
vec3 temp19 = vec3(0,3,-0.33);
vec3 temp20 = vec3(0,2,-1);


vector<vec3> allControlPoints;

vector<vec3> curveVertices;
vector<vec3> curveVertices2;
vector<vec3> curveVertices3;
vector<vec3> curveVertices4;
vector<vec3> curveVertices5;

vec3 lastLocation;
int curveIndex = 0;

vector<ControlPoint * > ctrlPointObjects;

double lastTime = -1;
double currentTime;
double deltaTime = 0;
double totalElapsedTime = 0;

glm::mat4 Window::projection; // Projection matrix.
double fov = 60.0;

glm::vec3 Window::eye(5, 5, 5); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
//glm::vec3 Window::eye(0, 0, 50); // Camera position.
//glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
//glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

// Light position in the world space
glm::vec3 Window::light_pos = glm::vec3(8, 0, 8);

GLuint Window::program; // The shader program id.
GLuint Window::skyboxShader; //the skybox shader program id.
GLuint Window::cubemapShader; //the cubemap shader program id.
GLuint Window::bezierCurveShader;
GLuint Window::bezierHandleShader;

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::lightLoc; // Location of light in shader.
GLuint Window::eyeLoc; // Location of cameraPos in shader.
GLuint Window::lightColorLoc; // location of the lightColor in shader

//car model
Shader* ourShader;
Model* ourModel;
vec3 carPosition;
vec3 speedVector = vec3(0,0,0.1);
float currentSpeed = 0.2;
int speedIndex = 0;
bool leftPress = false;
bool rightPress = false;

//particle systems
Shader* particleShader;
ParticleGenerator *Particles;
bool boost = false;

//terrain
vector<unsigned int> textureIDs;
Terrain * terrain;
bool fade = true;
GLuint terrainShader;

//camera
float camera_turn = 0;
float theta_angle = 0;


bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    cubemapShader = LoadShaders("shaders/cubemap.vert", "shaders/cubemap.frag");
    bezierCurveShader = LoadShaders("shaders/bezierCurve.vert", "shaders/bezierCurve.frag");
    bezierHandleShader = LoadShaders("shaders/bezierHandle.vert","shaders/bezierHandle.frag");
    terrainShader = LoadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
    if (!skyboxShader)
    {
        std::cerr << "Failed to initialize skybox program" << std::endl;
        return false;
    }
    if (!cubemapShader)
    {
        std::cerr << "Failed to initialize cubemap program" << std::endl;
        return false;
    }
    if (!bezierCurveShader)
    {
        std::cerr << "Failed to initialize bezier curve program" << std::endl;
        return false;
    }
    if (!bezierHandleShader)
    {
        std::cerr << "Failed to initialize bezier handle program" << std::endl;
        return false;
    }

    

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
    projectionLoc = glGetUniformLocation(program, "projection");
    lightLoc = glGetUniformLocation(program, "light_position_world");
    eyeLoc = glGetUniformLocation(program, "viewPos");
    lightColorLoc = glGetUniformLocation(program, "lightColor");
    
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    
    glUseProgram(cubemapShader);
    glUniform1i(glGetUniformLocation(cubemapShader, "skybox"), 0);
    
	return true;
}

bool Window::initializeObjects()
{
    terrain = new Terrain();
    textureIDs.push_back(TextureFromFile("resources/textures/sand.jpg", "."));
    textureIDs.push_back(TextureFromFile("resources/textures/grass.jpg", "."));
    textureIDs.push_back(TextureFromFile("resources/textures/snow.jpg", "."));
    textureIDs.push_back(TextureFromFile("resources/textures/grass_rock.jpg", "."));
    
    particleShader = new Shader("shaders/particle.vs", "shaders/particle.fs");
    
    Particles = new ParticleGenerator(*particleShader, (GLuint) 500);
    
    lastLocation = eye;
    
    animationBools[0] = true;
    
    SoundEngine = createIrrKlangDevice();
    
    ourShader = new Shader("shaders/1.model_loading.vs", "shaders/1.model_loading.fs");
    //"Lamborginhi Aventador OBJ/Lamborghini_Aventador.obj"
    //"nanosuit/nanosuit.obj"
    ourModel = new Model("Gta-spano-2010_obj/Gta-spano-2010.obj");
    
    frust = new Frustum(fov,double(width) / (double)height, 1.0, 100.0);
    
    world = new Transform(mat4(1.0f));
    
    skybox = new Skybox(skyboxShader);
    
    cubemapTexture = skybox->loadCubemap(faces);
    
    skybox->setCubemap(cubemapTexture);
    
    world->addNode(skybox);
    
    allControlPoints.clear();
    allControlPoints.push_back(temp1);
    allControlPoints.push_back(temp2);
    allControlPoints.push_back(temp3);
    allControlPoints.push_back(temp4);
    
    handleBezierCurves(curve, &curveVertices);
    
    allControlPoints.clear();
    allControlPoints.push_back(temp5);
    allControlPoints.push_back(temp6);
    allControlPoints.push_back(temp7);
    allControlPoints.push_back(temp8);
    
    handleBezierCurves(curve2, &curveVertices2);
    
    allControlPoints.clear();
    allControlPoints.push_back(temp9);
    allControlPoints.push_back(temp10);
    allControlPoints.push_back(temp11);
    allControlPoints.push_back(temp12);
    
    handleBezierCurves(curve3, &curveVertices3);
    
    allControlPoints.clear();
    allControlPoints.push_back(temp13);
    allControlPoints.push_back(temp14);
    allControlPoints.push_back(temp15);
    allControlPoints.push_back(temp16);
    
    handleBezierCurves(curve4, &curveVertices4);
    
    allControlPoints.clear();
    allControlPoints.push_back(temp17);
    allControlPoints.push_back(temp18);
    allControlPoints.push_back(temp19);
    allControlPoints.push_back(temp20);
    
    handleBezierCurves(curve5, &curveVertices5);


	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
    delete world;
    

	// Delete the shader program.
	glDeleteProgram(program);
    glDeleteProgram(cubemapShader);
    glDeleteProgram(skyboxShader);
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
	Window::projection = glm::perspective(glm::radians(fov),
                            double(width) / (double)height, 1.0, 1500.0);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
    
    Particles->Update(0.01);
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

    //Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(fov),
    double(width) / (double)height, 1.0, 1500.0);
    
    //frust->CalculateFrustum(view, projection);
    

	// Specify the values of the uniform variables we are going to use.
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(lightLoc, 1, glm::value_ptr(light_pos));
    glUniform3fv(eyeLoc, 1, glm::value_ptr(eye));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    

    steer();
    currentTime = glfwGetTime();
    if(lastTime == -1){
        lastTime = currentTime;
    }
    deltaTime = currentTime - lastTime;

    if(animationBools[0]){
        animationBools[0] = soundSubroutine("immortal.wav", false);
        animationBools[1] = !animationBools[0];
    }else if(animationBools[1]){
        animationBools[1] = waitSubroutine(2.7);
        animationBools[2] = !animationBools[1];
    }else if(animationBools[2]){
        animationBools[2] = fadeSubroutine(2.4, vec3(0,0,0));
        animationBools[3] = !animationBools[2];
    }else if(animationBools[3]){
        eye = vec3(-5,5,5);
        view = glm::lookAt(Window::eye, Window::center, Window::up);
        animationBools[3] = waitSubroutine(2.7);
        animationBools[4] = !animationBools[3];
    }else if(animationBools[4]){
        animationBools[4] = fadeSubroutine(2.3, vec3(0,0,0));
        animationBools[5] = !animationBools[4];
    }else if(animationBools[5]){
        eye = vec3(0,5,-5);
        view = glm::lookAt(Window::eye, Window::center, Window::up);
        animationBools[5] = waitSubroutine(3.0);
        animationBools[6] = !animationBools[5];
    }else if(animationBools[6]){
        animationBools[6] = fadeSubroutine(2.8, vec3(0,0,0));
        animationBools[7] = !animationBools[6];
    }else if(animationBools[7]){
        animationBools[7] = cameraSubroutine(1.0, curveVertices); //timed
        animationBools[8] = !animationBools[7];
    }else if(animationBools[8]){
        animationBools[8] = fadeSubroutine(1.0, vec3(0,0,0));
        animationBools[9] = !animationBools[8];
    }else if(animationBools[9]){
        animationBools[9] = cameraSubroutine(1.0, curveVertices2);
        animationBools[10] = !animationBools[9];
    }else if(animationBools[10]){
        animationBools[10] = fadeSubroutine(1.0, vec3(0,0,0));
        animationBools[11] = !animationBools[10];
    }else if(animationBools[11]){
        animationBools[11] = cameraSubroutine(1.0, curveVertices3);
        animationBools[12] = !animationBools[11];
    }else if(animationBools[12]){
        animationBools[12] = fadeSubroutine(1.0, vec3(0,0,0));
        animationBools[13] = !animationBools[12];
    }else if(animationBools[13]){
        animationBools[13] = cameraSubroutine(1.0, curveVertices4);
        animationBools[14] = !animationBools[13];
    }else if(animationBools[14]){
        animationBools[14] = fadeSubroutine(1.0, vec3(0,0,0));
        animationBools[15] = !animationBools[14];
    }else if(animationBools[15]){
        animationBools[15] = cameraSubroutine(1.0, curveVertices5);
        animationBools[16] = !animationBools[15];
    }else if(animationBools[16]){
        animationBools[16] = fadeSubroutine(1.0, vec3(0,0,0));
        animationBools[17] = !animationBools[16];
    }else if(animationBools[17]){
        eye = vec3(1,0.4,-2);
        center = vec3(0,0,0);
        view = glm::lookAt(Window::eye, Window::center, Window::up);
        animationBools[17] = false;
        animationBools[18] = !animationBools[16];
    }else if(animationBools[18]){
        animationBools[18] = fadeSubroutine(0.2, vec3(1,1,0));
        animationBools[19] = !animationBools[18];
    }else if(animationBools[19]){
        animationBools[19] = waitSubroutine(0.1);
        animationBools[20] = !animationBools[19];
    }else if(animationBools[20]){
        animationBools[20] = fadeSubroutine(0.2, vec3(1,1,0));
        animationBools[21] = !animationBools[20];
    }else if(animationBools[21]){
        animationBools[21] = waitSubroutine(0.1);
        animationBools[22] = !animationBools[21];
    }else if(animationBools[22]){
        animationBools[22] = fadeSubroutine(0.2, vec3(1,1,0));
        animationBools[23] = !animationBools[22];
    }else if(animationBools[23]){
        animationBools[23] = waitSubroutine(0.1);
        animationBools[24] = !animationBools[23];
    }else if(animationBools[24]){
        boost = true;
        animationBools[24] = soundSubroutine("revving.wav", GL_FALSE);
        animationBools[25] = !animationBools[24];
    }else if(animationBools[25]){
        animationBools[25] = waitSubroutine(0.5);
        animationBools[26] = !animationBools[25];
    }else if(animationBools[26]){
        boost = false;
        animationBools[26] = waitSubroutine(0.25);
        animationBools[27] = !animationBools[26];
    }else if(animationBools[27]){
        boost = true;
        animationBools[27] = waitSubroutine(0.5);
        animationBools[28] = !animationBools[27];
    }else if(animationBools[28]){
        boost = false;
        
        animationBools[28] = waitSubroutine(0.5);
        animationBools[29] = !animationBools[28];
    }else if(animationBools[29]){
        /*speedIndex++;
        if(speedIndex % 50 == 0){
            currentSpeed++;
        }*/
        carPosition = carPosition + (speedVector * currentSpeed);
        eye = vec3(carPosition.x, carPosition.y + 3, carPosition.z- 5);
        center = carPosition;
        view = glm::lookAt(Window::eye, Window::center, Window::up);
        fade = false;

        //animationBools[29] = waitSubroutine(0.5);
        //animationBools[30] = !animationBools[29];
    }
    


	// Render the object.
    world->draw(mat4(1.0f));
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    if(is_button_down) {
        double xpos, ypos;
        int width, height;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);
        currPoint = trackBallMapping( (float)xpos, (float)ypos, width, height );
        glm::vec3 direction = currPoint - lastPoint;
        float velocity = glm::length(direction);
        if( velocity > 0.0001 ) // If little movement - do nothing.
        {
            glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
            float rot_angle = velocity * 50;
            view = glm::rotate(glm::radians(rot_angle), rotAxis) * view;
        }
        lastPoint = currPoint;
    }
    // don't forget to enable shader before setting uniforms
    ourShader->use();

    // view/projection transformations
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, carPosition); // translate it down so it's at the center of the scene
    model = glm::rotate(model, glm::radians(((theta_angle * -1) * 50.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(theta_angle * 10.0f * camera_turn), glm::vec3(0.0f, 0.0f, 1.0f));
    
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));    // it's a bit too big for our scene, so scale it down
    ourShader->setMat4("model", model);
    ourShader->setFloat("shininess", 1.0f);
    ourShader->setVec3("lightColor", vec3(1.0f,1.0f,1.0f));
    ourShader->setVec3("light_position_world", light_pos);
    ourShader->setFloat("shininess", 64.0f);
    ourShader->setVec3("viewPos", eye);
    ourShader->setFloat("alpha", 1.0f);
    ourShader->setVec3("lookPos", carPosition);
    ourModel->Draw(*ourShader);
    
    
    
    if(boost){
        Particles->Draw();
    }

    if(fade){
        mygl_GradientBackground( gradientColor1.x,gradientColor1.y , gradientColor1.z, alpha, gradientColor1.x, gradientColor1.y, gradientColor1.z, alpha );
    }else{
        glUseProgram(terrainShader);
        terrain->draw(terrainShader, projection, view, textureIDs);
    }
	glfwSwapBuffers(window);
}

void Window::interpolate(glm::vec3 fromPos, glm::vec3 toPos, float weight){
    light_pos = fromPos + weight * (toPos - fromPos);
}
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && frust->getFOVY() <= 60.0f){
        fov -= yoffset;
        frust->setFOVY(fov);
    }
    if(fov <= 1.0f){
        fov = 1.0f;
        frust->setFOVY(fov);
    }
    if(fov >= 60.0f){
        fov = 60.0f;
        frust->setFOVY(fov);
    }
}
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action){
            double xpos, ypos;
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            glfwGetCursorPos(window, &xpos, &ypos);
            lastPoint = trackBallMapping((float)xpos, (float)ypos, width, height);
            is_button_down = true;
        }
        else if(GLFW_RELEASE == action)
            is_button_down = false;
    }
}
glm::vec3 Window::trackBallMapping( float xpos, float ypos, int width, int height)
{
    glm::vec3 v;
    float d;
    v.x = (2.0 * xpos - width) / width;
    v.y = (height - 2.0 * ypos) / height;
    v.z = 0.0;
    d = glm::length(v);
    d = (d<1.0) ? d : 1.0;
    v.z = sqrt(1.001 - d*d);
    v = glm::normalize(v);
    return v;
}
void Window::setMaterial (int option, Geometry* obj){
    if(option == 1){
        material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
        material.shininess = 4.0f;
        ambientStrength = 1.0f;
        diffuseStrength = 0.0f;
        specularStrength = 1.0f;
        
    }else if( option == 2){
        material.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
        material.diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
        material.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
        material.shininess = 0.6f;
        ambientStrength = 1.0f;
        diffuseStrength = 1.0f;
        specularStrength = 0.0f;
    }else if (option == 3){
        material.ambient = glm::vec3(0.25f, 0.25f, 0.25f);
        material.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
        material.specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);
        material.shininess = 0.6f;
        ambientStrength = 1.0f;
        diffuseStrength = 1.0f;
        specularStrength = 1.0f;
    }else if (option == 4){
        material.ambient = glm::vec3(0.24725, 0.1995, 0.0745);
        material.diffuse = glm::vec3(0.75164, 0.60648, 0.22648);
        material.specular = glm::vec3(0.628281, 0.555802, 0.366065);
        material.shininess = 0.4f;
        ambientStrength = 1.0f;
        diffuseStrength = 1.0f;
        specularStrength = 1.0f;
    }
    else if (option == 5){
        material.ambient = glm::vec3(0.19225,    0.19225,    0.19225);
        material.diffuse = glm::vec3(0.50754,    0.50754,    0.50754);
        material.specular = glm::vec3(0.508273,    0.508273,    0.508273);
        material.shininess = 0.4f;
        ambientStrength = 1.0f;
        diffuseStrength = 1.0f;
        specularStrength = 1.0f;
    }
    else{ //light material
        material.ambient = lightColor;
        material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
        material.shininess = 0.0f;
        ambientStrength = 1.0f;
        diffuseStrength = 0.0f;
        specularStrength = 0.0f;
    }
    obj->setMaterial(material.ambient, ambientStrength, material.diffuse, diffuseStrength, material.specular, specularStrength, material.shininess);
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
            case GLFW_KEY_C:{
                SoundEngine->play2D("dixie-horn.wav", GL_FALSE);
                break;
            }
            case GLFW_KEY_W:{
                view = glm::translate(view, vec3(0,0,1));
                break;
            }
            case GLFW_KEY_S:{
                view = glm::translate(view, vec3(0,0,-1));
                break;
            }
            case GLFW_KEY_A:{
                view = glm::translate(view, vec3(-1,0,0));
                break;
            }
            case GLFW_KEY_D:{
                view = glm::translate(view, vec3(1,0,0));
                break;
            }
            case GLFW_KEY_Z:{
                view = glm::translate(view, vec3(0,1,0));
                break;
            }
            case GLFW_KEY_X:{
                view = glm::translate(view, vec3(0,-1,0));
                break;
            }
            default:
                break;
		}
	}
    if(key == GLFW_KEY_A && action == GLFW_PRESS){
        leftPress = true;
    }else if(key == GLFW_KEY_A && action == GLFW_RELEASE){
        leftPress = false;
    }
    if(key == GLFW_KEY_D && action == GLFW_PRESS){
        rightPress = true;
    }else if(key == GLFW_KEY_D && action == GLFW_RELEASE){
        rightPress = false;
    }
    
}

void Window::steer() {
    if (leftPress && !rightPress) {
        if (camera_turn < 10.0f) camera_turn += 0.1;
        theta_angle -= 0.01f;
        speedVector.z = cos(theta_angle) * 0.5;
        speedVector.x = -sin(theta_angle) * 0.5;
    } else if (!leftPress && !rightPress) {
        if (camera_turn > 0.0f)
            camera_turn -= 0.1;
    }
    if (rightPress && !leftPress) {
        if (camera_turn < 10.0f) camera_turn += 0.1;
        theta_angle += 0.01f;
        speedVector.z = cos(theta_angle) * 0.5;
        speedVector.x = -sin(theta_angle) * 0.5;
    } else if (!leftPress && !rightPress) {
        if (camera_turn > 0.0f)
            camera_turn -= 0.1;
    }
}


bool Window::fadeSubroutine(double amountTime, vec3 color){
    gradientColor1 = color;
    double velocity;
    if(amountTime > 5){
        velocity = 0.04;
    }else{
        velocity = (0.75 * amountTime)/ 100;
    }
    if(deltaTime >= velocity){
        if(alpha < 1.0){
            alpha += 0.01;
        }
        lastTime = currentTime;
        totalElapsedTime += deltaTime;
    }
    if(totalElapsedTime >= amountTime){
        alpha = 0.0f;
        totalElapsedTime = 0;
        //continue complete
        return false;
    }
    //animation continue
    return true;
}
bool Window::waitSubroutine(double amountTime){
    if(deltaTime >= amountTime){
        lastTime = currentTime;
        //wait complete
        return false;
    }
    //continue waiting
    return true;
}
bool Window::soundSubroutine(string filename, bool repeat){
    if(repeat){
        SoundEngine->play2D(filename.c_str(), GL_TRUE);
    }else{
        SoundEngine->play2D(filename.c_str(), GL_FALSE);
    }
    lastTime = currentTime;
    return false;
}
bool Window::cameraSubroutine(double amountTime, vector<vec3> vertices){
    double velocity = amountTime / vertices.size();
    if(deltaTime >= velocity){
        lastLocation = vertices[curveIndex];
        eye = lastLocation;
        curveIndex ++;
        curveIndex = curveIndex % vertices.size();
        view = glm::lookAt(Window::eye, Window::center, Window::up);
        lastTime = currentTime;
        totalElapsedTime += deltaTime;
    }
    if(totalElapsedTime >= amountTime){
        totalElapsedTime = 0;
        curveIndex = 0;
        return false;
    }
    return true;
    
}
void Window::handleBezierCurves(BezierCurve* curve, vector<vec3>* vertices){
    curve = new BezierCurve(bezierCurveShader, allControlPoints);
    
    world->addNode(curve);
    
    ctrlPointObjects.clear();
    for(int i = 0; i< allControlPoints.size(); i++){
        ctrlPointObjects.push_back(NULL);
    }
    
    vector<vec3> tempVec1 = curve->getAllpoints();
    vector<int> tempVec2 = curve->getAnchorpoints();
    
    for(int i = 0; i < tempVec2.size(); i++){
        ControlPoint* ctrlPoint = new ControlPoint(tempVec1[tempVec2[i]], bezierCurveShader, true);
        ctrlPointObjects[tempVec2[i]] = ctrlPoint;
        world->addNode(ctrlPoint);
    }
    
    tempVec2 = curve->getCtrlpoints();
    for(int i = 0; i < tempVec2.size(); i++){
        ControlPoint* ctrlPoint = new ControlPoint(tempVec1[tempVec2[i]], bezierCurveShader, false);
        ctrlPointObjects[tempVec2[i]] = ctrlPoint;
        world->addNode(ctrlPoint);
    }
    
    *vertices = curve->getCurveVertices();
}
#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x
void Window::mygl_GradientBackground( float top_r, float top_g, float top_b, float top_a,
                              float bot_r, float bot_g, float bot_b, float bot_a )
{
  glDisable(GL_DEPTH_TEST);

  static GLuint background_vao = 0;
  static GLuint background_shader = 0;
  
  if (background_vao == 0)
  {
    glGenVertexArrays(1, &background_vao);
  
    const char* vs_src = (const char*) SHADER_HEADER SHADER_STR
    (
      out vec2 v_uv;
      void main()
      {
        uint idx = uint(gl_VertexID);
        gl_Position = vec4( idx & 1U, idx >> 1U, 0.0, 0.5 ) * 4.0 - 1.0;
        v_uv = vec2( gl_Position.xy * 0.5 + 0.5 );
      }
    );

    const char* fs_src = (const char*) SHADER_HEADER SHADER_STR
    (
      uniform vec4 top_color;
      uniform vec4 bot_color;
      in vec2 v_uv;
      out vec4 frag_color;

      void main()
      {
        frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;
      }
    );
    GLuint vs_id, fs_id;
    vs_id = glCreateShader( GL_VERTEX_SHADER );
    fs_id = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource(vs_id, 1, &vs_src, NULL);
    glShaderSource(fs_id, 1, &fs_src, NULL);
    glCompileShader(vs_id);
    glCompileShader(fs_id);
    background_shader = glCreateProgram();
    glAttachShader( background_shader, vs_id );
    glAttachShader( background_shader, fs_id );
    glLinkProgram(  background_shader );
    glDetachShader( background_shader, fs_id );
    glDetachShader( background_shader, vs_id );
    glDeleteShader( fs_id );
    glDeleteShader( vs_id );
    glUseProgram( background_shader );
  }

  glUseProgram( background_shader );
  GLuint top_color_loc = glGetUniformLocation( background_shader, "top_color" );
  GLuint bot_color_loc = glGetUniformLocation( background_shader, "bot_color" );
  glUniform4f( top_color_loc, top_r, top_g, top_b, alpha );
  glUniform4f( bot_color_loc, bot_r, bot_g, bot_b, alpha );
  
  glBindVertexArray( background_vao );
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
}
