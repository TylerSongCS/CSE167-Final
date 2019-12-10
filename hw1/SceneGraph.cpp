//
//  SceneGraph.cpp
//  hw1
//
//  Created by Mingxun Song on 10/25/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//
#include "objparser.hpp"
#include "SceneGraph.hpp"
#include "Window.h"
#include <sstream>
#include <cmath>
#include <iostream>

Transform::Transform(){}

Transform::Transform(mat4 m){
    M = m;
    numChildren = 0;
    center = vec3(0,0,0);
}

Transform::~Transform(){}

void Transform::addNode(Node * child){
    childNodes.push_back(child);
}

void Transform::draw(mat4 C){
    if(shouldRender){
        mat4 M_new = C * M;
        for( int i = 0; i < childNodes.size(); i++ ){
            childNodes[i]->draw(M_new);
        }
    }
}
void Transform::update(){}

void Transform::scale(vec3 factor){
    M = glm::scale(M, factor);
    
}
void Transform::translate(vec3 factor){
    M = glm::translate(M, factor);
    center = center + factor;
}
void Transform::rotate(float degree, vec3 axis){
    M = glm::rotate(mat4(1.0f), degree, axis) * M;
}
void Transform::animate(){
    rotate(radians( (float)-1*(animations.back()/10)), vec3(0,1,0));
    rotate(radians( (float)(animations.front()/10)), vec3(0,1,0));
    animations.push_back(animations.front());
    animations.pop_front();
}
void Transform::setAnimations(int options){
    int degree;
    bool increment;
    if(options == 0){
        degree = 0;
        increment = false;
        for(int i = 0; i < 600; i++){
            animations.push_back(degree);
            
            if(degree  <= -300){
                increment = true;
            }else if( degree >= 300){
                increment = false;
            }
            if(increment){
                degree += 2;
            }else{
                degree -= 2;
            }
        }
    }else if(options == 1){
        degree = 0;
        increment = true;
        for(int i = 0; i < 600; i++){
            animations.push_back(degree);
            
            if(degree  <= -300){
                increment = true;
            }else if( degree >= 300){
                increment = false;
            }
            if(increment){
                degree += 2;
            }else{
                degree -= 2;
            }
        }
        
    }
}
Geometry::Geometry(){}

Geometry::Geometry(string objFilename,GLuint shaderProgram){
    if(objParse(objFilename, vertices, normals)){
        cerr << "obj parse successful" << endl;
    }else{
        cerr << objFilename << " parse failed" << endl;
    }
    // Set the model matrix to an identity matrix.
    modelToWorld = glm::mat4(1);
    
    //Set geometry shader program
    this->shaderProgram = shaderProgram;
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &colorbuffer);
    glGenBuffers(1, &normalbuffer);
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        vertices.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    //Bind to color buffer
    /*glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                  // attribute
        3,                                  // size
        GL_FLOAT,                           // type
        GL_FALSE,                           // normalized?
        3 * sizeof(GLfloat),
        0                                   // array buffer offset
    );*/
    
    //Bind to normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                                  // attribute
        3,                                  // size
        GL_FLOAT,                           // type
        GL_FALSE,                           // normalized?
        3 * sizeof(GLfloat),
        0                                   // array buffer offset
    );

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the colorbuffer.
    //glBindBuffer(GL_ARRAY_BUFFER, 1);
    // Unbind from the normalbuffer.
    glBindBuffer(GL_ARRAY_BUFFER, 2);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    
}

Geometry::~Geometry(){
    
    // Delete the VBO's and the VAO.
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteVertexArrays(1, &vao);
}

void Geometry::draw(mat4 C){
    if(!shouldRender){
        return;
    }
    glUseProgram(shaderProgram);
    modelToWorld = C;
    cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    modelLoc = glGetUniformLocation(shaderProgram, "model");
    ambLoc = glGetUniformLocation(shaderProgram, "material.ambient");
    diffLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
    specLoc = glGetUniformLocation(shaderProgram, "material.specular");
    shinLoc = glGetUniformLocation(shaderProgram, "material.shininess");
    ambientStrengthLoc = glGetUniformLocation(shaderProgram, "ambientStrength");
    diffuseStrengthLoc = glGetUniformLocation(shaderProgram, "diffuseStrength");
    specularStrengthLoc = glGetUniformLocation(shaderProgram, "specularStrength");
    isPhongLoc = glGetUniformLocation(shaderProgram, "isPhong");
    colorLoc = glGetUniformLocation(shaderProgram, "color");
    
    glUniform3fv(cameraPosLoc, 1, glm::value_ptr(Window::eye));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Window::projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Window::view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    glUniform3fv(ambLoc, 1, glm::value_ptr(ambient));
    glUniform3fv(diffLoc, 1, glm::value_ptr(diffuse));
    glUniform3fv(specLoc, 1, glm::value_ptr(specular));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    glUniform1f(shinLoc, shininess);
    glUniform1f(ambientStrengthLoc, ambientStrength);
    glUniform1f(diffuseStrengthLoc, diffuseStrength);
    glUniform1f(specularStrengthLoc, specularStrength);
    glUniform1i(isPhongLoc, isPhong);
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Draw points
    if(isBoundingSphere){
        glDrawArrays(GL_LINES, 0, (GLsizei) vertices.size());
    }else{
        if(cubemapTexture != 0){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        }
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    }
    
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Geometry::update(){}

void Geometry::scale(vec3 factor){
    modelToWorld =  glm::scale(modelToWorld , factor);
}
void Geometry::translate(vec3 factor){
    modelToWorld = glm::translate(mat4(1.0f), factor) * modelToWorld;
}
void Geometry::rotate(float degree, vec3 axis){
    modelToWorld = glm::rotate(mat4(1.0f), degree, axis) * modelToWorld;
}
mat4 Geometry::getModel(){
    return modelToWorld;
}

void Geometry::setMaterial(vec3 amb, float ambStrength, vec3 diff,
                 float diffStrength, vec3 spec, float specStrength,
                 float shin ){
    ambient = amb;
    ambientStrength = ambStrength;
    diffuse = diff;
    diffuseStrength = diffStrength;
    specular = spec;
    specularStrength = specStrength;
    shininess = shin;
    
}
void Geometry::setRenderMode(float renderMode){
    isPhong = renderMode;
}

void Geometry::setIsBoundingSpere(bool b){
    isBoundingSphere = b;
}
void Geometry::setColor(vec3 rgb){
    color = rgb;
}
