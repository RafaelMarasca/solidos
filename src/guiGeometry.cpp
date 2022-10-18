#include "guiGeometry.h"


//Declaração das strings contendo os códigos fonte dos shaders
extern const std::string squareVertexShader;
extern const std::string squareFragmentShader;

//Inicialização da constante estática de classe responsável 
//por contar a quantidade de formas na tela
unsigned int square::squareCount = 0;

shaderProgram* square::program = nullptr;

/**
 * @brief Construtor para a classe square.
 * 
 */
square::square()
{
    //Inicializa a cor da forma.
    this->color = {GEOMETRY_R, GEOMETRY_G, GEOMETRY_B, 1.0f}; 
    this->usage = GL_STATIC_DRAW; //Inicializa o tipo de uso do desenho.

    //Verifica se já não há um shaderProgram instanciado, caso não haja, instancia.
    if(square::program == nullptr)
        square::program = new shaderProgram(squareVertexShader, squareFragmentShader);
   
    glGenBuffers(1, &(this->VBO));
    glGenVertexArrays(1,&(this->VAO));

    this->colorLoc = this->program->getLoc("color");
   
    this->setupBuffers();

    square::squareCount++; //Incrementa o contador de formas.
}


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

    //Inicializa a cor da forma.
    this->color = {GEOMETRY_R, GEOMETRY_G, GEOMETRY_B, 1.0f}; 

    this->vertices = 
    {
        xPos, yPos, 0.0f,
        xPos + width, yPos, 0.0f,
        xPos + width, yPos - height, 0.0f,
        xPos, yPos -height, 0.0f
    };

    //Verifica se já não há um shaderProgram instanciado, caso não haja, instancia.
    if(square::program == nullptr)
        square::program = new shaderProgram(squareVertexShader, squareFragmentShader);


    glGenBuffers(1, &(this->VBO));
    glGenVertexArrays(1,&(this->VAO));

    this->colorLoc = this->program->getLoc("color");

    this->setupBuffers();

    square::squareCount++; //Incrementa o contador de formas.
}


square::~square()
{
    square::squareCount--; //Decrementa o contador de formas.

    //Se não há mais formas no programa, desaloca a memória para o programa shader.
    if(square::squareCount == 0 && square::program != nullptr)
    {
        delete square::program; //deleta o programa
        square::program = nullptr; //Faz o ponteiro para o shaderProgram apontar para null.
    }
    
    //Libera a memória associadas aos buffers criados para a forma corrente.
    glDeleteBuffers(1,&(this->VBO));
    glDeleteVertexArrays(1,&(this->VAO));
}


/**
 * @brief Desenha o quadrado
 * 
 */
void square::draw()
{
    square::program->use();
    glUniform4fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size()/3);
}


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

    GLfloat xMax = std::numeric_limits<float>::lowest(), xMin = std::numeric_limits<float>::max(), 
            yMax = std::numeric_limits<float>::lowest(), yMin = std::numeric_limits<float>::max();
                                 
    //Verifica se há colisão entre o ponto e os vértices                                 
    for(it = this->vertices.begin(); it!= this->vertices.end(); it+=3)
    {
        if((std::abs(*it - x) < 0.0000001f ) && (std::abs(*(it+1) - y) < 0.000001f ))
            return true;
    }
                             
    //Obtém os pontos mínimos e máximos das coordenadas x e y e armazena no vetor min_max
    for(it = this->vertices.begin(); it!= this->vertices.end(); it+=3)
    {
        if((*it)<xMin)
        {
            xMin = *it;
        }
        if((*it)>xMax)
        {
            xMax = *it;
        }

        if(*(it+1)<yMin)
        {
            yMin = *(it+1);
        }
        if(*(it+1)>yMax)
        {
            yMax = *(it+1);
        }
    }

    //verifica se o ponto se encontra dentro dos limites do vetor min_max
    if(x>xMin && x<xMax && y>yMin && y<yMax)
    {
        //Sim, o ponto se encontra dentro dos limites(colisão).
        return true;
    }else{
        //Não o ponto não se encontra dentro dos limites (i.e. não há colisão).
        return false;
    }         
}

void square::setupBuffers()
{
    //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices).size()*sizeof(GLfloat),&(this->vertices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

/**
 * @brief Adiciona vértices ao objeto de geometry.
 * 
 * @param vertex : vetor de vértices a serem adicionados.
 */
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
