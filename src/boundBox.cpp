#include "boundBox.h"


extern const std::string boxVertexShader;
extern const std::string boxFragmentShader;

shaderProgram* boundBox::program = 0;
int boundBox::boxCount  = 0;


boundBox::boundBox() : geometry(GL_DYNAMIC_DRAW)
{
    this->modelMatrix = matrix::eye(4);
    this->viewMatrix = matrix::eye(4);
    this->projectionMatrix = matrix::eye(4);

    if(boundBox::program == nullptr)
        boundBox::program = new shaderProgram(boxVertexShader, boxFragmentShader);

    glGenBuffers(1, &(this->VBO));
    glGenVertexArrays(1,&(this->VAO));
    glGenBuffers(1,&(this->EBO));

    this->colorLoc = this->program->getLoc("color");
    this->modelLoc = this->program->getLoc("model");
    this->viewLoc = this->program->getLoc("view");
    this->projectionLoc = this->program->getLoc("projection");

    this->vertices = std::vector<GLfloat> (24,0.0f);
    this->indices = std::vector<int> (36,0);
    
    this->setColor(BB_R,BB_G,BB_B);

    this->setupBuffers();

    boundBox::boxCount++; //Incrementa o contador de formas.
}

boundBox::~boundBox()
{
    boundBox::boxCount--;
    //Se não há mais formas no programa, desaloca a memória para o programa shader.
    if(boundBox::boxCount == 0 && boundBox::program != nullptr)
    {
        delete boundBox::program; //deleta o programa
        boundBox::program = nullptr; //Faz o ponteiro para o shaderProgram apontar para null.
    }
    
    //Libera a memória associadas aos buffers criados para a forma corrente.
    glDeleteBuffers(1,&(this->VBO));
    glDeleteBuffers(1, &(this->EBO));
    glDeleteVertexArrays(1,&(this->VAO));
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
    this->updateBuffers();
}

void boundBox::setupBuffers()
{
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size())*sizeof(GLfloat), &(this->vertices[0]), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices.size())*sizeof(int), &(this->indices[0]), usage);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}


void boundBox::updateBuffers()
{
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (this->vertices.size())*sizeof(GLfloat), &(this->vertices[0]));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (this->indices.size())*sizeof(int), &(this->indices[0]));

    glBindVertexArray(0);
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glUseProgram(0);
}

void boundBox::setProjection(const matrix &projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
}

void boundBox::setView(const matrix &viewMatrix)
{
    this->viewMatrix = viewMatrix;
}