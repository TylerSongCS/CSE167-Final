//
//  BoundingSphere.hpp
//  hw1
//
//  Created by Mingxun Song on 10/29/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef BoundingSphere_hpp
#define BoundingSphere_hpp

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

class BoundingSphere {
public:
    BoundingSphere();
    void setCenter(vec3 center);
    void setRadius(float radius);
    vec3 getCenter();
    float getRadius();
protected:
    vec3 center;
    float radius;
};
#endif /* BoundingSphere_hpp */
