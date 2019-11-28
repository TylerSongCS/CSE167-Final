//
//  Frustum.cpp
//  hw1
//
//  Created by Mingxun Song on 10/29/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//  Credit to Gerlits Anatoliy of https://www.gamedev.net/articles/programming/general-and-gameplay-programming/frustum-culling-r4613/

#include "Frustum.hpp"
#include <iostream>

Frustum::Frustum(){
    
}
Frustum::Frustum(float fovy, float aspect, float znear, float zfar){
    this->fovy = fovy;
    this->aspect = aspect;
    this->znear = znear;
    this->zfar = zfar;
    projection = glm::perspective(glm::radians(fovy),aspect, znear, zfar);
}
mat4& Frustum::getProjection(){
    return projection;
}
float Frustum::getFOVY(){
    return fovy;
}
float Frustum::getAspect(){
    return aspect;
}
float Frustum::getZNear(){
    return znear;
}
float Frustum::getZFar(){
    return zfar;
}
void Frustum::setFOVY(float fovy){
    this->fovy = fovy;
}
void Frustum::createPlanes(vec3 eye, vec3 center, vec3 up, Plane* plane){
    //cerr << "fov: " << fovy << endl;
    float tang = (float) tan(radians(fovy) / 2);
    float nearHeight = znear * tang;
    float nearWidth = nearHeight * aspect;
    float farHeight = zfar * tang;
    float farWidth = farHeight * aspect;
    
    vec3 farCenter, nearCenter, X, Y, Z, ynh, xnw, nearTopLeft, nearTopRight, nearBotLeft, nearBotRight, farTopLeft, farTopRight, farBotLeft, farBotRight;
    
    // opposite direction of the looking direction
    Z = normalize(eye - center);
    
    //x axis of camera
    X = normalize(up * Z);
    
    //the real up vector
    Y = cross(Z, X);
    //Y = (Z * X);
    /*if( Y == (Z * X)){
        std::cerr << "true";
    }else{
        std::cerr << "false";
    }*/
    
    ynh = Y * (nearHeight);
    xnw = X * (nearWidth);
    
    //center of far and near planes
    nearCenter = eye - Z * znear;
    farCenter = eye - Z * zfar;
    
    //4 corners of the frustum on near plane
    nearTopLeft = nearCenter + Y * nearHeight - X * nearWidth;
    nearTopRight = nearCenter + Y * nearHeight + X * nearWidth;
    nearBotLeft = nearCenter - Y * nearHeight - X * nearWidth;
    nearBotRight = nearCenter - Y * nearHeight + X * nearWidth;
    
    //4 corners of the frustum on far plane
    farTopLeft = farCenter + Y * farHeight - X * farWidth;
    farTopRight = farCenter + Y * farHeight + X * farWidth;
    farBotLeft = farCenter - Y * farHeight - X *  farWidth;
    farBotRight = farCenter - Y * farHeight + X * farWidth;
    
    plane[TOP].set3Points(nearTopRight,nearTopLeft,farTopLeft);
    plane[BOTTOM].set3Points(nearBotLeft,nearBotRight,farBotRight);
    plane[LEFT].set3Points(nearTopLeft,nearBotLeft,farBotLeft);
    plane[RIGHT].set3Points(nearBotRight,nearTopRight,farBotRight);
    //plane[NEAR].set3Points(nearTopLeft,nearTopRight,nearBotRight);
    //plane[FAR].set3Points(farTopRight,farTopLeft,farBotLeft);
    vec3 aux, normal;
    
    /*plane[NEAR].setNormal(-Z);
    plane[NEAR].setPoint(nearCenter);
    
    plane[FAR].setNormal(Z);
    plane[FAR].setPoint(farCenter);
    
    aux = nearCenter + ynh;
    plane[TOP].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = cross(aux, X);
    plane[TOP].setNormal(normal);
    
    aux = nearCenter - ynh;
    plane[BOTTOM].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = cross(X, aux);
    plane[BOTTOM].setNormal(normal);
    
    aux = nearCenter - xnw;
    plane[LEFT].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = cross(aux, Y);
    plane[LEFT].setNormal(normal);
    
    aux = nearCenter + xnw;
    plane[RIGHT].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = cross( Y, aux);
    plane[RIGHT].setNormal(normal);
    
    /*plane[NEAR].setNormal(-Z);
    plane[NEAR].setPoint(nearCenter);
    
    plane[FAR].setNormal(Z);
    plane[FAR].setPoint(farCenter);
    
    aux = nearCenter + ynh;
    plane[TOP].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = aux * X;
    plane[TOP].setNormal(normal);
    
    aux = nearCenter - ynh;
    plane[BOTTOM].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = X * aux;
    plane[BOTTOM].setNormal(normal);
    
    aux = nearCenter - xnw;
    plane[LEFT].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = aux * Y;
    plane[LEFT].setNormal(normal);
    
    aux = nearCenter + xnw;
    plane[RIGHT].setPoint(aux);
    
    aux = normalize(aux - eye);
    normal = Y * aux;
    plane[RIGHT].setNormal(normal);*/
    
}

