#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include<vector>
#include<GL/glew.h>

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

};


#endif //CAMERA_H