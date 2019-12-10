#ifndef terrain_hpp
#define terrain_hpp

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <stdio.h>
#include "perlin_noise.hpp"

class Terrain {
    private:
        int rows;
        int cols;
        int height;
        int width;
        GLuint vao;
        GLuint vbos[3];
        GLuint ebo;
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<GLuint> indices;
        std::vector<glm::vec2> texture_coords;
        std::vector<glm::vec2> z_buffer;

        Terrain();
        ~Terrain();

        void init();
        void draw(GLuint shader, glm::mat4 P, glm::mat4 V, std::vector<unsigned int> textureIDs);
        void generate_terrain();
        void calculate_normals();
        glm::vec3 calculate_surface_normal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
        void calculate_indices();

        void calculate_texture_coordinates();

        bool check_collision(glm::vec3 position);
        void regenerate();
};
#endif /* terrain_hpp */
