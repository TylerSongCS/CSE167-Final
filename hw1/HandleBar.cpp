//
//  HandleBar.cpp
//  hw1
//
//  Created by Mingxun Song on 11/9/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "HandleBar.hpp"
#include "Window.h"
#include <sstream>
#include <cmath>
#include <iostream>

HandleBar::HandleBar(vector<vec3> temp, GLuint shaderProgram){
    this->shaderProgram = shaderProgram;
    points = temp;
    
    rgb = vec3(1,1,0);
    vector<vec3> lineColors;
    
    for( int i = 0; i < temp.size(); i++){
        lineColors.push_back(rgb);
    }
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &colorbuffer);
    
    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Pass in the data.
     glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
         points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lineColors.size(), lineColors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                  // attribute
        3,                                  // size
        GL_FLOAT,                           // type
        GL_FALSE,                           // normalized?
        3 * sizeof(GLfloat),
        0                                   // array buffer offset
    );
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the colorbuffer
    glBindBuffer(GL_ARRAY_BUFFER, 1);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

HandleBar::~HandleBar(){}

void HandleBar::setPoints(vector<vec3> temp){
    points = temp;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
         points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void HandleBar::draw(mat4 C){
    glUseProgram(shaderProgram);
    modelToWorld = C;
    
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    modelLoc = glGetUniformLocation(shaderProgram, "model");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Window::projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Window::view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    // Bind to the VAO.
    glBindVertexArray(vao);
    
    glDrawArrays(GL_LINES, 0, (GLsizei) points.size());
    glBindVertexArray(0);
}
