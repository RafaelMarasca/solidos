#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "geometry.h"
#include<vector>
#include<GL/glew.h>


#define ZFAR 100.0f
#define ZNEAR 0.1f

#define FRONT 0 
#define BACK 1
#define TOP 2
#define BOTTOM 3
#define LEFT 4
#define RIGHT 5


class plane
{
    private:
        vec3 normal;
        vec3 point;

    public:
        plane();
        plane(vec3 p0, vec3 p1, vec3 p2);
        GLfloat dist(vec3 point);
        void print();
};


class camera
{
    private:
        matrix viewMatrix;
        matrix projectionMatrix;
        vec3 cameraPos;
        vec3 target;
        vec3 up;
        vec3 dir;
        vec3 right;
        GLfloat fov;
        GLfloat ar;

        plane planes[6];

    public:
        camera(vec3 cameraPos, vec3 target, vec3 upVec, GLfloat fov = 90.0f, GLfloat aspectRatio = 1.0f);
        void lookAt(vec3 cameraPos, vec3 target, vec3 upVec);
        void perspective(GLfloat fov, GLfloat ar = 1.0f);
        matrix getView();
        matrix getProjection();

        void setPos(vec3 cameraPos);
        void setTarget(vec3 target);
        void rotate(GLfloat theta, GLfloat phi);
        void mouseMap(GLfloat dx, GLfloat dy, GLfloat vpHeight, GLfloat vpWidth);

        void zoom(GLfloat fovInc);

        void setupNormals();
        bool isInFrustrum(GLfloat x, GLfloat y, GLfloat z);
        bool isInFrustrum(solid* s);

        void print();


};


#endif //CAMERA_H