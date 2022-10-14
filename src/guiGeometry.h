#ifndef GUI_GEOMETRY_H
#define GUI_GEOMETRY_H

#include<GL/glew.h>
#include<vector>
#include"geometry.h"



/**
 * @brief Classe square - Define a abstração de um quadrado
 * 
 */
class square : public geometry
{

    public:

        square(GLenum usage = GL_STATIC_DRAW); //Construtor para a classe square
        square(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos); //Construtor para a classe square

        void draw();

        void addVertex(std::vector<GLfloat> &vertex);

        bool collision(GLfloat x, GLfloat y, GLfloat z); //Detecta colisão entre o quadrado e um ponto.
        //bool collision(geometry* other); //Detecta a colisão entre o quadrado e outra geometria.

};



#endif //GUI_GEOMETRY_H