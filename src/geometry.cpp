
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


int floatCmp(GLfloat a, GLfloat b)
{
    if(std::fabs(a-b)>0.00001f)
    {
        if(a-b > 0.0f)
            return 1;
        else if (a-b < 0.0f)
            return -1;
    }
    
    return 0;
}

geometry::geometry(std::vector<GLfloat> &vertices, std::vector<int> &indices, std::vector<GLfloat> &centralPoint, GLenum usage)
{

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
    glDeleteBuffers(1, &(this->EBO));
    glDeleteVertexArrays(1,&(this->VAO));
}

    
void solid::draw()
{
    if(isBoundBox)
        this->box->draw();

    this->program->use();
    glBindVertexArray(this->VAO);
    
    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));

    if(this->isSolid)
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 2.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);

        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if(this->isWireFrame)
    {
        glUniform4fv(this->colorLoc, 1, &(this->wireFrameColor[0]));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    glBindVertexArray(this->VAO);
    glUseProgram(0);
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
    this->colorLoc = this->program->getLoc("color");
    this->modelLoc = this->program->getLoc("model");
    this->viewLoc = this->program->getLoc("view");
    this->projectionLoc = this->program->getLoc("projection");
    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    //glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, glm::value_ptr(this->projectionMatrix));

}

void solid::setProjection(const matrix &projectionMatrix)
{
    geometry::setProjection(projectionMatrix);
    this->box->setProjection(projectionMatrix);
}

void solid::setView(const matrix &viewMatrix)
{
    geometry::setView(viewMatrix);
    this->box->setView(viewMatrix);
}


void solid::rotate(GLfloat degrees, AXIS a)
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

    //this->setupCollisionBox();
    this->box->update(this->vertices, this->centralPoint,this->modelMatrix);


    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}

void solid::translate(GLfloat dx, GLfloat dy, GLfloat dz)
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
    //this->setupCollisionBox();
     this->box->update(this->vertices, this->centralPoint, this->modelMatrix);

    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}


void solid::scale(GLfloat sx, GLfloat sy, GLfloat sz)
{

    matrix scale = matrix::eye(4);
    scale(0,0) = sx;
    scale(1,1) = sy;
    scale(2,2) = sz;

    matrix cMatrix = matrix(4,1);
    cMatrix(0,0) = this->centralPoint[0];
    cMatrix(1,0) = this->centralPoint[1];
    cMatrix(2,0) = this->centralPoint[2];
    cMatrix(3,0) = 1.0f;

    this->centralPoint = std::vector<GLfloat>(scale*cMatrix);
    this->modelMatrix = scale*this->modelMatrix;
    //this->setupCollisionBox();
     this->box->update(this->vertices, this->centralPoint ,this->modelMatrix);

    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}



void geometry::setProjection(const matrix &projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
    

    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    //glUniformMatrix4fv(this->projectionLoc, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
}

void geometry::setView(const matrix &view)
{
    this->viewMatrix = view;
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
}


void geometry::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->color[3] = a; 

    //this->wireFrameColor[0] = r;
    //this->wireFrameColor[1] = g;
    //this->wireFrameColor[2] = b;
    //this->wireFrameColor[3] = a; 
}

void solid::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->color[3] = a; 

    this->wireFrameColor[0] = r;
    this->wireFrameColor[1] = g;
    this->wireFrameColor[2] = b;
    this->wireFrameColor[3] = a; 
}

void geometry::resetColor()
{
    this->color[0] = GEOMETRY_R;
    this->color[1] = GEOMETRY_G;
    this->color[2] = GEOMETRY_B;
    this->color[3] = 1.0f; 

    //this->wireFrameColor[0] = WF_R;
    //this->wireFrameColor[1] = WF_G;
    //this->wireFrameColor[2] = WF_B;
    //this->wireFrameColor[3] = 1.0f; 
}

