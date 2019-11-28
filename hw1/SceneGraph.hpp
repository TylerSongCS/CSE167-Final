//
//  SceneGraph.hpp
//  hw1
//
//  Created by Mingxun Song on 10/25/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef SceneGraph_hpp
#define SceneGraph_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>


using namespace glm;
using namespace std;


class Node {
public:
    virtual void draw( mat4 C)=0;;
    virtual void update()=0;
protected:
    int numChildren;
    
};

class Transform : public Node {
public:
    Transform();
    Transform(mat4 M);
    ~Transform();
    void addNode (Node* child);
    void draw( mat4 C);
    void update();
    void scale(vec3 factor);
    void translate(vec3 factor);
    void rotate(float degree, vec3 axis);
    void setAnimations(int options);
    void animate();
    vec3 center;
    bool shouldRender = true;
protected:
    vector<Node*> childNodes;
    std::list<int> animations;
    mat4 M;
    
    
};

class Geometry: public Node {
public:
    Geometry();
    Geometry(string objFilename, GLuint shaderProgram);
    ~Geometry();
    void draw(mat4 C);
    void update();
    void scale(vec3 factor);
    void translate(vec3 factor);
    void rotate(float degree, vec3 axis);
    mat4 getModel();
    void setMaterial(vec3 ambient, float ambientStrength, vec3 diffuse,
                     float diffuseStrength, vec3 specular, float specularStrength,
                     float shininess );
    void setRenderMode(float isPhong);
    void setIsBoundingSpere(bool b);
    unsigned int cubemapTexture = 0;
protected:
    mat4 modelToWorld;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<glm::vec3> colors;
    GLuint vao, vbo, colorbuffer, normalbuffer;
    GLuint projectionLoc, viewLoc, modelLoc, colorLoc, lightLoc, eyeLoc, ambLoc, diffLoc, specLoc, shinLoc, ambientStrengthLoc, diffuseStrengthLoc, specularStrengthLoc, isPhongLoc, cameraPosLoc;
    GLuint shaderProgram;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float ambientStrength, diffuseStrength, specularStrength;
    int isPhong = 0;
    bool isBoundingSphere = false;
    
};
#endif /* SceneGraph_hpp */
