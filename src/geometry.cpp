
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

bool floatCmp(GLfloat a, GLfloat b)
{
    if(std::fabs(a-b)>0.00001f)
        return false;
    
    return true;
}

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
    glDeleteBuffers(1, &(this->EBO));
    glDeleteVertexArrays(1,&(this->VAO));
}

    
void geometry::draw()
{
    this->program->use();
    glBindVertexArray(this->VAO);
    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
    glUniformMatrix4fv(this->viewLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->viewMatrix)[0]));
    glUniformMatrix4fv(this->projectionLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->projectionMatrix)[0]));
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
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


    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}


void geometry::scale(GLfloat sx, GLfloat sy, GLfloat sz)
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

    glUniformMatrix4fv(this->modelLoc, 1, GL_TRUE, &(((std::vector<GLfloat>)this->modelMatrix)[0]));
}



void geometry::setProjection(matrix projectionMatrix)
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
}

void geometry::resetColor()
{
    this->color[0] = GEOMETRY_R;
    this->color[1] = GEOMETRY_G;
    this->color[2] = GEOMETRY_B;
    this->color[3] = 1.0f; 
}

cube::cube(GLfloat size, std::vector<GLfloat> &center, GLenum usage): geometry(usage)
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
}


icosahedron::icosahedron(GLfloat size, std::vector<GLfloat> &center, GLenum usage): geometry(usage)
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



icosphere::icosphere(GLfloat radius, std::vector<GLfloat> &center, int depth, GLenum usage) : geometry(usage)
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
}

#define HDIV 16
#define VDIV 16
#define HRES 2*M_PI/float(HDIV)
#define VRES 2*M_PI/float(VDIV)

torus::torus(GLfloat discRadius, GLfloat circleRadius, std::vector<GLfloat> &center, GLenum usage): geometry(usage)
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

}
