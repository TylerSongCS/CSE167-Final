//
//  ControlPoint.cpp
//  hw1
//
//  Created by Mingxun Song on 11/8/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "ControlPoint.hpp"
#include "Window.h"
#include <sstream>
#include <cmath>
#include <iostream>

ControlPoint::ControlPoint(vec3 point, GLuint shaderProgram, bool isAnchor){
    //this->point = point;
    points.push_back(point);
    this->shaderProgram = shaderProgram;
    this->isAnchor = isAnchor;
    if(isAnchor){
        this->rgb = vec3(1,0,0);
    }else{
        this->rgb = vec3(0,1,0);
    }
    this->pointSize = 2.5f;
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
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
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
ControlPoint::~ControlPoint(){}
void ControlPoint::setColor(vec3 rgb){
    this->rgb = rgb;
}
void ControlPoint::setPoint(vec3 point){
    vector<vec3> temp;
    temp.push_back(point);
    points = temp;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
         points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ControlPoint::setPointSize(float pointSize){
    this->pointSize = pointSize;
}
bool ControlPoint::getIsAnchor(){
    return isAnchor;
}

void ControlPoint::draw(mat4 C){
    glUseProgram(shaderProgram);
    modelToWorld = C;
    
    colorLoc = glGetUniformLocation(shaderProgram, "color");
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    modelLoc = glGetUniformLocation(shaderProgram, "model");

    glUniform3fv(colorLoc, 1, glm::value_ptr(rgb));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Window::projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Window::view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Set point size.
    glPointSize(pointSize);
    
    glDrawArrays(GL_POINTS, 0, (GLsizei) 1);
    glBindVertexArray(0);
    

}

