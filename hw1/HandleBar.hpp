//
//  HandleBar.hpp
//  hw1
//
//  Created by Mingxun Song on 11/9/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef HandleBar_hpp
#define HandleBar_hpp

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

class HandleBar : public Geometry{
    public:
        HandleBar(vector<vec3> temp, GLuint shaderProgram);
        ~HandleBar();
        void setPoints(vector<vec3> temp);
        void draw(mat4 C);
    protected:
        mat4 modelToWorld;
        vec3 rgb;
        GLuint colorLoc;
        vector<vec3> points;
};
#endif /* HandleBar_hpp */
