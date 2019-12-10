//
//  particle_generator.cpp
//  hw1
//
//  Created by Mingxun Song on 12/8/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "particle_generator.hpp"

ParticleGenerator::ParticleGenerator(Shader shader, GLuint amount) : shader(shader), amount(amount)
{
    for(int i = 0; i < amount; i++){
        particles.push_back(particle());
        points.push_back(glm::vec3(0,0,0));
        handleColors();
    }
    
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1,&colorbuffer);
    
    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
     glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colors.size(), colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the colorbuffer.
    glBindBuffer(GL_ARRAY_BUFFER, 1);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
void ParticleGenerator::Update(GLfloat deltaTime)
{
    points.clear();
    std::vector<particle>::iterator it;
    for(it = particles.begin(); it != particles.end(); it++){
        it->update(deltaTime);
        if(it->life <= 0){
            it->reset(vec3(0,0,0));
        }
        points.push_back(it->position);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
}

// Render all particles
void ParticleGenerator::Draw()
{

    shader.use();
    glm::mat4 model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1,0,0));
    model = glm::translate(model, vec3(0.2,1,0.15));
    shader.setMat4("projection", Window::projection);
    shader.setMat4("view", Window::view);
    shader.setMat4("model", model);
    // Bind to the VAO.
    glBindVertexArray(vao);
    glPointSize(2.5f);
    glDrawArrays(GL_POINTS, 0, (GLsizei) points.size());
    glBindVertexArray(0);
}

void ParticleGenerator::handleColors(){
    int random = rand() % 4;
    std::cerr << random << std::endl;
    vec4 color;
    if(random == 0){
        color = vec4(0.07,0.11,0.37,1);
    }else if(random == 1){
        color = vec4(0.7,0.12,0.4,1);
    }else if(random == 2){
        color = vec4(1,0.2,0.43,1);
    }else if(random == 3){
        //color = vec4(1,0.74,0.41,1);
        color = vec4(0,0,0,1);
    }else{
        color = vec4(0,0,0,1);
    }
        
    colors.push_back(color);
}
void ParticleGenerator::translate(vec3 transformation){
    model = glm::translate(model, transformation);
}

void ParticleGenerator::rotate(float degree, vec3 direction){
    model = glm::rotate(model, glm::radians(degree), direction);
}
