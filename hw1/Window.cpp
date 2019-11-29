#include "Window.h"
#include <list>
#include <cmath>
#include "model.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
using namespace std;


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

HandleBar* handles;

//initial control point values
vec3 temp1;
vec3 temp2 = vec3(4,0,-4);
vec3 temp3 = vec3(6,5,-6);
vec3 temp4;
vec3 temp5 = vec3(-3,0,-3);
vec3 temp6 = vec3(-4,0,-4);
vec3 temp7;
vec3 temp8 = vec3(-7,0,0);
vec3 temp9 = vec3(-8,4,-4);
vec3 temp10;
vec3 temp11 = vec3(-3,5,0);
vec3 temp12 = vec3(-10,0,-4);
vec3 temp13;
vec3 temp14 = vec3(-10,5,0);
vec3 temp15= vec3(-13,0,-4);
vec3 temp16;
vec3 temp17 = vec3(-6,-5,0);
vec3 temp18 = vec3(-8,0,0);
vec3 temp19;
vec3 temp20 = vec3(-4,-5,0);
vec3 temp21 = vec3(-3,0,0);
vec3 temp22;
vec3 temp23 = vec3(1,-5,0);
vec3 temp24= vec3(3,0,0);

int currentControlPoint = 0;
vector<vec3> allControlPoints;

vector<vec3> curveVertices;
vec3 lastLocation;
int curveIndex = 0;
bool paused = false;

vector<ControlPoint * > ctrlPointObjects;

double lastTime = -1;
double currentTime;
double deltaTime = 0;

glm::mat4 Window::projection; // Projection matrix.
double fov = 60.0;

glm::vec3 Window::eye(0, 5, 10); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

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

Shader* ourShader;

Model* ourModel;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    cubemapShader = LoadShaders("shaders/cubemap.vert", "shaders/cubemap.frag");
    bezierCurveShader = LoadShaders("shaders/bezierCurve.vert", "shaders/bezierCurve.frag");
    bezierHandleShader = LoadShaders("shaders/bezierHandle.vert","shaders/bezierHandle.frag");
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
    ourShader = new Shader("1.model_loading.vs", "1.model_loading.fs");
    
    ourModel = new Model("nanosuit/nanosuit.obj");
    
    frust = new Frustum(fov,double(width) / (double)height, 1.0, 100.0);
    
    world = new Transform(mat4(1.0f));
    
    carTransform = new Transform(mat4(1.0f));
    
    car = new Geometry(sphereFileName, cubemapShader);
    
    world->addNode(carTransform);
    
    carTransform->addNode(car);
    
    skybox = new Skybox(skyboxShader);
    
    cubemapTexture = skybox->loadCubemap(faces);
    
    skybox->setCubemap(cubemapTexture);
    
    car -> cubemapTexture = cubemapTexture;
    
    world->addNode(skybox);
    
    allControlPoints.push_back(temp1);
    allControlPoints.push_back(temp2);
    allControlPoints.push_back(temp3);
    allControlPoints.push_back(temp4);
    allControlPoints.push_back(temp5);
    allControlPoints.push_back(temp6);
    allControlPoints.push_back(temp7);
    allControlPoints.push_back(temp8);
    allControlPoints.push_back(temp9);
    allControlPoints.push_back(temp10);
    allControlPoints.push_back(temp11);
    allControlPoints.push_back(temp12);
    allControlPoints.push_back(temp13);
    allControlPoints.push_back(temp14);
    allControlPoints.push_back(temp15);
    allControlPoints.push_back(temp16);
    allControlPoints.push_back(temp17);
    allControlPoints.push_back(temp18);
    allControlPoints.push_back(temp19);
    allControlPoints.push_back(temp20);
    allControlPoints.push_back(temp21);
    allControlPoints.push_back(temp22);
    allControlPoints.push_back(temp23);
    allControlPoints.push_back(temp24);
    
    curve = new BezierCurve(bezierCurveShader, allControlPoints);
    
    world->addNode(curve);
    
    allControlPoints = curve->getAllpoints();
    
    for(int i = 0; i< 24; i++){
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
    
    tempVec1 = curve->getHandlepoints();
    handles = new HandleBar(tempVec1, bezierHandleShader);
    world->addNode(handles);
    
    curveVertices = curve->getCurveVertices();
    
    currentControlPoint = 0;
    
    ctrlPointObjects[currentControlPoint]->setColor(vec3(0,0,1));
    
    lastLocation = curveVertices[curveIndex];
    carTransform->translate(lastLocation);
    
    carTransform->scale(carTransformFactor);

    
    /*for(int i = 0; i < 24; i++){
        Transform * temp = new Transform(mat4(1.0f));
        ctrlPointTransforms.push_back(temp);
        world->addNode(temp);
    }
    for(int i = 0; i < 24; i++){
        Geometry * temp = new Geometry(sphereFileName, program);
        ctrlPointGeometry.push_back(temp);
        ctrlPointTransforms[i]->addNode(temp);
    }
    for(int i = 0; i < 24; i++){
        ctrlPointTransforms[i]->translate(allControlPoints[i]);
        //ctrlPointTransforms[i]->scale(vec3(0.1f,0.1f,0.1f));
    }*/
    
    
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
                            double(width) / (double)height, 1.0, 100.0);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
    

}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

    //Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(frust->getFOVY()),
    frust->getAspect(), frust->getZNear(), frust->getZFar());
    
    frust->CalculateFrustum(view, projection);
    
	// Specify the values of the uniform variables we are going to use.
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(lightLoc, 1, glm::value_ptr(light_pos));
    glUniform3fv(eyeLoc, 1, glm::value_ptr(eye));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    if(lastTime == -1){
        lastTime = 0;
    }
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    
    if ( deltaTime >= 0.005 && !paused) {
        carTransform->scale((1.0f / carTransformFactor));
        carTransform->translate( -1.0f * (lastLocation));
        curveIndex ++;
        curveIndex = curveIndex % curveVertices.size();
        lastLocation = curveVertices[curveIndex];
        carTransform->translate(lastLocation);
        carTransform->scale(carTransformFactor);
        lastTime = currentTime;
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
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));    // it's a bit too big for our scene, so scale it down
    ourShader->setMat4("model", model);
    ourModel->Draw(*ourShader);

	// Swap buffers.
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
int Window::checkCurrentCtrl(int index){
    if(index % 3 == 0){                 //found anchor
        return 0;
    }else if((index - 1) % 3 == 0 ){    //found right handle
        return 1;
    }else{                              //found left handle
        return 2;
    }
    
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
            case GLFW_KEY_LEFT:{
                int temp = checkCurrentCtrl(currentControlPoint);
                if(temp != 0){
                    ctrlPointObjects[currentControlPoint]->setColor(vec3(0,1,0));
                }else{
                    ctrlPointObjects[currentControlPoint]->setColor(vec3(1,0,0));
                }
                if(currentControlPoint == 0){
                    currentControlPoint = 23;
                }else{
                    currentControlPoint--;
                }
                ctrlPointObjects[currentControlPoint]->setColor(vec3(0,0,1));
                break;
            }
            case GLFW_KEY_RIGHT:{
                int temp = checkCurrentCtrl(currentControlPoint);
                if(temp != 0){
                    ctrlPointObjects[currentControlPoint]->setColor(vec3(0,1,0));
                }else{
                    ctrlPointObjects[currentControlPoint]->setColor(vec3(1,0,0));
                }

                currentControlPoint++;
                currentControlPoint = currentControlPoint % 24;
            
                ctrlPointObjects[currentControlPoint]->setColor(vec3(0,0,1));
                break;
            }
            case GLFW_KEY_X:{
                if(mods == GLFW_MOD_SHIFT){
                    cerr<<"shift x" << endl;
                    handleManipulate(vec3(-1,0,0));
                }else{
                    cerr<<"reg x" << endl;
                    handleManipulate(vec3(1,0,0));
                }
                
                break;
            }
            case GLFW_KEY_Y:{
                if(mods == GLFW_MOD_SHIFT){
                    cerr<<"shift y" << endl;
                    handleManipulate(vec3(0,-1,0));
                }else{
                    cerr<<"reg y" << endl;
                    handleManipulate(vec3(0,1,0));
                }
                
                break;
            }
            case GLFW_KEY_Z:{
                if(mods == GLFW_MOD_SHIFT){
                    cerr<<"shift z" << endl;
                    handleManipulate(vec3(0,0,-1));
                }else{
                    cerr<<"reg z" << endl;
                    handleManipulate(vec3(0,0,1));
                }
                
                break;
            }
            case GLFW_KEY_P:{
                paused = !paused;
                
                break;
            }
            default:
                break;
		}
	}
}

