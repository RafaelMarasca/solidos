#define _USE_MATH_DEFINES

#include "camera.h"
#include "matrix.h"
#include <cmath>
#include<iostream>

camera::camera(vec3 cameraPos, vec3 target, vec3 upVec, GLfloat fov, GLfloat aspectRatio)
{
    this->lookAt(cameraPos,target, upVec);
    this->perspective(fov, aspectRatio);
    this->setupNormals();
}

void camera::lookAt(vec3 cameraPos, vec3 target, vec3 upVec)
{
    this->viewMatrix = matrix::eye(4);

    this->cameraPos = cameraPos;
    this->target = target;


    this-> dir = (cameraPos-target).normalize();
    this-> right = (vec3::crossProduct(upVec, dir)).normalize();
    this->up = vec3::crossProduct(dir,right).normalize();


    this->viewMatrix(0,0) = right(0);
    this->viewMatrix(0,1) = right(1);
    this->viewMatrix(0,2) = right(2);
    this->viewMatrix(1,0) = up(0);
    this->viewMatrix(1,1) = up(1);
    this->viewMatrix(1,2) = up(2);
    this->viewMatrix(2,0) = dir(0);
    this->viewMatrix(2,1) = dir(1);
    this->viewMatrix(2,2) = dir(2);


    this->viewMatrix(0,3) = vec3::dotProduct(right, cameraPos);
    this->viewMatrix(1,3) = vec3::dotProduct(up, cameraPos);
    this->viewMatrix(2,3) = -vec3::dotProduct(dir, cameraPos);

    this->setupNormals();
}

matrix camera::getView()
{
    return this->viewMatrix;
}

void camera::setPos(vec3 cameraPos)
{
    this->lookAt(cameraPos, this->target, vec3(0.0f,1.0f,0.0f));
}

void camera::setTarget(vec3 target)
{
    this->lookAt(this->cameraPos, target, vec3(0.0f,1.0f,0.0f));
}

void camera::rotate(GLfloat theta, GLfloat phi)
{
    theta = theta*M_PI/180.0f;
    phi = phi*M_PI/180.0f;

    vec3 newPos;

    vec3 upv(0.0f, 1.0f, 0.0f);
    
    matrix rot = matrix::eye(4);
    matrix aux = matrix(4,1);
    aux(0,0) = this->cameraPos(0);
    aux(1,0) = this->cameraPos(1);
    aux(2,0) = this->cameraPos(2);
    aux(3,0) = 1;

    {
        rot(0,0) = cos(theta) + (upv(0)*upv(0))*(1-cos(theta));
        rot(0,1) = upv(0)*upv(1)*(1-cos(theta)) - upv(2)*sin(theta);
        rot(0,2) = upv(0)*upv(2)*(1-cos(theta)) + upv(1)*sin(theta); 
        rot(1,0) = upv(0)*upv(1)*(1-cos(theta)) + upv(2)*sin(theta);
        rot(1,1) = cos(theta) + (upv(1)*upv(1))*(1-cos(theta));
        rot(1,2) = upv(0)*upv(2)*(1-cos(theta)) - upv(0)*sin(theta);
        rot(2,0) = upv(0)*upv(2)*(1-cos(theta)) - upv(1)*sin(theta);
        rot(2,1) = upv(0)*upv(2)*(1-cos(theta)) + upv(0)*sin(theta);
        rot(2,2) = cos(theta) + (upv(2)*upv(2))*(1-cos(theta));

        aux = rot*aux;
    }

    
        rot = matrix::eye(4);

        rot(0,0) = cos(phi) + (right(0)*right(0))*(1-cos(phi));
        rot(0,1) = right(0)*right(1)*(1-cos(phi)) - right(2)*sin(phi);
        rot(0,2) = right(0)*right(2)*(1-cos(phi)) + right(1)*sin(phi); 
        rot(1,0) = right(0)*right(1)*(1-cos(phi)) + right(2)*sin(phi);
        rot(1,1) = cos(phi) + (right(1)*right(1))*(1-cos(phi));
        rot(1,2) = right(0)*right(2)*(1-cos(phi)) - right(0)*sin(phi);
        rot(2,0) = right(0)*right(2)*(1-cos(phi)) - right(1)*sin(phi);
        rot(2,1) = right(0)*right(2)*(1-cos(phi)) + right(0)*sin(phi);
        rot(2,2) = cos(phi) + (right(2)*right(2))*(1-cos(phi));

        aux = rot*aux;
   

    
    newPos(0) = aux(0,0);
    newPos(1) = aux(1,0);
    newPos(2) = aux(2,0);

    //this->lookAt = this->lookAt*aux;
    this->lookAt(newPos, this->target, upv);

}


void camera::mouseMap(GLfloat dx, GLfloat dy, GLfloat vpHeight, GLfloat vpWidth)
{
    GLfloat dTheta = 360.0f/vpWidth;
    GLfloat dPhi = 180.0f/vpHeight;
    this->rotate(dTheta*dx, dPhi*dy);
}

