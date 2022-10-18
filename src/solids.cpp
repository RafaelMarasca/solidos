#include "solids.h"
#include<cmath>
#include<iostream>


extern const std::string solidVertexShader;
extern const std::string solidFragmentShader;

shaderProgram* solid::program = 0;
int solid::solidCount  = 0;


solid::solid() : geometry(GL_STATIC_DRAW)
{
    this->modelMatrix = matrix::eye(4);
    this->viewMatrix = matrix::eye(4);
    this->projectionMatrix = matrix::eye(4);
    this->cameraPos = {0.0f, 0.0f, 3.0f}; //CameraPos
    this->wireFrameColor = {WF_R, WF_G, WF_B, 1.0f};

    if(solid::program == nullptr)
        solid::program = new shaderProgram(solidVertexShader, solidFragmentShader);

    glGenBuffers(1, &(this->positionVBO));
    glGenBuffers(1, &(this->normalVBO));
    glGenVertexArrays(1,&(this->VAO));
    glGenBuffers(1,&(this->EBO));

    this->lightPosLoc = this->program->getLoc("lightPos");
    this->colorLoc = this->program->getLoc("color");
    this->modelLoc = this->program->getLoc("model");
    this->viewLoc = this->program->getLoc("view");
    this->projectionLoc = this->program->getLoc("projection");


    this->isWireFrame = 0;
    this->isSolid = 1;
    this->isBoundBox = 0;
    this->box = new boundBox();

    solid::solidCount++; //Incrementa o contador de formas.
}


void solid::setupBuffers()
{
    //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->positionVBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat),&(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);


    glBindBuffer(GL_ARRAY_BUFFER, this->normalVBO);
    glBufferData(GL_ARRAY_BUFFER, (this->normals.size())*sizeof(GLfloat), &(this->normals[0]), usage);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


solid::~solid()
{
    solid::solidCount--;

    delete this->box;

    //Se não há mais formas no programa, desaloca a memória para o programa shader.
    if(solid::solidCount == 0 && solid::program != nullptr)
    {
        delete solid::program; //deleta o programa
        solid::program = nullptr; //Faz o ponteiro para o shaderProgram apontar para null.
    }
    
    //Libera a memória associadas aos buffers criados para a forma corrente.
    glDeleteBuffers(1,&(this->positionVBO));
    glDeleteBuffers(1,&(this->normalVBO));
    glDeleteBuffers(1, &(this->EBO));
    glDeleteVertexArrays(1,&(this->VAO));
}

void solid::draw()
{
    if(isBoundBox)
        this->box->draw();

    this->program->use();

    glBindVertexArray(this->VAO);
    
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glUniform3fv(this->lightPosLoc, 1, &(this->cameraPos[0]));
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
        glUniform3fv(this->colorLoc, 1, &(this->wireFrameColor[0]));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    glBindVertexArray(0);
    glUseProgram(0);
}



/*************************************************************************/

cube::cube(GLfloat size, std::vector<GLfloat> &center)
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

    this->generateNormals();

    this->setupBuffers();
    
    this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
}



icosahedron::icosahedron(GLfloat size, std::vector<GLfloat> &center)
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

    this->generateNormals();

    this->setupBuffers();

    this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
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
}


icosphere::icosphere(GLfloat radius, std::vector<GLfloat> &center, int depth)
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

    this->generateNormals();

    this->setupBuffers();

    this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
}


torus::torus(GLfloat discRadius, GLfloat circleRadius, std::vector<GLfloat> &center)
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
            /*theta = i*HRES;
            phi = j*VRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);

            tmpIndex.push_back(this->vertices.size()/3);
            
            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            phi = (j+1)*VRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);
            
            tmpIndex.push_back(this->vertices.size()/3);
            
            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);
            
            phi = j*VRES;
            theta = (1+i)*HRES;

            x = (r*cos(phi)+R)*cos(theta);
            y =  r*sin(phi);
            z = (r*cos(phi)+R)*sin(theta);
            
            tmpIndex.push_back(this->vertices.size()/3);

            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);
    
            phi = (j+1)*VRES;

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
            tmpIndex.clear();*/
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

    this->generateNormals();

    this->setupBuffers();

    this->box->update(this->vertices, this->centralPoint ,this->modelMatrix);
}



