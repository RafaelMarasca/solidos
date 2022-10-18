
#define _USE_MATH_DEFINES

#include"geometry.h"
#include"matrix.h"
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<vector>
#include<cmath>
#include<iostream>


geometry::geometry(GLenum usage)
{
    this->centralPoint = {0.0f, 0.0f, 0.0f};
    this->color = {GEOMETRY_R, GEOMETRY_G, GEOMETRY_B, 1.0f};
    this->usage = usage;
}

geometry::~geometry(){}

void geometry::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->color[3] = a; 
}

void geometry::resetColor()
{
    this->color[0] = GEOMETRY_R;
    this->color[1] = GEOMETRY_G;
    this->color[2] = GEOMETRY_B;
    this->color[3] = 1.0f; 
}