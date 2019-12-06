//
//  Frustum.hpp
//  hw1
//
//  Created by Mingxun Song on 10/29/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef Frustum_hpp
#define Frustum_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>
#include "Plane.hpp"

using namespace glm;
using namespace std;

//enum { TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR};
enum FrustumSide{
    RIGHT    = 0,        // The RIGHT side of the frustum
        LEFT    = 1,        // The LEFT     side of the frustum
        BOTTOM    = 2,        // The BOTTOM side of the frustum
        TOP        = 3,        // The TOP side of the frustum
        BACK    = 4,        // The BACK    side of the frustum
        FRONT    = 5            // The FRONT side of the frustum
};
    
enum PlaneData
{
    A = 0,                // The X value of the plane's normal
    B = 1,                // The Y value of the plane's normal
    C = 2,                // The Z value of the plane's normal
    D = 3                // The distance the plane is from the origin
};


class Frustum {
public:
    Frustum();
    Frustum(float, float, float, float);
    mat4& getProjection();
    float getFOVY();
    float getAspect();
    float getZNear();
    float getZFar();
    void setFOVY(float);
    void createPlanes(vec3, vec3, vec3, Plane*);
    void CalculateFrustum(mat4 &view_matrix, mat4 &proj_matrix);
    void normalizePlane(vec4 &frustum_plane);
    bool SphereInFrustum(vec3 &pos, float &radius);
    vec4* getFustumPlanes();
protected:
    float fovy, aspect, znear, zfar;
    mat4 projection;
    vec4 frustum_planes[6];

    
    
};
#endif /* Frustum_hpp */