void camera::perspective(GLfloat FOV, GLfloat ar)
{
    GLfloat rad = FOV*M_PI/360.0f;

    this->fov = FOV;
    this->ar = ar;

    this->projectionMatrix = matrix::eye(4);

    this->projectionMatrix(0,0) = 1.0f/(tan(rad)*ar);
    this->projectionMatrix(1,1) = 1.0f/tan(rad);
    this->projectionMatrix(2,2) = (ZNEAR + ZFAR)/(ZNEAR-ZFAR);
    this->projectionMatrix(2,3) = 2*ZNEAR*ZFAR/(ZNEAR-ZFAR);
    this->projectionMatrix(3,2) = -1.0f;
    this->projectionMatrix(3,3) = 0.0f;

    this->setupNormals();
}

matrix camera::getProjection()
{
    return this->projectionMatrix;
}


void camera::zoom(GLfloat fovInc)
{
    if(fov + fovInc >150 || fov + fovInc <30)
        return;

    this->fov += fovInc;

    this->perspective(fov, this->ar);
}


void camera::setupNormals()
{
    matrix norm(3,6);
    GLfloat rad = (this->fov)*M_PI/180.0f;

    GLfloat hFar, wFar, hNear, wNear;
    vec3 farTR, farTL, farBR, farBL, nearTR, nearTL, nearBR, nearBL,cNear, cFar;

    hNear = 2.0f*ZNEAR*tan(rad/2.0f);
    hFar = 2.0f* ZFAR*tan(rad/2.0f);

    wNear = 2.0f*ZNEAR*tan(rad/2.0f)*ar; //ar = 1
    wFar = 2.0f* ZFAR*tan(rad/2.0f)*ar; //ar = 1

    cFar = cameraPos - dir*ZFAR;
    cNear = cameraPos - dir*ZNEAR; 

    farTL = cFar + (up*(hFar/2.0f)) - (right * (wFar/2.0f));
    farTR = cFar + (up*(hFar/2.0f)) + (right * (wFar/2.0f));
    farBL = cFar - (up*(hFar/2.0f)) - (right * (wFar/2.0f));
    farBR = cFar - (up*(hFar/2.0f)) + (right * (wFar/2.0f));

    nearTL = cNear + (up*(hNear/2.0f)) - (right * (wNear/2.0f));
    nearTR = cNear + (up*(hNear/2.0f)) + (right * (wNear/2.0f));
    nearBL = cNear - (up*(hNear/2.0f)) - (right * (wNear/2.0f));
    nearBR = cNear - (up*(hNear/2.0f)) + (right * (wNear/2.0f));

    //Face Frontal
    this->planes[FRONT] = (plane(nearTR, nearTL, nearBL));

    //Face Traseira
    this->planes[BACK] = (plane(farTL, farTR, farBR));

    //Face Direita
    this->planes[RIGHT] = (plane(nearTR, nearBR, farBR));
   
    //Face Esquerda
    this->planes[LEFT] = (plane(nearBL, nearTL, farTL));
    
    //Face Superior
    this->planes[TOP] = (plane(nearTL, nearTR, farTL));
   
    //Face Inferior
    this->planes[BOTTOM] = (plane(nearBR, nearBL, farBL));
}


bool camera::isInFrustrum(GLfloat x, GLfloat y, GLfloat z)
{
    for(int i = 0; i<6; i++)
    {
        if(this->planes[i].dist(vec3(x,y,z))<0)
        {
            return false;
        }
    }

    return true;
}

bool camera::isInFrustrum(solid* s)
{
    vec3 max = s->getBoundBox()->getMax();
    vec3 min = s->getBoundBox()->getMin();
    
    for(int i = 0; i<6; i++)
    {
        if(this->planes[i].dist(max)<0 || this->planes[i].dist(min)<0)
            return false;
    }
    return true;
}


void camera::print()
{
    for(int i = 0; i<6; i++)
    {
        this->planes[i].print();
    }
}

vec3& camera::getCameraPos()
{
    return this->cameraPos;
}


plane::plane(vec3 p0, vec3 p1, vec3 p2)
{
    vec3 aux1 = p0-p1;
    vec3 aux2 = p2-p1;

    this->normal = vec3::crossProduct(aux1,aux2).normalize();
    this->point = p1;
}

plane::plane()
{

    this->normal = vec3(0,0,0);
    this->point = vec3(0,0,0);
}

GLfloat plane::dist(vec3 point)
{
    return vec3::dotProduct(this->normal, point) - vec3::dotProduct(this->normal,this->point);
}

void plane::print()
{
    std::cout<<this->normal(0)<<" "<<this->normal(1)<<" "<<this->normal(2)<<" "<<std::endl;
}


void camera::setAspectRatio(GLfloat ar)
{
    this->perspective(fov, ar);
}