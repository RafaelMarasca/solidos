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
#include"axis.h"
#include"camera.h"
#include<iostream>
#include<glm/glm.hpp>
#include"window.h"



geometry* geo = nullptr;
geometry* geo2 = nullptr;
geometry* geo3 = nullptr;
vec3 pos(0.0f,0.0f,3.0f);
vec3 target(0.0f, 0.0f, 0.0f);
vec3 up(0.0f, 1.0f, 0.0f);
camera cam(pos,target,up);




void rotate(int value)
{
    static GLfloat rot = 15;
    //cam.rotate(15.0f, 0.0f);
    cam.rotate(0.0f, 15.0f);
    geo->setView((cam.getView()));
    geo2->setView((cam.getView()));
    geo3->setView((cam.getView()));
    //geo->rotate(15, Y_AXIS);
    //geo2->rotate(15,Y_AXIS);
    //geo3->rotate(15, Y_AXIS);
    //geo->rotate(7, Y_AXIS);
    //geo2->rotate(7,Y_AXIS);
    //geo->rotate(30, X_AXIS);
    //geo2->rotate(30,X_AXIS);
    //rot+=15;
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
    geo3->draw();
    //glutTimerFunc(500,rotate, 0); 
    glutSwapBuffers();
}

void teste(unsigned char key, int x, int y)
{
    if(key == 43)
    {
        cam.zoom(-10);
        geo->setProjection(cam.getProjection());
        geo2->setProjection(cam.getProjection());
        geo3->setProjection(cam.getProjection());
        std::cout<<"maisss";
    }

    if(key == 45)
    {
        cam.zoom(10);
        geo->setProjection(cam.getProjection());
        geo2->setProjection(cam.getProjection());
        geo3->setProjection(cam.getProjection());
        std::cout<<"menos";
    }

    glutPostRedisplay();
}

int xLast = 0, yLast = 0;
int flag = 0;

void mouseMove(int x, int y)
{
    if(flag)
    {
        cam.mouseMap(xLast-x, yLast-y, 600, 600);
        geo->setView((cam.getView()));
        geo2->setView((cam.getView()));
        geo3->setView((cam.getView()));
        xLast = x;
        yLast = y;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        flag = 1;
        xLast = x;
        yLast = y;
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        flag = 0;
    }
    

}
/*
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
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(teste);
    glutMouseFunc(mouse);
    glPointSize(3.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    //glClearDepth(1.0f);
    glDepthFunc(GL_LESS);

    //Inicializa o GLEW
    if(glewInit() != GLEW_OK)
    {
        //Falha na inicialização
        throw std::string("Falha na inicialização do GLEW.");
    }

    std::vector<GLfloat> c= {0.0f, 0.0f, 0.0f};
    
    
    //cam.rotate(45.0f, 0.0f);
    geo = new cube(0.5f, c);
    geo->setView(cam.getView());
    //geo->rotate(-45.0,Y_AXIS);
    //geo->rotate(45.0, X_AXIS);
   // geo->translate(0.0f,0.0f,-3.0f);
    geo->setProjection(cam.getProjection());

    //dynamic_cast<icosahedron*>(geo)->subdivide(0,1.0f);

    //cam.getView().print();
    //std::cout<<"----------------------"<<std::endl;
    //std::cout<<glm::to_string(glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  		   //glm::vec3(0.0f, 0.0f, 0.0f), 
  		   //glm::vec3(0.0f, 1.0f, 0.0f)))<<std::endl;

    geo2 = new cube(0.5f, c);
    geo2->setColor(1.0f,0.0f,0.0f);
    geo2->setView((cam.getView()));

    //geo2->rotate(-45.0,Y_AXIS);
    //geo2->rotate(45.0, X_AXIS);
    //geo2->translate(0.0f,0.0f,-3.0f);
    geo2->setProjection(cam.getProjection());
    //dynamic_cast<icosahedron*>(geo2)->subdivide(0,1.0f);
    //geo->rotate(45,Z);

    geo3 = new axis();// new icosphere(1.02f, c, 5);//new icosahedron(2.0f, c);//new cube(0.5f, c);
    geo3->setColor(0.0f,1.0f,0.0f);
    geo3->setView((cam.getView()));

    //geo3->rotate(-45.0,Y_AXIS);
    //geo3->rotate(45.0, X_AXIS);
    //geo3->translate(0.0f,0.0f,-3.0f);
    geo3->setProjection(cam.getProjection());

    glutMainLoop(); //Roda o loop principal da janela


    delete geo;
    
    return 0;
}*/


int main(int argc, char** argv)
{
    try{
        //Cria uma janela com tamanho inicial 250x250
        window a(600,600, std::string("Hello Curves").c_str(), &argc, argv);
        //Inicializa a janela e a exibe na tela
        a.init(); 
    }catch(std::string b){
        //Exibe as exceções no terminal
        std::cout<<b<<std::endl;
    };

    return 0;
}