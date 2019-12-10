//
//  particle_generator.hpp
//  hw1
//
//  Created by Mingxun Song on 12/8/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef particle_generator_hpp
#define particle_generator_hpp

#include <vector>

#include <glm/glm.hpp>

#include "shader2.h"
#include "particle.hpp"
#include "Window.h"

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, GLuint amount);
    // Update all particles
    void Update(GLfloat dt);
    // Render all particles
    void Draw();
    void handleColors();
    void translate(vec3 transformation);
    void rotate(float degree, glm::vec3 direction);
private:
    std::vector<particle> particles;
    std::vector<glm::vec3> points;
    std::vector<glm::vec4> colors;
    GLuint amount;
    Shader shader;
    mat4 model = mat4(1.0f);
    GLuint vao, vbo, colorbuffer;
};

#endif /* particle_generator_hpp */
