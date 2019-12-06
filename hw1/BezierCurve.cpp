//
//  BezierCurve.cpp
//  hw1
//
//  Created by Mingxun Song on 11/7/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "BezierCurve.hpp"
#include "Window.h"
#include <sstream>
#include <cmath>
#include <iostream>

BezierCurve::BezierCurve(GLuint shaderProgram, vector<vec3> temp){

    this->shaderProgram = shaderProgram;
    this->rgb = vec3(0,0,0);
    setUpEverything(temp);
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   
   // Bind to the VAO.
   glBindVertexArray(vao);

   // Bind to the first VBO. We will use it to store the points.
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   // Pass in the data.
    //glBufferData(GL_ARRAY_BUFFER, sizeof(curveVertices), &curveVertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * curveVertices.size(),
        curveVertices.data(), GL_STATIC_DRAW);
   // Enable vertex attribute 0.
   // We will be able to access points through it.
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
   
   // Unbind from the VBO.
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   // Unbind from the VAO.
   glBindVertexArray(0);
}

BezierCurve::~BezierCurve(){}

void BezierCurve::updateControlPoints(vector<vec3> temp){
    setUpEverything(temp);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
     //glBufferData(GL_ARRAY_BUFFER, sizeof(curveVertices), &curveVertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * curveVertices.size(),
         curveVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void BezierCurve::draw(mat4 C){
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
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) curveVertices.size());
    glBindVertexArray(0);
    


}
vec3 BezierCurve::lerp(float t, vector<vec3> controlPoints){
    vec3 p1 = controlPoints[0];
    vec3 p2 = controlPoints[1];
    vec3 p3 = controlPoints[2];
    vec3 p4 = controlPoints[3];
    return (float) pow((1-t), 3) * p1 + 3 * (float) pow((1-t),2) * t * p2 + 3 * (1-t) * (float) pow(t,2) * p3 + (float) pow(t, 3) * p4;
}

void BezierCurve::generateVertices(vector<vec3> controlPoints){
    for(float t = 0; t < 1; t+=0.005){
        curveVertices.push_back(lerp(t, controlPoints));
    }
}

void BezierCurve::setColor(vec3 rgb){
    this->rgb = rgb;
}

vector<vec3> BezierCurve::getAllpoints(){
    return allPoints;
}
vector<int> BezierCurve::getAnchorpoints(){
    return anchorPoints;
}
vector<int> BezierCurve::getCtrlpoints(){
    return ctrlPoints;
}
vector<vec3> BezierCurve::getHandlepoints(){
    return handlePoints;
}
vector<vec3> BezierCurve::getCurveVertices(){
    return curveVertices;
}

void BezierCurve::setUpEverything(vector<vec3> temp){
    
    vector<vec3> controlPoints1 = {temp[0], temp[1], temp[2], temp[3]};
    
    allPoints = temp;
    
    anchorPoints.clear();
    anchorPoints.push_back(0);
    anchorPoints.push_back(3);
    
    ctrlPoints.clear();
    ctrlPoints.push_back(1);
    ctrlPoints.push_back(2);
    
    curveVertices.clear();
    generateVertices(controlPoints1);
    
}
