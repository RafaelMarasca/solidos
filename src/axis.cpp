/**
 * @file axis.cpp
 * @author Rafael Marasca Martins
 * @brief Implementação da classe axis
 * @date 2022-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "axis.h"
#include "geometry.h"
#include<iostream>
#include<GL/glew.h>

#define AXIS_TICK_SPACING 0.1f //Espaçamento entre ticks
#define PLANE_COLOR_R 0.4f
#define PLANE_COLOR_G 0.4f
#define PLANE_COLOR_B 0.4f



//Construtor para os eixos
axis::axis() : geometry(GL_STATIC_DRAW)
{
    this->viewMatrix = matrix::eye(4);
    this->projectionMatrix = matrix::eye(4);
    this->color = {1.0f, 0.0f, 0.0f, 1.0f};
    
    program = new shaderProgram(axisVertexShader, axisFragmentShader);

    glGenBuffers(1, &(this->VBO));
    glGenVertexArrays(1,&(this->VAO));
    glGenBuffers(1,&(this->EBO));

    //this->colorLoc = this->program->getLoc("color");
    this->viewLoc = this->program->getLoc("view");
    this->projectionLoc = this->program->getLoc("projection");

    this->setup();
}


//Configura os pontos do eixo
void axis::setup()
{
    this->vertices = 
    {
        -1.0f,   0.0f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,   0.0f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
          0.0f, -1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
          0.0f,  1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
          0.0f,   0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
          0.0f,   0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };

    this->indices = 
    {
        0, 1,
        2, 3,
        4, 5
    };

    GLfloat initX, initZ;
    initX = initZ = -10*AXIS_TICK_SPACING;

    int tempIndex[3] = {0,0,0};

    for(int i = 0; i <=20; i++)
    {
        if(i == 10)
            continue;
        //X AXIS
        tempIndex[0] = this->vertices.size()/7;

        this->vertices.push_back(initX + i*AXIS_TICK_SPACING);
        this->vertices.push_back(0.0f);
        this->vertices.push_back(initZ);

        this->vertices.push_back(PLANE_COLOR_R);
        this->vertices.push_back(PLANE_COLOR_G);
        this->vertices.push_back(PLANE_COLOR_B);
        this->vertices.push_back(1.0f);

        tempIndex[1] = this->vertices.size()/7;
    
        this->vertices.push_back(initX + i*AXIS_TICK_SPACING);
        this->vertices.push_back(0.0f);
        this->vertices.push_back(initZ + 20*AXIS_TICK_SPACING);

        this->vertices.push_back(PLANE_COLOR_R);
        this->vertices.push_back(PLANE_COLOR_G);
        this->vertices.push_back(PLANE_COLOR_B);
        this->vertices.push_back(1.0f);

        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[1]);

        //Z AXIS
        tempIndex[0] = this->vertices.size()/7;

        this->vertices.push_back(initX);
        this->vertices.push_back(0.0f);
        this->vertices.push_back(initZ + i*AXIS_TICK_SPACING);

        this->vertices.push_back(PLANE_COLOR_R);
        this->vertices.push_back(PLANE_COLOR_G);
        this->vertices.push_back(PLANE_COLOR_B);
        this->vertices.push_back(1.0f);

        tempIndex[1] = this->vertices.size()/7;
    
        this->vertices.push_back(initX + 20*AXIS_TICK_SPACING);
        this->vertices.push_back(0.0f);
        this->vertices.push_back(initZ + i*AXIS_TICK_SPACING);

        this->vertices.push_back(PLANE_COLOR_R);
        this->vertices.push_back(PLANE_COLOR_G);
        this->vertices.push_back(PLANE_COLOR_B);
        this->vertices.push_back(1.0f);
    
        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[1]);
    }

    this->setupBuffers();
}


//Seta os buffers
void axis::setupBuffers()
{
    //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

//Método de desnho para o eixo
void axis::draw()
{
    this->program->use();
    
    glBindVertexArray(this->VAO);
    
    //glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    
    glDrawElements(GL_LINES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
    glUseProgram(0);
}

//Destrutor para o eixo
axis::~axis()
{
    delete this->program;

    glDeleteBuffers(1,&(this->VBO));
    glDeleteBuffers(1, &(this->EBO));
    glDeleteVertexArrays(1,&(this->VAO));
}


//Seta a matriz de projeção
void axis::setProjection(const matrix &projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
}

//Seta a matriz view
void axis::setView(const matrix &viewMatrix)
{
    this->viewMatrix = viewMatrix;
}