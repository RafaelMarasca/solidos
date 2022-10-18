#ifndef GEOMETRY_H
#define GEOMETRY_H


#include"matrix.h"
#include"shaderProgram.h"
#include<GL/glew.h>
#include<cstddef>
#include<vector>
#include<string>
#include<glm/glm.hpp>



//Cor inicial da Forma
#define GEOMETRY_R 0.329f
#define GEOMETRY_G 0.341f
#define GEOMETRY_B 0.486f 


class geometry
{
    protected:
        std::vector<GLfloat> centralPoint; //Ponto central da forma
        std::vector<GLfloat> vertices; //Vetor de vértices da forma
        std::vector<GLfloat> color;
    
        GLenum usage;        

    public:

        geometry(GLenum usage = GL_STATIC_DRAW);
        virtual ~geometry();
       
        virtual void setColor(GLfloat r, GLfloat g,  GLfloat b, GLfloat a = 1.0f);
        virtual void resetColor();

        virtual void draw() = 0; 

        virtual void setupBuffers() = 0;
};

#endif //GEOMETRY_H