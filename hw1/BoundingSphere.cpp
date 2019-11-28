//
//  BoundingSphere.cpp
//  hw1
//
//  Created by Mingxun Song on 10/29/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "BoundingSphere.hpp"

BoundingSphere::BoundingSphere(){
    center = vec3(0,0,0);
    radius = 0;
}

void BoundingSphere::setCenter(vec3 c){
    center = c;
}
void BoundingSphere::setRadius(float r){
    radius = r;
}
vec3 BoundingSphere::getCenter(){
    return center;
}
float BoundingSphere::getRadius(){
    return radius;
}
