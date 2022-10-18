/**
 * @file window.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe window e dos métodos da classe.
 * @version 0.1
 * @date 2022-09
 * 
 * 
 */

#include"window.h"
#include"scene.h"
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<GL/glew.h>
#include<iostream>
#include<sstream>
#include<thread>
#include<string>
#include<vector>
#include<cmath>
#include"gui.h"
#include"menu.h"

int mouseFlag = 0; //Verifica se o mouse esta sendo pressionado. 
int xLast;
int yLast;


/**
 * @brief Adiciona uma forma à cena.
 * 
 * @param shape Ponteiro para a forma a ser adicionada 
 */
void window::addShape(solid* shape)
{
    this->vision->addObject(shape);
    shape->setView(this->cam->getView());
    shape->setProjection(this->cam->getProjection());
    shape->setCameraPos(this->cam->getCameraPos());
    this->selIterator = this->vision->begin();
}


/**
 * @brief Função chamada ao se pressionar uma tecla
 * 
 * @param key : tecla pressionada
 * @param x : posição x do mouse no instante do clique
 * @param y : posição y do mouse no instante do clique
 * @return ** void 
 */
void window::keyp(unsigned char key, int x, int y)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    
    //Verifica qual tecla foi pressionada
    switch(key)
    {
        //Tecla ESC
        case 27: //Desseleciona objeto/cancela operação
            if(w->waitingInput) //Verifica se está sendo construída uma forma
            {
                w->waitingInput = 0; //Seta a flag de espera de entrada para 0
                w->deleteShape();
                w->inType = NONE; //Seta o tipo de entrada pra NONE
                w->setMenu(0); //Seta o menu corrente como 0
            }
            w->clearSelection(); //Desseleciona os objetos
        break;
    
        //Tecla DEL
        case 127: //Deleta a forma selecionada
            try
            {
                w->deleteShape(); //Deleta a forma selecionada
            }catch(std::string s)
            {
                w->showPopUp(s.c_str());
            }
        break;

        //Tecla Espaço
        case 32: //Exibe/Esconde o menu atual
            w->setMenu(w->currentMenu, 
                        w->menu[w->currentMenu]->visible()? HIDDEN : VISIBLE, 
                        false);
        break;

        case 43: //Tecla +
            if(!w->menu[w->currentMenu]->visible())
            {
                w->cam->zoom(-10);
                w->vision->updateCam(w->cam->getView(), w->cam->getProjection(), w->cam->getCameraPos());
            } 
        break;

        case 45: //Tecla -
            if(!w->menu[w->currentMenu]->visible())
            {
                w->cam->zoom(10);
                w->vision->updateCam(w->cam->getView(), w->cam->getProjection(), w->cam->getCameraPos());
            }
        break;

        case 114: //debug
            //if(w->selectedShape)
                //std::cout<<w->cam->isInsideFrostrum(w->selectedShape->centralPoint[0],w->selectedShape->centralPoint[1],w->selectedShape->centralPoint[2])<<std::endl;
            //w->cam->print();
        break;
    }

    //Passa apenas as teclas de números para as caixas de texto
    if(((key>=48) && (key<=57))|| key == 8 || key == 46 ||key==45)
        w->menu[w->currentMenu]->keyPress(key);

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}

void window::specialKeyp(int key, int x, int y)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    if(w->vision->end() != w->vision->begin())
    {
        if(key == GLUT_KEY_RIGHT)
        {
            w->selIterator++;
            
            if(!w->selectedShape || w->selIterator == w->vision->end())
            {
                w->selIterator = w->vision->begin();
            }
            
            w->select(w->vision->getObject((*(w->selIterator)).first));
        }

        if(key == GLUT_KEY_LEFT)
        {
            
            if(!w->selectedShape || w->selIterator == w->vision->begin())
            {
                w->selIterator = w->vision->end();
            }

            w->selIterator--;
            w->select(w->vision->getObject((*w->selIterator).first));
            
        }
    }
    glutPostRedisplay();
}



/**
 * @brief Construtor da classe Window
 * 
 * @param width : largura da janela.
 * @param height : altura da janela.
 * @param title : título da janela
 * @param argc : quantidade de argumentos de linha de comando
 * @param argv : argumentos passados por linha de comando
 */
