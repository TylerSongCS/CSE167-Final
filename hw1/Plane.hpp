//
//  Plane.hpp
//  hw1
//
//  Created by Mingxun Song on 10/29/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

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

class Plane {
public:
    Plane();
    Plane( vec3, vec3, float, float);
    void set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3);
    void setPoint(vec3);
    void setNormal(vec3);
    void setWidth(float);
    void setHeight(float);
    float getWidth();
    float getHeight();
    vec3& getPoint();
    vec3& getNormal();
    float distance(vec3);
    float d;
protected:
    float width;
    float height;
    vec3 point;
    vec3 normal;
};

#endif /* Plane_hpp */
