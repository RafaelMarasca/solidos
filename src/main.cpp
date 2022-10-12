/**
 * @file main.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Função principal, inicializa o programa.
 * @version 0.1
 * @date 2022-09
 */

#include"geometry.h"
#include"GL/glew.h"
#include"GL/freeglut.h"
#include<iostream>
#include<glm/glm.hpp>



geometry* geo = nullptr;
geometry* geo2 = nullptr;

void rotate(int value)
{
    geo->rotate(15, Y_AXIS);
    geo2->rotate(15,Y_AXIS);
    //geo->rotate(7, Y_AXIS);
    //geo2->rotate(7,Y_AXIS);
    //geo->rotate(30, X_AXIS);
    //geo2->rotate(30,X_AXIS);

    glutPostRedisplay();
}

void draw()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.784f,0.784f,0.784f, 1.0f); //Determina a cor de limpeza do buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa o buffer com a cor determinada
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    geo->draw();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    geo2->draw();
    glutTimerFunc(500,rotate, 0); 
    glutSwapBuffers();
}




int main(int argc, char**argv)
{

    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("teste");
    glutDisplayFunc(draw);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(3.0f);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
   // glFrontFace(GL_CCW);  
    glEnable(GL_DEPTH_TEST);
    
    //glClearDepth(1.0f);
    //glDepthFunc(GL_LESS);

    //Inicializa o GLEW
    if(glewInit() != GLEW_OK)
    {
        //Falha na inicialização
        throw std::string("Falha na inicialização do GLEW.");
    }

    std::vector<GLfloat> c= {0.0f, 0.0f, 0.0f};

    geo =  new icosphere(1.0f, c, 5);//new cube(0.5f, c);

    geo->rotate(-45.0,Y_AXIS);
    geo->rotate(45.0, X_AXIS);
    geo->translate(0.0f,0.0f,-5.0f);
    geo->setProjection(90.0f);

    //dynamic_cast<icosahedron*>(geo)->subdivide(0,1.0f);

    geo2 =  new icosphere(1.02f, c, 5);//new icosahedron(2.0f, c);//new cube(0.5f, c);
    geo2->setColor(1.0f,0.0f,0.0f);

    geo2->rotate(-45.0,Y_AXIS);
    geo2->rotate(45.0, X_AXIS);
    geo2->translate(0.0f,0.0f,-5.0f);
    geo2->setProjection(90.0f);
    //dynamic_cast<icosahedron*>(geo2)->subdivide(0,1.0f);
    //geo->rotate(45,Z);

    glutMainLoop(); //Roda o loop principal da janela


    delete geo;
    
    return 0;
}