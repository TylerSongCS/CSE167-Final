#ifndef _OBJ_H_
#define _OBJ_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"
#include "parser.hpp"

class OBJ : public Object
{
private:
    GLuint vao;
    GLuint vbos[2];
    GLuint ebo;
    int num_triangles;
public:
    OBJ(float size, std::string filename);
    ~OBJ();
    
    void draw();
    void update();
    
    void spin(float deg);
    
    void set_position(glm::vec3 pos);

    void set_cubemapTexture(unsigned int ct);
};

#endif