void Frustum::normalizePlane(vec4 &frustum_plane)
{
    float magnitude = (float)sqrt(frustum_plane[A] * frustum_plane[A] + frustum_plane[B] * frustum_plane[B] + frustum_plane[C] * frustum_plane[C]);
    frustum_plane[A] /= magnitude;
    frustum_plane[B] /= magnitude;
    frustum_plane[C] /= magnitude;
    frustum_plane[D] /= magnitude;
}

void Frustum::CalculateFrustum(mat4 &view_matrix, mat4 &proj_matrix)
{
    float   *proj = glm::value_ptr(proj_matrix);
    float   *modl = glm::value_ptr(view_matrix);
    float   clip[16]; //clipping planes

    clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
    clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
    clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
    clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

    clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
    clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
    clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
    clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

    clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
    clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
    clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

    frustum_planes[RIGHT][A] = clip[3] - clip[0];
    frustum_planes[RIGHT][B] = clip[7] - clip[4];
    frustum_planes[RIGHT][C] = clip[11] - clip[8];
    frustum_planes[RIGHT][D] = clip[15] - clip[12];
    normalizePlane(frustum_planes[RIGHT]);

    frustum_planes[LEFT][A] = clip[3] + clip[0];
    frustum_planes[LEFT][B] = clip[7] + clip[4];
    frustum_planes[LEFT][C] = clip[11] + clip[8];
    frustum_planes[LEFT][D] = clip[15] + clip[12];
    normalizePlane(frustum_planes[LEFT]);

    frustum_planes[BOTTOM][A] = clip[3] + clip[1];
    frustum_planes[BOTTOM][B] = clip[7] + clip[5];
    frustum_planes[BOTTOM][C] = clip[11] + clip[9];
    frustum_planes[BOTTOM][D] = clip[15] + clip[13];
    normalizePlane(frustum_planes[BOTTOM]);

    frustum_planes[TOP][A] = clip[3] - clip[1];
    frustum_planes[TOP][B] = clip[7] - clip[5];
    frustum_planes[TOP][C] = clip[11] - clip[9];
    frustum_planes[TOP][D] = clip[15] - clip[13];
    normalizePlane(frustum_planes[TOP]);

    frustum_planes[BACK][A] = clip[3] - clip[2];
    frustum_planes[BACK][B] = clip[7] - clip[6];
    frustum_planes[BACK][C] = clip[11] - clip[10];
    frustum_planes[BACK][D] = clip[15] - clip[14];
    normalizePlane(frustum_planes[BACK]);

    frustum_planes[FRONT][A] = clip[3] + clip[2];
    frustum_planes[FRONT][B] = clip[7] + clip[6];
    frustum_planes[FRONT][C] = clip[11] + clip[10];
    frustum_planes[FRONT][D] = clip[15] + clip[14];
    normalizePlane(frustum_planes[FRONT]);
}

bool Frustum::SphereInFrustum(vec3 &pos, float &radius)
{
    bool res = true;
    //test all 6 frustum planes
    for (int i = 0; i < 6; i++)
    {
        //calculate distance from sphere center to plane.
        //if distance larger then sphere radius - sphere is outside frustum
        if (frustum_planes[i].x * pos.x + frustum_planes[i].y * pos.y + frustum_planes[i].z * pos.z + frustum_planes[i].w <= -radius)
            //res = false;
            return false; //with flag works faster
    }
    return res;
    //return true;
}


