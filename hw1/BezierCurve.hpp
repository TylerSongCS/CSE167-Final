//
//  BezierCurve.hpp
//  hw1
//
//  Created by Mingxun Song on 11/7/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef BezierCurve_hpp
#define BezierCurve_hpp
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

class BezierCurve : public Geometry{
public:
    BezierCurve(GLuint shaderProgram, vector<vec3> temp);
    ~BezierCurve();
    void draw(mat4 C);
    vec3 lerp(float t, vector<vec3> controlPoints);
    void generateVertices(vector<vec3> controlPoints);
    void setColor(vec3 rgb);
    vector<vec3> getAllpoints();
    vector<int> getAnchorpoints();
    vector<int> getCtrlpoints();
    vector<vec3> getHandlepoints();
    vector<vec3> getCurveVertices();
    void setUpEverything(vector<vec3> temp);
    void updateControlPoints(vector<vec3> temp);
protected:
    mat4 modelToWorld;
    vector<vec3> curveVertices;
    vector<vec3> allPoints;
    vector<int> anchorPoints;
    vector<int> ctrlPoints;
    vector<vec3> handlePoints;
    GLuint colorLoc;
    vec3 rgb;
};


#endif /* BezierCurve_hpp */
