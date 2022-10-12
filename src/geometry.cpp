
#define _USE_MATH_DEFINES

#include"geometry.h"
#include"matrix.h"
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<vector>
#include<cmath>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Declaração das strings contendo os códigos fonte dos shaders
extern const std::string vertexShaderSource;
extern const std::string fragmentShaderSource;

//Inicialização da constante estática de classe responsável 
//por contar a quantidade de formas na tela
unsigned int geometry::geometryCount = 0;

//Inicialização do ponteiro estático que aponta para o shader gerado
shaderProgram* geometry::program = nullptr;

std::vector<GLfloat> branco = {1.0f, 1.0f, 1.0f, 1.0f};


geometry::geometry(std::vector<GLfloat> &vertices, std::vector<int> &indices, std::vector<GLfloat> &centralPoint, GLenum usage)
{
    this->wireFrame = 0;

    this->modelMatrix = matrix::eye(4);
    this->viewMatrix = matrix::eye(4);
    this->projectionMatrix = matrix::eye(4);// glm::mat4(1.0f);//matrix::eye(4);

    this->vertices = vertices;
    this->indices = indices;
    this->centralPoint = centralPoint;
    this->usage = usage;
    
    this->color = {GEOMETRY_R, GEOMETRY_G, GEOMETRY_B, 1.0f};


    if(geometry::program == nullptr)
        geometry::program = new shaderProgram(vertexShaderSource, fragmentShaderSource);

    this->setup();  //Chama a Função de inicialização.

    geometry::geometryCount++; //Incrementa o contador de formas.
}

geometry::geometry(GLenum usage)
{
    this->wireFrame = 0;

    this->modelMatrix = matrix::eye(4);
    this->viewMatrix = matrix::eye(4);
    this->projectionMatrix = matrix::eye(4);//= glm::mat4(1.0f);//matrix::eye(4);

    this->centralPoint = {0.0f, 0.0f, 0.0f};
    this->usage = usage;
    
    this->color = {GEOMETRY_R, GEOMETRY_G, GEOMETRY_B, 1.0f};


    if(geometry::program == nullptr)
        geometry::program = new shaderProgram(vertexShaderSource, fragmentShaderSource);

    this->setup();  //Chama a Função de inicialização.

    geometry::geometryCount++; //Incrementa o contador de formas.
}



