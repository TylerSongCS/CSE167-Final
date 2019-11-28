//
//  ControlPoint.hpp
//  hw1
//
//  Created by Mingxun Song on 11/8/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef ControlPoint_hpp
#define ControlPoint_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "SceneGraph.hpp"
#include <vector>
#include <string>
#include <list>


using namespace glm;
using namespace std;

class ControlPoint : public Geometry{
public:
    ControlPoint(vec3 point, GLuint shaderProgram, bool isAnchor);
    ~ControlPoint();
    void setColor(vec3 rgb);
    void setPoint(vec3 point);
    void setPointSize(float pointSize);
    void draw(mat4 C);
    bool getIsAnchor();
protected:
    bool isAnchor;
    mat4 modelToWorld;
    vec3 rgb = vec3(1,0,0);
    vec3 point;
    float pointSize;
    GLuint colorLoc;
    vector<vec3> points;
};

#endif /* ControlPoint_hpp */
