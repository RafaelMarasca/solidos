#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include<vector>
#include<GL/glew.h>

class camera
{
    private:
        matrix lookAt;
        vec3 cameraPos;
        vec3 target;
        vec3 up;
        vec3 dir;
        vec3 right;

    public:
        camera(vec3 cameraPos, vec3 target, vec3 upVec);
        void setLookAt(vec3 cameraPos, vec3 target, vec3 upVec);
        matrix getLookAt();

        void setPos(vec3 cameraPos);
        void setTarget(vec3 target);
        void rotate(GLfloat theta, GLfloat phi);
        void mouseMap(GLfloat dx, GLfloat dy, GLfloat vpHeight, GLfloat vpWidth);

};


#endif //CAMERA_H