//
//  Plane.cpp
//  hw1
//
//  Created by Mingxun Song on 10/29/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane(){
    
}

Plane::Plane( vec3 point, vec3 normal, float width, float height){
    this->point = point;
    this->normal = normal;
    this->width = width;
    this->height = height;
}

void Plane::setPoint(vec3 point){
    this->point = point;
}

void Plane::setNormal(vec3 normal){
    this->normal = normalize(normal);
    d = -(dot(this->normal,point));
}

void Plane::setWidth(float width){
    this->width = width;
}
void Plane::setHeight(float height){
    this->height = height;
}
float Plane::getWidth(){
    return width;
}
float Plane::getHeight(){
    return height;
}
vec3& Plane::getPoint(){
    return point;
}
vec3& Plane::getNormal(){
    return normal;
}
float Plane::distance(vec3 p){
    //return dot((p - point), normal);
    return (d + dot(normal, p));

}
void Plane::set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3) {


    vec3 aux1, aux2;

    aux1 = v1 - v2;
    aux2 = v3 - v2;

    normal = cross(aux2 , aux1);

    normal = normalize(normal);
    point = v2;
    d = -(dot(normal, point));
}
