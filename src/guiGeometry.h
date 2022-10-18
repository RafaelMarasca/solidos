#ifndef GUI_GEOMETRY_H
#define GUI_GEOMETRY_H

#include<GL/glew.h>
#include<vector>
#include "shaderProgram.h"
#include<iostream>
#include<cmath>
#include "geometry.h"



/**
 * @brief Código fonto para o o shader de vértices
 * 
 */
const std::string squareVertexShader = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main(){\n"
        "gl_Position = vec4(pos*0.9999f, 1.0f);\n"
        "}\n";

/**
 * @brief Código fonte para o shader de fragmentos.
 * 
 */
const std::string squareFragmentShader = 
        "#version 330 core\n"
        "uniform vec4 color;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "FragColor = vec4(color);\n"
        "}\n";


/**
 * @brief Classe square - Define a abstração de um quadrado
 * 
 */
class square : public geometry
{

    private:
        void setup(); //Função de inicialização

        GLuint VBO; //Vertex Buffer Object
        GLuint VAO; //Vertex Array Object
        GLuint colorLoc; //Localização do uniform color

        static unsigned int squareCount; //Contador de formas instanciadas
        static shaderProgram* program;

    public:
        square();
        square(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos); //Construtor para a classe square
        virtual ~square();

        void addVertex(std::vector<GLfloat> &vertex); //Insere vértices na forma.

        bool collision(GLfloat x, GLfloat y, GLfloat z); //Detecta colisão entre o quadrado e um ponto.

        void setupBuffers();

        void draw();
};

#endif //GUI_GEOMETRY_H