void solid::resetColor()
{
    this->color[0] = GEOMETRY_R;
    this->color[1] = GEOMETRY_G;
    this->color[2] = GEOMETRY_B;
    this->color[3] = 1.0f; 

    this->wireFrameColor[0] = WF_R;
    this->wireFrameColor[1] = WF_G;
    this->wireFrameColor[2] = WF_B;
    this->wireFrameColor[3] = 1.0f; 
}

solid::solid(std::vector<GLfloat> &vertices, std::vector<int> &indices, std::vector<GLfloat> &centralPoint, GLenum usage):geometry(vertices, indices, centralPoint, usage)
{
    this->isWireFrame = 0;
    this->isSolid = 1;
    this->isBoundBox = 0;
    this->wireFrameColor = {WF_R, WF_G, WF_B, 1.0f};
    this->box = new boundBox();
};

solid::solid(GLenum usage):geometry(usage)
{
    this->isWireFrame = 0;
    this->isSolid = 1;
    this->isBoundBox = 0;
    this->wireFrameColor = {WF_R, WF_G, WF_B, 1.0f};
    this->box = new boundBox();
    
}

solid::~solid()
{
    delete this->box;
}

cube::cube(GLfloat size, std::vector<GLfloat> &center, GLenum usage): solid(usage)
{

    this->vertices = 
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
    };

    /*
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
    };*/

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
    this->translate(center[0], center[1], center[2]);

     //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
   // glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
     this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
}


icosahedron::icosahedron(GLfloat size, std::vector<GLfloat> &center, GLenum usage): solid(usage)
{
    GLfloat gr = (1+sqrt(5))/2.0f;

    this->vertices = 
    {
         0.0f,   gr,  1.0f, //0
         0.0f,   gr, -1.0f, //1
         0.0f,  -gr,  1.0f, //2
         0.0f,  -gr, -1.0f, //3
         1.0f,  0.0f,   gr, //4
        -1.0f,  0.0f,   gr, //5
         1.0f,  0.0f,  -gr, //6
        -1.0f,  0.0f,  -gr, //7
           gr,  1.0f, 0.0f, //8
           gr, -1.0f, 0.0f, //9
          -gr,  1.0f, 0.0f, //10
          -gr, -1.0f, 0.0f  //11
    };

    this->indices = 
    {
         0,  5,  4, //1 v
         0,  4,  8, //2 v
         0,  8,  1, //3 v
         0,  1, 10, //4 v
         0, 10,  5, //5 v 
         5,  2,  4, //6 v 
         4,  2,  9, //7 v 
         9,  6,  8, //8 v
         3,  7,  6, //9 v
         7, 10,  1, //10 v
        10, 11,  5, //11 v
         5, 11,  2, //12 v
         2,  3,  9, //13  v
         2, 11,  3, //14 v
         3,  11, 7, //15 v
         1,  6,  7, //16 v
         1,  8,  6, //17 v
         6,  9,  3, //18 v
         4,  9,  8, //19 v
         7, 11,  10 //20 v
    };

    this->scale(size/2.0f, size/2.0f, size/2.0f);
    this->translate(center[0], center[1], center[2]);

     //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
     this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
}