window::window(int width, int height, const char* title, int* argc, char** argv)
{
    //Funções de inicialização do GLUT
    glutInit(argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width,height);
    glutCreateWindow(title);
    glutDisplayFunc(window::draw);
    glutMouseFunc(window::mouseClick);
    glutMotionFunc(window::mouseMove);
    glutSpecialFunc(window::specialKeyp);
    glutKeyboardFunc(keyp);
    glutReshapeFunc(window::resize);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutSetWindowData(this);
    /*glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(7);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2);*/

    //Inicializa o GLEW
    if(glewInit() != GLEW_OK)
    {
        //Falha na inicialização
        throw std::string("Falha na inicialização do GLEW.");
    }

    this->vision= new scene; //Cria uma cena para exibição
    this->cam = new camera(vec3(0.0f,0.0f,3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,1.0f,0.0f));
    this->cam->rotate(45.0f, 0.0f);
    this->vision->updateCam(cam->getView(), cam->getProjection(), cam->getCameraPos());
    this->waitingInput = 0; //Inicializa a flag de espera de entrada em 0
    this->inType = NONE;  //Tipo de entrada
    this->width = width; //Inicializa a largura da tela
    this->height = height; //Inicializa a altura da tela
    this->selectedShape = nullptr; //Inicializa a forma selecionada como NULL (Nenhuma forma selecionada)
    this->selectedShapeID = 0; //Inicializa o ID de forma selecionada como 0 (Nenhuma forma selecionada)
    this->selIterator = this->vision->begin();
   
    //Inicializa os menus
    this->menu[0] = newShapeMenu();
    this->menu[1] = newCube();
    this->menu[2] = newSphere();
    this->menu[3] = newTorus(); 
    this->menu[4] = newIcosahedron();  
    this->menu[5] = newOptMenu();
    this->menu[6] = newPopUp();
    this->menu[7] = newRotMenu();
    this->menu[8] = newTransMenu();
    this->menu[9] = newScaleMenu();
    this->menu[10] = newHexa();

    this->currentMenu = 0;
    std::vector<GLfloat> c= {0.0f, 0.0f, 0.0f};

    //this->addShape(new cube(0.2, c));
    //c[0] = 0.5;
    //this->addShape(new cube(0.2, c));
    //c[0] = -0.5;
    //this->addShape(new icosphere(0.3, c));
    this->addShape(new torus(0.1, 0.2, c));
}


/**
 * @brief Destrutor para a classe Window.
 * 
 */
window::~window()
{
    sceneIterator it;
    std::pair<unsigned int, geometry*> object;
    std::map<int, frame*>::iterator itMenu;

    //Itera por todos os objetos da cena atual e os deleta.
    for(it = this->vision->begin(); it!= this->vision->end(); it++)
    {
        object = this->vision->getObject((*it).first);
        delete object.second;
        object.second = nullptr;   
    }

    //Deleta os menus
    for(itMenu = this->menu.begin(); itMenu!= this->menu.end(); itMenu++)
        delete (*itMenu).second; 

    delete this->vision; //Deleta a cena.
    delete this->cam;
}


/**
 * @brief Método de desenho para a classe Window.
 * 
 */
void window::draw()
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    glClearColor(0.784f,0.784f,0.784f, 1.0f); //Determina a cor de limpeza do buffer de cor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa o buffer de cor e o buffer de produndidade
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    w->vision->draw(); //Desenha a cena
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    w->menu[w->currentMenu]->draw(); //Desenha o menu corrente.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutSwapBuffers(); //Troca os buffers (Exibe na tela)
}

/**
 * @brief Função que executa a janela.
 * 
 */
void window::init()
{
    glutMainLoop(); //Roda o loop principal da janela
}

/**
 * @brief Método executado ao clicar com o mouse na tela
 * 
 * @param button : botão do mouse clicado
 * @param state : estado do botão no momento do clique
 * @param x : posição x do mouse no instante do clique
 * @param y : posição y do mouse no instante do clique
 */
