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
#include<GL/glew.h>

#define AXIS_TICK_SPACING 0.1f //Espaçamento entre ticks


axis::axis():geometry(GL_STATIC_DRAW)
{

     /*this->vertices = 
    {
        //Face frontal
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f, 

        //Face traseira
         1.0f,  1.0f,  -1.0f,
        -1.0f,  1.0f,  -1.0f,
        -1.0f, -1.0f,  -1.0f,
         1.0f, -1.0f,  -1.0f,
    };*/

    GLfloat initX, initY, initZ;
    initX = initY = initZ = -10*AXIS_TICK_SPACING;

    this->vertices.push_back(initX);
    this->vertices.push_back(initY);
    this->vertices.push_back(initZ);

    this->vertices.push_back(initX + 20*AXIS_TICK_SPACING);
    this->vertices.push_back(initY);
    this->vertices.push_back(initZ);

    this->vertices.push_back(initX);
    this->vertices.push_back(initY + 20*AXIS_TICK_SPACING);
    this->vertices.push_back(initZ);

    this->vertices.push_back(initX);
    this->vertices.push_back(initY);
    this->vertices.push_back(initZ + 20*AXIS_TICK_SPACING);

    this->indices = {0,1,
                     0,2,
                     0,3};

    int tempIndex[3] = {0,0,0};

    for(int i = 1; i <=20; i++)
    {
        //X AXIS
        tempIndex[0] = this->vertices.size()/3;

        this->vertices.push_back(initX + i*AXIS_TICK_SPACING);
        this->vertices.push_back(initY);
        this->vertices.push_back(initZ);

        tempIndex[1] = this->vertices.size()/3;
    
        this->vertices.push_back(initX + i*AXIS_TICK_SPACING);
        this->vertices.push_back(initY);
        this->vertices.push_back(initZ + 20*AXIS_TICK_SPACING);

        
        tempIndex[2] = this->vertices.size()/3;

        this->vertices.push_back(initX + i*AXIS_TICK_SPACING);
        this->vertices.push_back(initY + 20*AXIS_TICK_SPACING);
        this->vertices.push_back(initZ);

        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[1]);
        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[2]);

        //Y AXIS
        tempIndex[0] = this->vertices.size()/3;

        this->vertices.push_back(initX);
        this->vertices.push_back(initY + i*AXIS_TICK_SPACING);
        this->vertices.push_back(initZ);

        tempIndex[1] = this->vertices.size()/3;
    
        this->vertices.push_back(initX);
        this->vertices.push_back(initY + i*AXIS_TICK_SPACING);
        this->vertices.push_back(initZ + 20*AXIS_TICK_SPACING);

        
        tempIndex[2] = this->vertices.size()/3;

        this->vertices.push_back(initX + 20*AXIS_TICK_SPACING);
        this->vertices.push_back(initY + i*AXIS_TICK_SPACING);
        this->vertices.push_back(initZ);

        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[1]);
        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[2]);

        //Z AXIS
        tempIndex[0] = this->vertices.size()/3;

        this->vertices.push_back(initX);
        this->vertices.push_back(initY);
        this->vertices.push_back(initZ + i*AXIS_TICK_SPACING);

        tempIndex[1] = this->vertices.size()/3;
    
        this->vertices.push_back(initX + 20*AXIS_TICK_SPACING);
        this->vertices.push_back(initY);
        this->vertices.push_back(initZ + i*AXIS_TICK_SPACING);

        
        tempIndex[2] = this->vertices.size()/3;

        this->vertices.push_back(initX);
        this->vertices.push_back(initY + 20*AXIS_TICK_SPACING);
        this->vertices.push_back(initZ + i*AXIS_TICK_SPACING);
    
        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[1]);
        this->indices.push_back(tempIndex[0]);
        this->indices.push_back(tempIndex[2]);
    }

     /*this->indices = 
    {
        //Face frontal
        0, 1, 2,
        2, 3, 0,

        //Face direita
        0, 3, 7,
        7, 4, 0,

        //Face Traseira
        4, 7, 6,
        6, 5, 4,

        //Face Esquerda
        5, 6, 2,
        2, 1, 5,

        //Face Superior
        5, 1, 0,
        0, 4, 5,

        //Face Inferior
        2, 6, 7,
        7, 3, 2
    };*/

    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    
}

void axis::draw()
{
    this->program->use();
    glBindVertexArray(this->VAO);
    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    glDrawElements(GL_LINES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(this->VAO);
    glUseProgram(0);
}