std::vector<int> triangleDivision(std::vector<int>::iterator it, std::vector<GLfloat> &vertices, std::vector<bool> &visited)
{
    
    GLfloat radius = 1.902f;
    std::vector<int> newTriangles;
    vec3 v1 (vertices[(*it)*3], vertices[((*it)*3)+1],vertices[((*it)*3)+2]);
    vec3 v2 (vertices[(*(it+1))*3], vertices[((*(it+1))*3)+1],vertices[((*(it+1))*3)+2]);
    vec3 v3 (vertices[(*(it+2))*3], vertices[((*(it+2))*3)+1],vertices[((*(it+2))*3)+2]);

    

    vec3 v12 = (((v2 + v1)).normalize()*radius); //v12
    vec3 v23 = (((v3 + v2)).normalize()*radius); //v23
    vec3 v31 = (((v1 + v3)).normalize()*radius); //v31

    
    int index1 = 0;
    int index2 = 0;
    int index3 = 0;


    /*if(visited[(*it)] && visited[*(it+1)])
    {
        for(int i = 0; i< vertices.size(); i+=3)
        {
            if(floatCmp(vertices[i], v12(0,0))&&floatCmp(vertices[i+1], v12(1,0)) && floatCmp(vertices[i+2], v12(2,0)))
            {
                index1 = i/3;
                break;
            }
        }
    }*/

    if(!index1)
    {
        index1 = vertices.size()/3;
        for(int i = 0; i<3; i++)
            vertices.push_back(v12(i));
        visited.push_back(1);
    }

    /*if(visited[*(it+1)] && visited[*(it+2)])
    {
        for(int i = 0; i< vertices.size(); i+=3)
        {
            if(floatCmp(vertices[i], v23(0,0))&&floatCmp(vertices[i+1], v23(1,0)) && floatCmp(vertices[i+2], v23(2,0)))
            {
                index2 = i/3;
                break;
            }
        }
    }*/
    if(!index2)
    {
        index2 = vertices.size()/3;
        for(int i = 0; i<3; i++)
            vertices.push_back(v23(i));
        visited.push_back(1);
    }

    /*if(visited[*(it+2)] && visited[*(it)])
    {
        for(int i = 0; i< vertices.size(); i+=3)
        {
            if(floatCmp(vertices[i], v31(0,0)) && floatCmp(vertices[i+1], v31(1,0)) && floatCmp(vertices[i+2], v31(2,0)))
            {
                index3 = i/3;
                break;
            }
        }
    }*/
    
    if(!index3)
    {
        index3 = vertices.size()/3;
        for(int i = 0; i<3; i++)
            vertices.push_back(v31(i));
        visited.push_back(1);
    }
    
    newTriangles.push_back(*it);
    newTriangles.push_back(index1);
    newTriangles.push_back(index3);

    newTriangles.push_back(index1);
    newTriangles.push_back(*(it+1));
    newTriangles.push_back(index2);

    newTriangles.push_back(index1);
    newTriangles.push_back(index2);
    newTriangles.push_back(index3);

    newTriangles.push_back(index2);
    newTriangles.push_back(*(it+2));
    newTriangles.push_back(index3);

    visited[*it] = true;
    visited[*(it+1)] = true;
    visited[*(it+2)] = true;

  
    return newTriangles;
}