void window::mouseClick(int button, int state,int x, int y)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    if(w)
    {
        //Se o botão esquerdo foi pressionado
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            float xpos, ypos;

            //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
            if(w->width > w->height){
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
                ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
            }else{
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
                ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
            }

            if(w->menu[w->currentMenu]->visible()) //Verifica se há um menu sendo exibido
            {   
                w->menu[w->currentMenu]->mouseClick(button,state,xpos,ypos); //Função de clique de mouse para os menus
            }else{
                std::vector<GLfloat> coordinates = {xpos, ypos, 0.0f};

                //Cria o início caixa de seleção caso não esteja sendo inserido um ponto
                if(mouseFlag == 0 && !w->waitingInput)
                {
                    mouseFlag = 1; 
                    xLast = x;
                    yLast = y;
                }

                //Caso esteja sendo inserida uma forma via mouse
                if(w->waitingInput && w->inType == MOUSE)
                { 
                    if(w->selectedShape)
                    {
                      
                        
                        /*if((w->vision->checkCollision(xpos, ypos)).first) //Verifica se o ponto inserido é repetido
                        {
                            w->showPopUp("Ponto Repetido!");      

                        }else*/{
                            //Caso não seja repetido adiciona o ponto repetido
                        
                            w->waitingInput--;
                            
                            if(!(w->waitingInput))//Verifica se acabou a inserção de pontos
                            {
                                //Gera a B-Spline
                               
                                w->inType = NONE;
                            }
                        }   
                    }
                }
            }
        }

        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) //Bloco executado ao soltar o mouse e não estar sendo inserida nenhuma forma
        {
            float xpos, ypos;

            //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
            if(w->width > w->height){
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
                ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
            }else{
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
                ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
            }

            if(mouseFlag == 1 && !w->waitingInput) //Altera o tamanho da caixa de seleção
            {
                mouseFlag = 0;
            }
        }
        glutPostRedisplay(); //Requere que a tela seja redesenhada    
    }
}


/**
 * @brief Deleta a forma selecionada atualmente
 * 
 */
void window::deleteShape()
{
    if(this->selectedShape) //Caso alguma forma esteja selecionada
    {
	    geometry* temp = this->vision->removeObject(this->selectedShapeID);//Remove o objeto da cena
        if(!temp)
        {
            throw std::string("Objeto inválido");
		}else
		{
            //Limpa a seleção
		    delete temp;
            this->selectedShape = nullptr;
            this->selectedShapeID = 0;
            this->clearSelection();
		}
    }
}

/**
 * @brief Seleciona uma forma da tela.
 * 
 * @param obj Par ID, ponteiro para gometry, definindo o ID da forma e a própria forma.
 */
void window::select(std::pair<unsigned int, solid*> obj)
{
    if(this->selectedShape)
        this->selectedShape->resetColor(); //Muda a cor da forma desselecionada
    if((this->selectedShape = obj.second))
        this->selectedShape->setColor(SELECTION_R, SELECTION_G, SELECTION_B); //Muda a cora da nova forma selecionada
    
    this->selectedShapeID = obj.first;
    
    if(obj.second == nullptr)
        this->setMenu(0,HIDDEN);
    else
        this->setMenu(5,HIDDEN);

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}

/**
 * @brief Limpa a forma selecionada
 * 
 */
void window::clearSelection()
{
    this->select(std::pair<unsigned int, solid*>{0,nullptr});

    this->setMenu(0,HIDDEN); //Seta o menu corrente para 0.
    this->selIterator = this->vision->begin();

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}

/**
 * @brief Método chamado quando a tela é redimensionada - Mantém a razão de aspecto.
 * 
 * @param newWidth : nova largura da janela
 * @param newHeight : nova altura da janela
 */
