#define _USE_MATH_DEFINES

#include "camera.h"
#include "matrix.h"
#include <cmath>

camera::camera(vec3 cameraPos, vec3 target, vec3 upVec, GLfloat fov, GLfloat aspectRatio)
{
    this->lookAt(cameraPos,target, upVec);
    this->perspective(fov, aspectRatio);
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
}

matrix camera::getView()
{
    return this->viewMatrix;
}

void camera::setPos(vec3 cameraPos)
{
    this->lookAt(cameraPos, this->target, this->up);
}

void camera::setTarget(vec3 target)
{
    this->lookAt(this->cameraPos, target, this->up);
}

void camera::rotate(GLfloat theta, GLfloat phi)
{
    theta = theta*M_PI/180.0f;
    phi = phi*M_PI/180.0f;

    vec3 newPos;
    
    matrix rot = matrix::eye(4);
    matrix aux = matrix(4,1);
    aux(0,0) = this->cameraPos(0);
    aux(1,0) = this->cameraPos(1);
    aux(2,0) = this->cameraPos(2);
    aux(3,0) = 1;

    //if()
    {
        rot(0,0) = cos(theta) + (this->up(0)*this->up(0))*(1-cos(theta));
        rot(0,1) = this->up(0)*this->up(1)*(1-cos(theta)) - this->up(2)*sin(theta);
        rot(0,2) = this->up(0)*this->up(2)*(1-cos(theta)) + this->up(1)*sin(theta); 
        rot(1,0) = this->up(0)*this->up(1)*(1-cos(theta)) + this->up(2)*sin(theta);
        rot(1,1) = cos(theta) + (this->up(1)*this->up(1))*(1-cos(theta));
        rot(1,2) = this->up(0)*this->up(2)*(1-cos(theta)) - this->up(0)*sin(theta);
        rot(2,0) = this->up(0)*this->up(2)*(1-cos(theta)) - this->up(1)*sin(theta);
        rot(2,1) = this->up(0)*this->up(2)*(1-cos(theta)) + this->up(0)*sin(theta);
        rot(2,2) = cos(theta) + (this->up(2)*this->up(2))*(1-cos(theta));

        aux = rot*aux;
    }

        
    vec3 upv(0.0f, 1.0f, 0.0f);

    if(!(std::abs(vec3::dotProduct(dir, upv))>0.999f))
    {
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
    }

    
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
    GLfloat nearZ= 0.1f;
    GLfloat farZ = 100.0f;

    this->fov = FOV;
    this->ar = ar;

    this->projectionMatrix = matrix::eye(4);

    this->projectionMatrix(0,0) = 1.0f/(tan(rad)*ar);
    this->projectionMatrix(1,1) = 1.0f/tan(rad);
    this->projectionMatrix(2,2) = (nearZ + farZ)/(nearZ-farZ);
    this->projectionMatrix(2,3) = 2*nearZ*farZ/(nearZ-farZ);
    this->projectionMatrix(3,2) = -1.0f;
    this->projectionMatrix(3,3) = 0.0f;
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