void icosphere::subdivide(int depth)
{
    while(depth)
    {
        std::vector<bool> visited (this->vertices.size()/3, false);
        std::vector<int> newTriangles;

        std::vector<int>::iterator it = this->indices.begin();

        for(it = this->indices.begin(); it != this->indices.end(); it+=3)
        {
            std::vector<int> aux = triangleDivision(it, this->vertices, visited);
            newTriangles.insert(newTriangles.end(),aux.begin(), aux.end());
        }

        indices = newTriangles;
        depth--;
    }

    //Seleciona o array de vértices da forma corrente.
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



icosphere::icosphere(GLfloat radius, std::vector<GLfloat> &center, int depth, GLenum usage) : solid(usage)
{

    GLfloat gr = (1+sqrt(5))/2.0f;
    GLfloat scaleFactor = sqrt(gr*gr +1);

    this->vertices = 
    {
         0.0f,   gr,  1.0f, //0
         0.0f,   gr, -1.0f, //1
         0.0f,  -gr,  1.0f, //2
         0.0f,  -gr, -1.0f, //3
         1.0f,  0.0f,   gr, //4
        -1.0f,  0.0f,   gr, //5
         1.0f,  0.0f,  -gr, //6
        -1.0f,  0.0f,  -gr, //7
           gr,  1.0f, 0.0f, //8
           gr, -1.0f, 0.0f, //9
          -gr,  1.0f, 0.0f, //10
          -gr, -1.0f, 0.0f  //11
    };

    this->indices = 
    {
         0,  5,  4, //1 v
         0,  4,  8, //2 v
         0,  8,  1, //3 v
         0,  1, 10, //4 v
         0, 10,  5, //5 v 
         5,  2,  4, //6 v 
         4,  2,  9, //7 v 
         9,  6,  8, //8 v
         3,  7,  6, //9 v
         7, 10,  1, //10 v
        10, 11,  5, //11 v
         5, 11,  2, //12 v
         2,  3,  9, //13  v
         2, 11,  3, //14 v
         3,  11, 7, //15 v
         1,  6,  7, //16 v
         1,  8,  6, //17 v
         6,  9,  3, //18 v
         4,  9,  8, //19 v
         7, 11,  10 //20 v
    };

    this->subdivide(depth);

    this->scale(radius/scaleFactor, radius/scaleFactor, radius/scaleFactor);
    this->translate(center[0], center[1], center[2]);

     this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
}

#define HDIV 16
#define VDIV 16
#define HRES 2*M_PI/float(HDIV)
#define VRES 2*M_PI/float(VDIV)

torus::torus(GLfloat discRadius, GLfloat circleRadius, std::vector<GLfloat> &center, GLenum usage): solid(usage)
{
    GLfloat theta = 0.0f;
    GLfloat phi = 0.0f;
    GLfloat r = discRadius;
    GLfloat R = circleRadius;
    GLfloat x,y,z;
    //x = (r*cos(phi)+R)*cos(theta)
    //z = (r*cos(phi)+R)*sin(theta)
    //y = r*sin(phi)    
    std::vector<int> tmpIndex;
    for(int j = 0; j<VDIV; j++)
    {
        for(int i = 0; i<HDIV; i++)
        {
            theta = i*HRES;
            phi = j*VRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);

            tmpIndex.push_back(this->vertices.size()/3);
            
            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);
            
            theta = (1+i)*HRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);
            
            tmpIndex.push_back(this->vertices.size()/3);

            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            theta = i*HRES;
            phi = (j+1)*VRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);
            
            tmpIndex.push_back(this->vertices.size()/3);
            
            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            theta = (i+1)*HRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);
            
            tmpIndex.push_back(this->vertices.size()/3);
            
            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            tmpIndex.push_back(tmpIndex[2]);
            tmpIndex.push_back(tmpIndex[1]);

            this->indices.insert(this->indices.end(), tmpIndex.begin(), tmpIndex.end());
            tmpIndex.clear();
        }
    }

    this->translate(center[0], center[1], center[2]);

    //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);

     this->box->update(this->vertices, this->centralPoint ,this->modelMatrix);
}

void solid::setWireFrame(bool state)
{
    this->isWireFrame = state;
}

void solid::setSolid(bool state)
{
    this->isSolid = state;
}

bool solid::getWireFrameState()
{
    return this->isWireFrame;
}

bool solid::getSolidState()
{
    return this->isSolid;
}

bool solid::collision(GLfloat x, GLfloat y, GLfloat z)
{
    vec3 max = this->box->getMax();
    vec3 min = this->box->getMin();

    return(x>=min(0) && x<=max(0) 
            && y>=min(1) && y<=max(1) 
            && z>=min(2) && z<=max(2));
}

bool solid::collision(solid* other)
{

    vec3 max = this->box->getMax();
    vec3 min = this->box->getMin();
    vec3 otherMax = other->box->getMax();
    vec3 otherMin = other->box->getMin();


    return (min(0) <= otherMax(0)
            && min(1) <= otherMax(1)
            && min(2) <= otherMax(2)
            && max(0) >= otherMin(0)
            && max(1) >= otherMin(1)
            && max(2) >= otherMin(2));
}


bool geometry::collision(GLfloat x, GLfloat y, GLfloat z){return false;}
bool geometry::collision(geometry* other){return false;}