/*************************************************************/

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

    if(!index1)
    {
        index1 = vertices.size()/3;
        for(int i = 0; i<3; i++)
            vertices.push_back(v12(i));
        visited.push_back(1);
    }

    if(!index2)
    {
        index2 = vertices.size()/3;
        for(int i = 0; i<3; i++)
            vertices.push_back(v23(i));
        visited.push_back(1);
    }
    
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



hexahedron::hexahedron(GLfloat xSize, GLfloat ySize, GLfloat zSize, std::vector<GLfloat> &center)
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

    this->generateNormals();

    this->setupBuffers();
}

/********************************************************/

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

/******************************************************/

void solid::setProjection(const matrix &projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
    this->box->setProjection(projectionMatrix);
}

void solid::setView(const matrix &viewMatrix)
{
    this->viewMatrix = viewMatrix;
    this->box->setView(viewMatrix);
}

void solid::setCameraPos(vec3 &cameraPos)
{
    this->cameraPos = {cameraPos(0),cameraPos(1),cameraPos(2)};
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

    this->box->update(this->vertices, this->centralPoint,this->modelMatrix);
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

    this->box->update(this->vertices, this->centralPoint, this->modelMatrix);
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
    
     this->box->update(this->vertices, this->centralPoint ,this->modelMatrix);
}

/******************************************************/

void solid::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    geometry::setColor(r,g,b,a);

    this->wireFrameColor[0] = r;
    this->wireFrameColor[1] = g;
    this->wireFrameColor[2] = b;
    this->wireFrameColor[3] = a;
}

void solid::resetColor()
{
    geometry::resetColor();

    this->wireFrameColor[0] = WF_R;
    this->wireFrameColor[1] = WF_G;
    this->wireFrameColor[2] = WF_B;
    this->wireFrameColor[3] = 1.0f; 
}

/*******************************************************/

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


void solid::generateNormals()
{
    std::vector<int>::iterator it;
    std::vector<GLfloat>::iterator nit;
    vec3 v1;
    vec3 v2;
    vec3 v3;
    vec3 normal;

    std::vector<int> visited(this->vertices.size(),0);
    this->normals = std::vector<GLfloat> (this->vertices.size(),0.0f);

    for(it = this->indices.begin(); it!= this->indices.end(); it+=3)
    {
        v1(0) = this->vertices[*it*3];
        v1(1) = this->vertices[(*it)*3+1];
        v1(2) = this->vertices[(*it)*3+2];

        v2(0) = this->vertices[(*(it+1))*3];
        v2(1) = this->vertices[(*(it+1))*3+1];
        v2(2) = this->vertices[(*(it+1))*3+2];

        v3(0) = this->vertices[(*(it+2))*3];
        v3(1) = this->vertices[(*(it+2))*3+1];
        v3(2) = this->vertices[(*(it+2))*3+2];

        normal = vec3::crossProduct(v3-v1,v2-v1);

        this->normals[(*it)*3] += normal(0);
        this->normals[((*it)*3)+1] += normal(1);
        this->normals[((*it)*3)+2] += normal(2);

        this->normals[(*(it+1))*3] += normal(0);
        this->normals[(*(it+1))*3+1] += normal(1);
        this->normals[(*(it+1))*3+2] += normal(2);
        
        this->normals[(*(it+2))*3] += normal(0);
        this->normals[((*(it+2))*3)+1] += normal(1);
        this->normals[((*(it+2)))*3+2] += normal(2);

        visited[(*it)*3] += 1;
        visited[((*it)*3)+1] += 1;
        visited[((*it)*3)+2] += 1;

        visited[(*(it+1))*3] += 1;
        visited[(*(it+1))*3+1] += 1;
        visited[(*(it+1))*3+2] += 1;
        
        visited[(*(it+2))*3] += 1;
        visited[((*(it+2))*3)+1] += 1;
        visited[((*(it+2)))*3+2] += 1;
    }

    for(unsigned i = 0; i<this->normals.size(); i++)
    {
        normals[i]/= float(visited[i]);
    }
}