geometry::~geometry()
{
    geometry::geometryCount--; //Decrementa o contador de formas.

    //Se não há mais formas no programa, desaloca a memória para o programa shader.
    if(geometry::geometryCount == 0 && geometry::program != nullptr)
    {
        delete geometry::program; //deleta o programa
        geometry::program = nullptr; //Faz o ponteiro para o shaderProgram apontar para null.
    }
    
    //Libera a memória associadas aos buffers criados para a forma corrente.
    glDeleteBuffers(1,&(this->VBO));
    glDeleteVertexArrays(1,&(this->VAO));
    glDeleteBuffers(1, &(this->EBO));
}

    
void geometry::draw()
{
    this->program->use();
    glBindVertexArray(this->VAO);
    //glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    //glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void geometry::setup()
{
    //Gera os buffers de objeto e array de vértices.
    glGenBuffers(1, &(this->VBO));
    glGenVertexArrays(1,&(this->VAO));
    glGenBuffers(1,&(this->EBO));

    //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices).size()*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);

    //Inicia o uso do programa de shader e seta a cor inicial da forma.
    this->program->use();
    //this->colorLoc = this->program->getLoc("color");
    this->modelLoc = this->program->getLoc("model");
    this->viewLoc = this->program->getLoc("view");
    this->projectionLoc = this->program->getLoc("projection");
    //glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    //glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, glm::value_ptr(this->projectionMatrix));

}


void geometry::rotate(GLfloat degrees, AXIS a)
{
    GLfloat rad = degrees*M_PI/180.0;
    
    std::vector<GLfloat> cPoint = this->centralPoint;

    this->translate(-cPoint[0], -cPoint[1], -cPoint[2]);

    matrix rotation = matrix::eye(4);
    if(a == X_AXIS)
    {
        rotation(1,1) = cos(rad);
        rotation(1,2) = -sin(rad);
        rotation(2,1) = sin(rad);
        rotation(2,2) = cos(rad); 

    }else if (a == Y_AXIS)
    {
        rotation(0,0) = cos(rad);
        rotation(0,2) = sin(rad);
        rotation(2,0) = -sin(rad);
        rotation(2,2) = cos(rad); 

    }else if (a == Z_AXIS)
    {
        rotation(0,0) = cos(rad);
        rotation(0,1) = -sin(rad);
        rotation(1,0) = sin(rad);
        rotation(1,1) = cos(rad); 
    }

    this->modelMatrix = rotation*this->modelMatrix;
    this->translate(cPoint[0], cPoint[1], cPoint[2]);
    for(int i = 0; i<3; i++)
    {
        std::cout<<this->centralPoint[i]<<std::endl;
    }

    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}

void geometry::translate(GLfloat dx, GLfloat dy, GLfloat dz)
{
    matrix translation = matrix::eye(4);
    translation(0,3) = dx;
    translation(1,3) = dy;
    translation(2,3) = dz;
    
    matrix cMatrix = matrix(4,1);
    cMatrix(0,0) = this->centralPoint[0];
    cMatrix(1,0) = this->centralPoint[1];
    cMatrix(2,0) = this->centralPoint[2];
    cMatrix(3,0) = 1.0f;

    this->centralPoint = std::vector<GLfloat>(translation*cMatrix);

    this->modelMatrix = translation*this->modelMatrix;

    for(int i = 0; i<3; i++)
    {
        std::cout<<this->centralPoint[i]<<std::endl;
    }

    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}


void geometry::scale(GLfloat sx, GLfloat sy, GLfloat sz)
{

    matrix scale = matrix::eye(4);
    scale(0,3) = sx;
    scale(1,3) = sy;
    scale(2,3) = sz;

    matrix cMatrix = matrix(4,1);
    cMatrix(0,0) = this->centralPoint[0];
    cMatrix(1,0) = this->centralPoint[1];
    cMatrix(2,0) = this->centralPoint[2];
    cMatrix(3,0) = 1.0f;

    this->centralPoint = std::vector<GLfloat>(scale*cMatrix);
    this->modelMatrix = scale*this->modelMatrix;

    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}



void geometry::setProjection(GLfloat FOV, GLfloat ar)
{
    GLfloat rad = FOV*M_PI/360.0f;
    GLfloat nearZ= 0.1f;
    GLfloat farZ = 100.0f;

    //matrix aux = matrix::eye(4);

    this->projectionMatrix(0,0) = 1.0f/(tan(rad)*ar);
    this->projectionMatrix(1,1) = 1.0f/tan(rad);
    this->projectionMatrix(2,2) = (nearZ + farZ)/(nearZ-farZ);
    this->projectionMatrix(2,3) = 2*nearZ*farZ/(nearZ-farZ);
    this->projectionMatrix(3,2) = -1.0f;
    this->projectionMatrix(3,3) = 0.0f;
    /*aux(0,0) = 1.0f/tan(rad);
    aux(1,1) = 1.0f/tan(rad);
    aux(2,2) = (nearZ + farZ)/(nearZ-farZ);
    aux(2,3) = 2*nearZ*farZ/(nearZ-farZ);
    aux(3,2) = -1.0f;
    aux(3,3) = 0.0f; */  

    /*this->projectionMatrix = glm::perspective(glm::radians(90.0f), 600.0f / 600.0f, 0.1f,
100.0f);*/
    

    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    //glUniformMatrix4fv(this->projectionLoc, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
}

void geometry::setView(matrix &view)
{
    this->viewMatrix = view;
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
}

cube::cube(GLfloat size, std::vector<GLfloat> &center, GLenum usage): geometry(usage)
{

    this->vertices = 
    {
        //Face frontal
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, 1.0f, //Branco
        -1.0f,  1.0f,  1.0f,
         0.0f,  1.0f,  1.0f, 1.0f, //Ciano
        -1.0f, -1.0f,  1.0f,
         0.0f,  0.0f,  1.0f, 1.0f, //Azul
         1.0f, -1.0f,  1.0f, 
         1.0f,  0.0f,  1.0f, 1.0f, //Magenta

        //Face traseira
         1.0f,  1.0f,  -1.0f,
         1.0f,  1.0f,   0.0f, 1.0f, //Amarelo
        -1.0f,  1.0f,  -1.0f,
         0.0f,  1.0f,   0.0f, 1.0f, //Verde
        -1.0f, -1.0f,  -1.0f,
         0.0f,  0.0f,   0.0f, 1.0f, //Preto
         1.0f, -1.0f,  -1.0f,
         1.0f,  0.0f,   0.0f, 1.0f, //Vermelho
    };

    this->indices = 
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
    };

    this->scale(size/2.0f, size/2.0f, size/2.0f);

     //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
}