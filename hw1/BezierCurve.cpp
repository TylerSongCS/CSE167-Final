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
    
    temp[3] = (temp[4] + temp[2]) * (0.5f);
    temp[6] = (temp[7] + temp[5]) * (0.5f);
    temp[9] = (temp[10] + temp[8]) * (0.5f);
    temp[12] = (temp[13] + temp[11]) * (0.5f);
    temp[15] = (temp[16] + temp[14]) * (0.5f);
    temp[18] = (temp[19] + temp[17]) * (0.5f);
    temp[21] = (temp[22] + temp[20]) * (0.5f);
    temp[0] = (temp[1] + temp[23]) * (0.5f);
    
    
    vector<vec3> controlPoints1 = {temp[0], temp[1], temp[2], temp[3]};
    vector<vec3> controlPoints2 = {temp[3], temp[4], temp[5], temp[6]};
    vector<vec3> controlPoints3 = {temp[6], temp[7], temp[8], temp[9]};
    vector<vec3> controlPoints4 = {temp[9], temp[10], temp[11], temp[12]};
    vector<vec3> controlPoints5 = {temp[12], temp[13], temp[14], temp[15]};
    vector<vec3> controlPoints6 = {temp[15], temp[16], temp[17], temp[18]};
    vector<vec3> controlPoints7 = {temp[18], temp[19], temp[20], temp[21]};
    vector<vec3> controlPoints8 = {temp[21], temp[22], temp[23], temp[0]};
    
    allPoints = temp;
    
    anchorPoints.clear();
    anchorPoints.push_back(0);
    anchorPoints.push_back(3);
    anchorPoints.push_back(6);
    anchorPoints.push_back(9);
    anchorPoints.push_back(12);
    anchorPoints.push_back(15);
    anchorPoints.push_back(18);
    anchorPoints.push_back(21);
    
    ctrlPoints.clear();
    ctrlPoints.push_back(1);
    ctrlPoints.push_back(2);
    ctrlPoints.push_back(4);
    ctrlPoints.push_back(5);
    ctrlPoints.push_back(7);
    ctrlPoints.push_back(8);
    ctrlPoints.push_back(10);
    ctrlPoints.push_back(11);
    ctrlPoints.push_back(13);
    ctrlPoints.push_back(14);
    ctrlPoints.push_back(16);
    ctrlPoints.push_back(17);
    ctrlPoints.push_back(19);
    ctrlPoints.push_back(20);
    ctrlPoints.push_back(22);
    ctrlPoints.push_back(23);

    handlePoints.clear();
    handlePoints.push_back(temp[2]);
    handlePoints.push_back(temp[4]);
    handlePoints.push_back(temp[5]);
    handlePoints.push_back(temp[7]);
    handlePoints.push_back(temp[8]);
    handlePoints.push_back(temp[10]);
    handlePoints.push_back(temp[11]);
    handlePoints.push_back(temp[13]);
    handlePoints.push_back(temp[14]);
    handlePoints.push_back(temp[16]);
    handlePoints.push_back(temp[17]);
    handlePoints.push_back(temp[19]);
    handlePoints.push_back(temp[20]);
    handlePoints.push_back(temp[22]);
    handlePoints.push_back(temp[23]);
    handlePoints.push_back(temp[1]);
    
    curveVertices.clear();
    generateVertices(controlPoints1);
    generateVertices(controlPoints2);
    generateVertices(controlPoints3);
    generateVertices(controlPoints4);
    generateVertices(controlPoints5);
    generateVertices(controlPoints6);
    generateVertices(controlPoints7);
    generateVertices(controlPoints8);
    
}