hexahedron::hexahedron(GLfloat xSize, GLfloat ySize, GLfloat zSize, std::vector<GLfloat> &center, GLenum usage) : solid(usage)
{
    this->vertices = 
    {
        //Face frontal
         xSize,  ySize,  zSize,
        -xSize,  ySize,  zSize,
        -xSize, -ySize,  zSize,
         xSize, -ySize,  zSize, 

        //Face traseira
         xSize,  ySize,  -zSize,
        -xSize,  ySize,  -zSize,
        -xSize, -ySize,  -zSize,
         xSize, -ySize,  -zSize,
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

    this->scale(0.5f, 0.5f, 0.5f);
    this->translate(center[0], center[1], center[2]);

     //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
   // glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
}

void geometry::print()
{
    std::cout<<"RT ";
    for(int i = 0; i<3; i++)
    {
         //std::cout<<this->collisionRT(i,0)<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"LB ";
     for(int i = 0; i<3; i++)
    {
        //std::cout<<this->collisionLB(i,0)<<" ";
    }
    std::cout<<std::endl;
}


boundBox::boundBox() :geometry(GL_DYNAMIC_DRAW)
{

    this->vertices = std::vector<GLfloat> (24,0.0f);
    this->indices = std::vector<int> (36,0);
    this->setColor(BB_R,BB_G,BB_B);

    //glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat), NULL, usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), NULL, usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
}

void boundBox::updateVertices()
{
    GLfloat xSize, ySize, zSize;
    xSize = (this->max(0) - this->min(0))/2.0f;
    ySize = (this->max(1) - this->min(1))/2.0f;
    zSize = (this->max(2) - this->min(2))/2.0f;

    this->vertices = 
    {
        //Face frontal
         xSize,  ySize,  zSize,
        -xSize,  ySize,  zSize,
        -xSize, -ySize,  zSize,
         xSize, -ySize,  zSize, 

        //Face traseira
         xSize,  ySize,  -zSize,
        -xSize,  ySize,  -zSize,
        -xSize, -ySize,  -zSize,
         xSize, -ySize,  -zSize,
    };

    for(int i = 0; i<this->vertices.size(); i+=3)
    {
        vertices[i] += centralPoint[0];
        vertices[i+1] += centralPoint[1];
        vertices[i+2] += centralPoint[2];
    }

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

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (this->vertices.size())*sizeof(GLfloat), &(this->vertices[0]));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (this->indices.size())*sizeof(int), &(this->indices[0]));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
}

void boundBox::update(std::vector<GLfloat> &v, std::vector<GLfloat> &center,matrix &model)
{
    GLfloat minX, maxX, minY, maxY, minZ, maxZ; 
    std::vector<GLfloat>::iterator it;

    maxX = maxY = maxZ = std::numeric_limits<float>::lowest();
    minX = minY = minZ = std::numeric_limits<float>::max();

    matrix aux(4,1);

    for(it = v.begin(); it!= v.end(); it+=3)
    {
        aux(0,0) = *it;
        aux(1,0) = *(it+1);
        aux(2,0) = *(it+2);
        aux(3,0) = 1.0f;

        aux = model*aux;

        if(aux(0,0)<minX)
        {
            minX = aux(0,0);
        }
        if(aux(0,0)>maxX)
        {
            maxX = aux(0,0);
        }

        if(aux(1,0)<minY)
        {
            minY = aux(1,0);
        }
        if(aux(1,0)>maxY)
        {
            maxY = aux(1,0);
        }

        if(aux(2,0)<minZ)
        {
            minZ = aux(2,0);
        }
        
        if(aux(2,0)>maxZ)
        {
            maxZ = aux(2,0);
        }
    }

    this->centralPoint[0] = center[0];
    this->centralPoint[1] = center[1];
    this->centralPoint[2] = center[2];
  

    this->min(0) = minX;
    this->min(1) = minY;
    this->min(2) = minZ;

    this->max(0) = maxX;
    this->max(1) = maxY;
    this->max(2) = maxZ;

    this->updateVertices();
}

vec3 boundBox::getMax()
{
    return this->max;
}

vec3 boundBox::getMin()
{
    return this->min;
}


void boundBox::draw()
{
    this->program->use();
    glBindVertexArray(this->VAO);
    
    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));

    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool solid::getBoundBoxState()
{
    return this->isBoundBox;
}

void solid::setBoundBox(bool state)
{
    this->isBoundBox = state;
}

boundBox* solid::getBoundBox()
{
    return this->box;
}