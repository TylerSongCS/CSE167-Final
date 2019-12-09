//
//  particle.cpp
//  hw1
//
//  Created by Mingxun Song on 12/8/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "particle.hpp"

particle::particle(){
    velocity = glm::vec3(0,1,0);
    position = glm::vec3(0,0,0);
    life = randomFloat(0, LIFE);
}
void particle::update(float time){
    life -= time;
    glm::vec3 tempVelocity = velocity * ((rand() % 100)/100.0f);
    position = position + tempVelocity*time + glm::vec3((rand() % 100 - 50)/100.0f, 0, (rand() % 100 - 50)/100.0f)*time;
}
void particle::reset(glm::vec3 pos){
    position = pos;
    life = randomFloat(0, LIFE);
}
float particle::randomFloat(float a, float b){
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
