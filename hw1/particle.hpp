//
//  particle.hpp
//  hw1
//
//  Created by Mingxun Song on 12/8/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>
#include <glm/glm.hpp>
class particle
{

public:
    glm::vec3 velocity;
    glm::vec3 position;
    glm::vec4 color;
    float life;
    const float LIFE = 2.0f;
    particle();
    void reset(glm::vec3 pos);
    void update(float time);
    float randomFloat(float a, float b);
};

#endif /* particle_hpp */