void window::resize(int newWidth, int newHeight)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    w->width = newWidth; //Define a nova largura da janela
    w->height = newHeight;  //Define a nova altura da janela

    GLfloat aspectRatio = (GLfloat)newWidth / (GLfloat)newHeight; //Calcula a razão de aspecto da janela atual
    w->aspectRatio = aspectRatio; //Atualiza o valor da razão de aspecto da janela atual

    //Redimensiona a viewport para um quadrado limitado pela menor dimensão da janela
    if(newWidth>newHeight)
    {
        GLfloat aux = GLfloat(newWidth-newHeight)/2.0f;
        glViewport(aux, 0, newHeight, newHeight);
    }else
    {
        GLfloat aux = GLfloat(newHeight-newWidth)/2.0f;
        glViewport(0, aux, newWidth, newWidth);
    }
   
    //Inicializa o sistema de coordenadas para a projeção ortogonal
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //Mantém a razão de aspecto na projeção.
    if (newWidth <= newHeight)
    {
        glOrtho (-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, 1.0f, -1.0f);
    }else
    {
        glOrtho (-1.0f * aspectRatio, 1.0f * aspectRatio,-1.0f, 1.0f, 1.0f, -1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}


/**
 * @brief Função para lidar com o movimento do mouse enquanto está pressionado.
 * 
 * @param x : Posição x do mouse.
 * @param y : Posição y do mouse.
 */
void window::mouseMove(int x, int y)
{
    //Se o mouse está pressionado
    if(mouseFlag)
    {
        window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

        float xpos, ypos;

        //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
        if(w->width > w->height){
            xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
            ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
        }else{
            xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
            ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
        }

        w->cam->mouseMap(xLast-x, yLast-y, 600, 600);
        w->vision->updateCam(w->cam->getView(),w->cam->getProjection(), w->cam->getCameraPos());
        xLast = x;
        yLast = y;
        glutPostRedisplay();
    }
}


/**
 * @brief Retorna o menu corrente.
 * 
 * @return frame* Ponteiro para o menu corrente
 */
frame* window::getMenu()
{
    return this->menu[this->currentMenu];
}

/**
 * @brief Seta o menu corrente.
 * 
 * @param ID : ID do menu que será corrente 
 * @param state : Estado inicial do menu (HIDDEN/ VISIBLE) - DEFAULT: VISIBLE
 * @param clear : true - Limpa os dados do menu / false - Não limpa os dados do menu.
 */
void window::setMenu(int ID, menuState state, bool clear)
{
    if(clear)
        this->menu[this->currentMenu]->clear(); //Limpa os dados do menu corrente.

    this->menu[this->currentMenu]->hide(); //Esconde o menu corrente.

    if(ID == 5)
    {
        if(this->selectedShape)
        {
            this->getMenu()->setState(2,this->selectedShape->getWireFrameState());
            this->getMenu()->setState(4,this->selectedShape->getSolidState());
        }
    }
    if(state == VISIBLE) //Verifica se o estado inicial é VISIBLE.
        this->menu[ID]->show(); //Mostra o novo menu.
    else
        this->menu[ID]->hide(); //Esconde o novo menu.
    this->currentMenu = ID; //Seta o menu ID como corrente.
}

/**
 * @brief Mostra o popup
 * 
 * @param text Texto a ser exibido pelo popup
 */
void window::showPopUp(const char* text)
{
    //Armazena os dados do menu corrente
    this->tempMenu.first = currentMenu;
    this->tempMenu.second = this->menu[this->currentMenu]->visible();  

    //Seta o menu atual como popup
    this->setMenu(6);
    textDisp* t = dynamic_cast<textDisp*>(this->menu[6]->getElement(1));
    //Altera o texto do popup
    if(t)
        t->setText(text);
}

/**
 * @brief Fecha o popUp corrente
 * 
 */

void window::closePopUp()
{
    //Resgata o menu passado conforme salvo pela função showPopUp
    if(this->tempMenu.second)
            this->setMenu(this->tempMenu.first, VISIBLE);
        else
            this->setMenu(this->tempMenu.first, HIDDEN); 
}


/**
 * @brief Getter para o valor de espera de entrada.
 * 
 */
int window::getWaiting()
{
    return this->waitingInput;
}


/**
 * @brief Setter para o valor de espera de entrada.
 * 
 */
void window::setWaiting(int waiting)
{
    this->waitingInput = waiting;
}

/**
 * @brief Setter para o tipo de entrada.
 * 
 * @param in Tipo de entrada.
 */
void window::setInputType(inputType in)
{
    this->inType = in;
}

/**
 * @brief Getter para o tipo de entrada atual;
 * 
 * @return inputType Tipo de entrada atual.
 */
inputType window::getInputType()
{
    return this->inType;
}

/**
 * @brief Decrementa o valor de entrada atual.
 * 
 */
void window::decWaiting()
{
    this->waitingInput--;
}


/**
 * @brief Getter para a forma selecionada.
 * 
 */
solid* window::getSelectedShape()
{
    return this->selectedShape;
}

scene* window::getScene()
{
    return this->vision;
}


camera* window::getCam()
{
    return this->cam;
}


void window::updateSelection()
{
    this->selIterator = this->vision->begin();
}