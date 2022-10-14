#include"guiGeometry.h"


/**
 * @brief Recebe um ponto (x,y,z) e verifica se há colisão entre a forma e o ponto.
 * 
 * @param x : coordenada x do ponto.
 * @param y : coordenada y do ponto.
 * @param z : coordenada z do ponto.
 * @return true : Se há uma colisão entre um dos pontos de controle e o ponto x,y,z.
 * @return false : Caso não haja colisão entre o ponto x,y,z e algum ponto de controle.
 */
bool square::collision(GLfloat x, GLfloat y, GLfloat z)
{
    std::vector<GLfloat>::iterator it; //Iterator para um vector de GLfloat
    int i = 1; //Variável auxiliar

    //Inicializa o vetor que representa os valores mínimos e máximos das coordenadas x e y da forma
    std::vector<GLfloat> min_max={std::numeric_limits<float>::max(),std::numeric_limits<float>::lowest(),
                                 std::numeric_limits<float>::max(),std::numeric_limits<float>::lowest()};
                                 
    //Verifica se há colisão entre o ponto e os vértices                                 
    for(it = this->vertices.begin(); it!= this->vertices.end(); it+=3)
    {
       if((std::abs(*it - x) < 0.0000001f ) && (std::abs(*(it+1) - y) < 0.000001f ))
        return true;
    }
                             
    //Obtém os pontos mínimos e máximos das coordenadas x e y e armazena no vetor min_max
    for(it = this->vertices.begin(); it!= this->vertices.end(); it++)
    {
        if(i%3 == 1)
        {
            if(*it<min_max[0])
                min_max[0] = *it;
            if(*it>min_max[1])
                min_max[1] = *it;
        }else if(i%3 == 2)
        {
            if(*it<min_max[2])
                min_max[2] = *it;
            if(*it>min_max[3])
                min_max[3] = *it;
        }   
        i++;
    }

    //verifica se o ponto se encontra dentro dos limites do vetor min_max
    if(x>min_max[0] && x<min_max[1] && y>min_max[2] && y<min_max[3])
    {
        //Sim, o ponto se encontra dentro dos limites(colisão).
        return true;
    }else{
        //Não o ponto não se encontra dentro dos limites (i.e. não há colisão).
        return false;
    }         
}


/**
 * @brief Verifica colisão entre o quadrado e outra forma
 * 
 * @param other : a forma a se verificar colisão.
 * @return true : Caso haja colisão entre a forma e o quadrado.
 * @return false : Caso não haja colisão entre a forma e o quadrado.
 */
/*
bool square::collision(square* other)
{
    std::vector<GLfloat>::iterator it;

    //Verifica se há colisão entre um dos vértices do outro objeto e a forma atual.        
    for(it = this->vertices.begin(); it!= this->vertices.end(); it+=3)
    {
        if(!(other->collision(*it, *(it+1), *(it+2))))
			return false; //colisão detectada.
    }
 
    return true; // Não foi detectada colisão.
}*/

/**
 * @brief Desenha o quadrado
 * 
 */
void square::draw()
{
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size()/3);
}

/**
 * @brief Construtor para a classe quadrado
 * 
 * @param usage Modo de desenho
 */
square::square(GLenum usage):geometry(usage){}

/**
 * @brief Construtor para a classe quadrado
 * 
 * @param width Largura do quadrado
 * @param height Altura do quadrado
 * @param xPos Posição x do canto superior esquerdo do quadrado
 * @param yPos Posição y do canto superior esquerdo do quadrado
 */
square::square(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos)
{
    std::vector<GLfloat> aux (12,0.0f);
    aux[0] = xPos;
    aux[1] = yPos;
    aux[3] = xPos + width;
    aux[4] = yPos;
    aux[6] = xPos + width;
    aux[7] = yPos - height;
    aux[9] = xPos;
    aux[10] = yPos -height;
    this->addVertex(aux);
}


void square::addVertex(std::vector<GLfloat> &vertex)
{
    //Verifica se há 3 coordenadas em cada ponto
    if(vertex.size()%3 != 0)
        throw std::string("Número inválido de vértices e/ou pontos.");

    (this->vertices).insert((this->vertices).end(), vertex.begin(), vertex.end()); //Insere os vértices
    
    //Tranfere os novos dados aos buffers de desenho.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
    glBufferData(GL_ARRAY_BUFFER, (this->vertices).size()*sizeof(GLfloat),&(this->vertices[0]), this->usage);
}