void Window::handleManipulate(vec3 translateAmount){
    int temp = checkCurrentCtrl(currentControlPoint);
    int right;
    int left;

    if( temp == 0){
         right = currentControlPoint + 1;
        if(currentControlPoint == 0){
            left = 23;
        }else{
            left = currentControlPoint - 1;
        }
        allControlPoints[left] += translateAmount;
        allControlPoints[right] += translateAmount;
    }else if(temp == 1){
        allControlPoints[currentControlPoint] += translateAmount;
        if(currentControlPoint == 1){
            left = 23;
        }else{
            left = currentControlPoint - 2;
        }
        allControlPoints[left] -= translateAmount;
    }else{
        allControlPoints[currentControlPoint] += translateAmount;
        if(currentControlPoint == 23){
            right = 1;
        }else{
            right = currentControlPoint + 2;
        }
        allControlPoints[right] -= translateAmount;
    }


    curve->updateControlPoints(allControlPoints);
    
    allControlPoints = curve->getAllpoints();
    
    vector<int> anchors = curve->getAnchorpoints();
    
    for(int i = 0; i < anchors.size(); i++){
        int index = anchors[i];
        ctrlPointObjects[index]->setPoint(allControlPoints[index]);
    }
    
    vector<int> controls = curve->getCtrlpoints();
    
    for(int i = 0; i < controls.size(); i++){
        int index = controls[i];
        ctrlPointObjects[index]->setPoint(allControlPoints[index]);
    }
    
    vector<vec3> handlePoints = curve->getHandlepoints();
    handles->setPoints(handlePoints);
    
    curveVertices = curve->getCurveVertices();
    
    //reset car on the curve
    carTransform->scale((1.0f / carTransformFactor));
    carTransform->translate(-1.0f * lastLocation);
    carTransform->translate(curveVertices[curveIndex]);
    carTransform->scale(carTransformFactor);
    lastLocation = curveVertices[curveIndex];
    
}
