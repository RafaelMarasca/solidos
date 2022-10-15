#include "menu.h"
#include "gui.h"
#include "window.h"

/**
 * @brief Cria o menu 0 (menu de criação de formas)
 * 
 * @return frame* ponteiro para o menu criado
 */
frame* newShapeMenu()
{
    frame* menu = new frame(1.0f, 0.5f, 5, 4, 0.45, 0.9); //Cria o frame

    //Adiciona os elementos de menu
    menu->addText(0,0,1,4,0,"Adicionar Forma");
    menu->addButton(1,0,1,4,1,"Cubo");
    menu->addButton(2,0,1,4,2,"Esfera");
    menu->addButton(3,0,1,4,3,"Torus");
    menu->addButton(4,0,1,4,4,"Icosaedro");

    menu->addClickFunction(menuClickShape); //Adiciona a função chamada ao clicar em um elemento do menu 0
    
    menu->generate(); //Gera o menu

    return menu;
}

/**
 * @brief Handler para os cliques no elemenstos do menu 0 (Menu de adição de B-Spline)
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */

void menuClickShape(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 1:
                w->setWaiting(1);
                w->getMenu()->clear(); //Limpa os dados do menu
                w->setMenu(1); //Seta o menu atual para o menu de adição de pontos
            break;

            case 2:
                w->setWaiting(1);
                w->getMenu()->clear(); //Limpa os dados do menu
                w->setMenu(2); //Seta o menu atual para o menu de adição de pontos
            break;

            case 3:
                w->setWaiting(1);
                w->getMenu()->clear(); //Limpa os dados do menu
                w->setMenu(3); //Seta o menu atual para o menu de adição de pontos
            break;
            
            case 4:
                w->setWaiting(1);
                w->getMenu()->clear(); //Limpa os dados do menu
                w->setMenu(4); //Seta o menu atual para o menu de adição de pontos
            break;
        }
    }
}

/**
 * @brief Handler para os cliques do menu 1 (menu de adição de pontos).
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClick1(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 5: //Botão Adicionar
            {
                std::string xStr = (w->getMenu())->getTextInput(2); //Obtém o texto da caixa de texto 2.
                std::string yStr = (w->getMenu())->getTextInput(4); //Obtém o texto da caixa de texto 4.

                if(xStr.size()!= 0 && yStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat x=0, y=0;

                    //Converte as strings para float.
                    x = stof(xStr);
                    y = stof(yStr);

                    if(std::fabs(x)>1  || std::fabs(y)>1) //Verifica se o desenho ficará fora da tela
                    {
                         w->showPopUp("O Desenho Ficara Fora da Tela!");
                    }
                    
                    //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                    {
                        //Caso não tenha colidido
                    
                        w->decWaiting(); //Decrementa a variável de espera de entrada.

                        w->getMenu()->clear(); //Limpa os dados do menu
                        
                        if(!w->getWaiting()) //Se não há espera por entrada
                        {
                            w->setInputType(NONE); //Seta o tipo de entrada para nenhuma
                            w->setMenu(0,HIDDEN); //Seta o menu como 0.
                        }
                    }//else{
                        //Caso tenha havido colisão
                        //w->showPopUp("Ponto Repetido!"); //Mostra o popUp de ponto repetido 
                   // }
                }
            }break;
        
            case 6: //Botão Cancelar;
                if(w->getWaiting())
                {
                    w->setWaiting(0); //Zera a flag de espera por entrada
                    w->setMenu(0);//Seta o menu para 0.
                    w->setInputType(NONE);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;
        }
    }
}



/**
 * @brief Handler para os cliques do menu de Opção.
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClickOpt(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        if(w->getSelectedShape()) //Verifica se alguma forma está selecionada
        {
            switch(ID)
            {
                case 2: //Botão de Aramado
                {
                    
                }break;

                case 3: //Botão de Rotação
                {
                    w->getMenu()->clear(); //Limpa os dados do menu
                    w->setMenu(7); //Seta o menu atual para o menu de adição de pontos
                }break;
                
                case 4: //Botão de Translação
                {
                    w->getMenu()->clear(); //Limpa os dados do menu
                    w->setMenu(8); //Seta o menu atual para o menu de adição de pontos
                }break;

                case 5: //Botão de Escala
                {
                    w->getMenu()->clear(); //Limpa os dados do menu
                    w->setMenu(9); //Seta o menu atual para o menu de adição de pontos
                }break;
            }
        }
    }
}


/**
 * @brief Handler para os cliques do menu 3 (menu de popUp).
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClickPopUp(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
       if(ID == 0) //Botão OK
       {
            w->closePopUp(); //Fecha o popUp
       }
    }
}

/**
 * @brief Handler para os cliques do menu 4 (menu de Adicao).
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClick4(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 10: //Botão Cancelar;
                if(w->getWaiting())
                {
                    w->setWaiting(0); //Zera a flag de espera por entrada
                    w->setMenu(0);//Seta o menu para 0.
                    w->setInputType(NONE);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;
        }
    }
}

/**
 * @brief Handler para os cliques do menu de adição de Torus.
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClickTorus(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 12: //Botão Cancelar;
                if(w->getWaiting())
                {
                    w->setWaiting(0); //Zera a flag de espera por entrada
                    w->setMenu(0);//Seta o menu para 0.
                    w->setInputType(NONE);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;

            case 13: //Botão Adicionar;
            {
                std::string xStr = (w->getMenu())->getTextInput(3); //Obtém o texto da caixa de texto 3.
                std::string yStr = (w->getMenu())->getTextInput(5); //Obtém o texto da caixa de texto 5.
                std::string zStr = (w->getMenu())->getTextInput(7); //Obtém o texto da caixa de texto 7.
                std::string R1Str = (w->getMenu())->getTextInput(9); //Obtém o texto da caixa de texto 9.
                std::string R2Str = (w->getMenu())->getTextInput(11); //Obtém o texto da caixa de texto 11.

                if(xStr.size()!= 0 && yStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat R1 = 0, R2 = 0;
                    std::vector<GLfloat> center(3,0.0f);

                    //Converte as strings para float.
                    center[0] = stof(xStr);
                    center[1] = stof(yStr);
                    center[2] = stof(zStr);
                    R1 = stof(R1Str);
                    R2 = stof(R2Str);


                    if(R1<0.0001f || R2<0.0001f)
                    {
                        w->showPopUp("Raio deve ser > 0");
                    }else
                    {
                        if(std::fabs(center[0])>1  || std::fabs(center[1])>1 || std::fabs(center[2])>1) //Verifica se o desenho ficará fora da tela
                        {
                            w->showPopUp("O Desenho Ficara Fora da Tela!");
                        }
                        
                        //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                        {
                            //Caso não tenha colidido
                        
                            w->addShape(new torus(R1, R2, center));

                            w->setWaiting(0); //Decrementa a variável de espera de entrada.

                            w->getMenu()->clear(); //Limpa os dados do menu
                            
                            if(!w->getWaiting()) //Se não há espera por entrada
                            {
                                w->setInputType(NONE); //Seta o tipo de entrada para nenhuma
                                w->setMenu(0,HIDDEN); //Seta o menu como 0.
                            }

                            
                        }//else{
                            //Caso tenha havido colisão
                            //w->showPopUp("Ponto Repetido!"); //Mostra o popUp de ponto repetido 
                    // }
                    }
                }
            }break;
        }
    }
}


/**
 * @brief Handler para os cliques do menu de adição de Esfera.
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClickSphere(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 10: //Botão Cancelar;
                if(w->getWaiting())
                {
                    w->setWaiting(0); //Zera a flag de espera por entrada
                    w->setMenu(0);//Seta o menu para 0.
                    w->setInputType(NONE);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;

            case 11: //Botão Adicionar;
            {
                std::string xStr = (w->getMenu())->getTextInput(3); //Obtém o texto da caixa de texto 3.
                std::string yStr = (w->getMenu())->getTextInput(5); //Obtém o texto da caixa de texto 5.
                std::string zStr = (w->getMenu())->getTextInput(7); //Obtém o texto da caixa de texto 7.
                std::string rStr = (w->getMenu())->getTextInput(9); //Obtém o texto da caixa de texto 9.

                if(xStr.size()!= 0 && yStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat r = 0;
                    std::vector<GLfloat> center(3,0.0f);

                    //Converte as strings para float.
                    center[0] = stof(xStr);
                    center[1] = stof(yStr);
                    center[2] = stof(zStr);
                    r = stof(rStr);

                    if(r<0.0001f)
                    {
                        w->showPopUp("Raio deve ser > 0");
                    }else
                    {
                        if(std::fabs(center[0])>1  || std::fabs(center[1])>1 || std::fabs(center[2])>1) //Verifica se o desenho ficará fora da tela
                        {
                            w->showPopUp("O Desenho Ficara Fora da Tela!");
                        }
                        
                        //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                        {
                            //Caso não tenha colidido
                        
                            w->addShape(new icosphere(r, center));

                            w->setWaiting(0); //Decrementa a variável de espera de entrada.

                            w->getMenu()->clear(); //Limpa os dados do menu
                            
                            if(!w->getWaiting()) //Se não há espera por entrada
                            {
                                w->setInputType(NONE); //Seta o tipo de entrada para nenhuma
                                w->setMenu(0,HIDDEN); //Seta o menu como 0.
                            }
                        }//else{
                            //Caso tenha havido colisão
                            //w->showPopUp("Ponto Repetido!"); //Mostra o popUp de ponto repetido 
                    // }
                    }
                }
            }break;
        }
    }
}


/**
 * @brief Handler para os cliques do menu de adição de Cubo.
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClickCube(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 10: //Botão Cancelar;
                if(w->getWaiting())
                {
                    w->setWaiting(0); //Zera a flag de espera por entrada
                    w->setMenu(0);//Seta o menu para 0.
                    w->setInputType(NONE);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;

            case 11: //Botão Adicionar;
            {
                std::string xStr = (w->getMenu())->getTextInput(3); //Obtém o texto da caixa de texto 3.
                std::string yStr = (w->getMenu())->getTextInput(5); //Obtém o texto da caixa de texto 5.
                std::string zStr = (w->getMenu())->getTextInput(7); //Obtém o texto da caixa de texto 7.
                std::string edgeStr = (w->getMenu())->getTextInput(9); //Obtém o texto da caixa de texto 9.

                if(xStr.size()!= 0 && yStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat edge = 0;
                    std::vector<GLfloat> center(3,0.0f);

                    //Converte as strings para float.
                    center[0] = stof(xStr);
                    center[1] = stof(yStr);
                    center[2] = stof(zStr);
                    edge = stof(edgeStr);

                    if(edge<0.0001f)
                    {
                        w->showPopUp("Aresta deve ser > 0");
                    }else
                    {
                        if(std::fabs(center[0])>1  || std::fabs(center[1])>1 || std::fabs(center[2])>1) //Verifica se o desenho ficará fora da tela
                        {
                            w->showPopUp("O Desenho Ficara Fora da Tela!");
                        }
                        
                        //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                        {
                            //Caso não tenha colidido
                        
                            w->addShape(new cube(edge, center));

                            w->setWaiting(0); //Decrementa a variável de espera de entrada.

                            w->getMenu()->clear(); //Limpa os dados do menu
                            
                            if(!w->getWaiting()) //Se não há espera por entrada
                            {
                                w->setInputType(NONE); //Seta o tipo de entrada para nenhuma
                                w->setMenu(0,HIDDEN); //Seta o menu como 0.
                            }
                        }//else{
                            //Caso tenha havido colisão
                            //w->showPopUp("Ponto Repetido!"); //Mostra o popUp de ponto repetido 
                    // }
                    }
                }
            }break;
        }
    }
}


/**
 * @brief Handler para os cliques do menu de adição de Icosaedro.
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void menuClickIcosahedron(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 10: //Botão Cancelar;
                if(w->getWaiting())
                {
                    w->setWaiting(0); //Zera a flag de espera por entrada
                    w->setMenu(0);//Seta o menu para 0.
                    w->setInputType(NONE);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;

            case 11: //Botão Adicionar;
            {
                std::string xStr = (w->getMenu())->getTextInput(3); //Obtém o texto da caixa de texto 3.
                std::string yStr = (w->getMenu())->getTextInput(5); //Obtém o texto da caixa de texto 5.
                std::string zStr = (w->getMenu())->getTextInput(7); //Obtém o texto da caixa de texto 7.
                std::string edgeStr = (w->getMenu())->getTextInput(9); //Obtém o texto da caixa de texto 9.

                if(xStr.size()!= 0 && yStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat edge = 0;
                    std::vector<GLfloat> center(3,0.0f);

                    //Converte as strings para float.
                    center[0] = stof(xStr);
                    center[1] = stof(yStr);
                    center[2] = stof(zStr);
                    edge = stof(edgeStr);

                    if(edge<0.0001f)
                    {
                        w->showPopUp("Aresta deve ser > 0");
                    }else
                    {
                        if(std::fabs(center[0])>1  || std::fabs(center[1])>1 || std::fabs(center[2])>1) //Verifica se o desenho ficará fora da tela
                        {
                            w->showPopUp("O Desenho Ficara Fora da Tela!");
                        }
                        
                        //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                        {
                            //Caso não tenha colidido
                        
                            w->addShape(new icosahedron(edge, center));

                            w->setWaiting(0); //Decrementa a variável de espera de entrada.

                            w->getMenu()->clear(); //Limpa os dados do menu
                            
                            if(!w->getWaiting()) //Se não há espera por entrada
                            {
                                w->setInputType(NONE); //Seta o tipo de entrada para nenhuma
                                w->setMenu(0,HIDDEN); //Seta o menu como 0.
                            }
                        }//else{
                            //Caso tenha havido colisão
                            //w->showPopUp("Ponto Repetido!"); //Mostra o popUp de ponto repetido 
                    // }
                    }
                }
            }break;
        }
    }
}

/**
 * @brief Cria o menu 1 (menu de adição de pontos)
 * 
 * @return frame* ponteiro para o menu criado
 */
frame* newAddMenu()
{
    frame* menu = new frame(1.2f, 1.0f, 7, 4, -0.5, 0.5); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};

    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ADICIONAR CUBO");

    menu->addText(1,0,1,4,1,"PONTO CENTRAL");
    menu->addText(2,0,1,2,2,"X");
    menu->addTextInput(2,1,1,2,3);
    menu->addText(3,0,1,2,4,"Y");
    menu->addTextInput(3,1,1,2,5);
    menu->addText(4,0,1,2,6,"Z");
    menu->addTextInput(4,1,1,2,7);

    menu->addText(5,0,1,2,8,"ARESTA");
    menu->addTextInput(5,1,1,2,9);

    menu->addButton(6,0,1,2,10,"Cancelar",red);
    menu->addButton(6,1,1,2,11,"Adicionar");
    
    menu->addClickFunction(menuClick1); //Adiciona a função chamada ao se clicar no menu
    
    menu->generate(); //Gera o menu

    return menu;
}

/**
 * @brief Cria o menu de Opção.
 * 
 * @return frame* ponteiro para o menu criado.
 */
frame* newOptMenu()
{
    frame* menu = new frame(1.0f, 1.0f, 5, 4, -0.5, 0.5); //Cria o frame do menu

    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"MODIFICAR FORMA");
    menu->addText(1,0,1,3,1,"Mostrar Aramado");
    menu->addToggleButton(1,1,1,1,2,false);
    menu->addButton(2,0,1,4,3,"Rotacionar");
    menu->addButton(3,0,1,4,4,"Transladar");
    menu->addButton(4,0,1,4,5,"Escalar");
    menu->addClickFunction(menuClickOpt); //Adiciona a função chamada ao clicar em um elemento do menu
    
    menu->generate();//Gera o menu

    return menu;
}

/**
 * @brief Cria o menu de Rotação.
 * 
 * @return frame* ponteiro para o menu criado.
 */
frame* newRotMenu()
{
    frame* menu = new frame(0.5f, 1.0f, 3, 4, -0.5, 0.25); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};

    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ROTACIONAR");

    menu->addText(1,0,1,2,1,"Angulo");
    menu->addTextInput(1,1,1,2,2);

    menu->addButton(2,0,1,2,3,"Cancelar",red);
    menu->addButton(2,1,1,2,4,"Confirmar");

    menu->addClickFunction(menuClickRot); //Adiciona a função chamada ao clicar em um elemento do menu

    menu->generate();//Gera o menu

    return menu;
}

/**
 * @brief Cria o menu de Translação.
 * 
 * @return frame* ponteiro para o menu criado.
 */
frame* newTransMenu()
{
    frame* menu = new frame(0.8f, 1.0f, 5, 4, -0.5, 0.4); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};

    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"TRANSLADAR");

    menu->addText(1,0,1,2,1,"DX");
    menu->addTextInput(1,1,1,2,2);
    menu->addText(2,0,1,2,3,"DY");
    menu->addTextInput(2,1,1,2,4);
    menu->addText(3,0,1,2,5,"DZ");
    menu->addTextInput(3,1,1,2,6);

    menu->addButton(4,0,1,2,7,"Cancelar",red);
    menu->addButton(4,1,1,2,8,"Confirmar");
    
     menu->addClickFunction(menuClickTrans); //Adiciona a função chamada ao clicar em um elemento do menu

    menu->generate();//Gera o menu

    return menu;
}

/**
 * @brief Cria o menu de Escala.
 * 
 * @return frame* ponteiro para o menu criado.
 */
frame* newScaleMenu()
{
    frame* menu = new frame(0.5f, 1.0f, 3, 4, -0.5, 0.25); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};

    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ESCALAR");

    menu->addText(1,0,1,2,1,"Escala");
    menu->addTextInput(1,1,1,2,2);

    menu->addButton(2,0,1,2,3,"Cancelar",red);
    menu->addButton(2,1,1,2,4,"Confirmar");

    menu->addClickFunction(menuClickScale); //Adiciona a função chamada ao clicar em um elemento do menu

    menu->generate();//Gera o menu

    return menu;
}


/**
 * @brief Cria o menu 3 (menu de popUp)
 * 
 * @return frame* ponteiro para o menu criado
 */

frame* newPopUp()
{
    frame* menu = new frame(0.5f, 1.2f, 2, 4, -0.6, 0.25, -1, 0.1); //Cria o frame do menu
    
    //Adiciona os elementos do menu
    menu->addButton(1,1,1,2,0,"OK");
    menu->addText(0,0,1,4,1,"LOREM IPSUM AMET");
    menu->addClickFunction(menuClickPopUp);
    
    menu->generate();//Gera o menu

    return menu;
}


frame* newSphere()
{
    frame* menu = new frame(1.2f, 1.0f, 7, 4, -0.5, 0.6); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};
    
    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ADICIONAR ESFERA");

    menu->addText(1,0,1,4,1,"PONTO CENTRAL");
    menu->addText(2,0,1,2,2,"X");
    menu->addTextInput(2,1,1,2,3);
    menu->addText(3,0,1,2,4,"Y");
    menu->addTextInput(3,1,1,2,5);
    menu->addText(4,0,1,2,6,"Z");
    menu->addTextInput(4,1,1,2,7);

    menu->addText(5,0,1,2,8,"RAIO");
    menu->addTextInput(5,1,1,2,9);

    menu->addButton(6,0,1,2,10,"Cancelar",red);
    menu->addButton(6,1,1,2,11,"Adicionar");
    
    menu->addClickFunction(menuClickSphere); //Adiciona a função chamada ao se clicar no menu
    
    menu->generate(); //Gera o menu

    return menu;
}


frame* newCube()
{
    frame* menu = new frame(1.2f, 1.0f, 7, 4, -0.5, 0.6); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};
    
    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ADICIONAR CUBO");

    menu->addText(1,0,1,4,1,"PONTO CENTRAL");
    menu->addText(2,0,1,2,2,"X");
    menu->addTextInput(2,1,1,2,3);
    menu->addText(3,0,1,2,4,"Y");
    menu->addTextInput(3,1,1,2,5);
    menu->addText(4,0,1,2,6,"Z");
    menu->addTextInput(4,1,1,2,7);

    menu->addText(5,0,1,2,8,"ARESTA");
    menu->addTextInput(5,1,1,2,9);

    menu->addButton(6,0,1,2,10,"Cancelar",red);
    menu->addButton(6,1,1,2,11,"Adicionar");
    
    menu->addClickFunction(menuClickCube); //Adiciona a função chamada ao se clicar no menu
    
    menu->generate(); //Gera o menu

    return menu;
}


frame* newTorus()
{
    frame* menu = new frame(1.2f, 1.0f, 8, 4, -0.5, 0.6); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};
    
    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ADICIONAR TORUS");

    menu->addText(1,0,1,4,1,"PONTO CENTRAL");
    menu->addText(2,0,1,2,2,"X");
    menu->addTextInput(2,1,1,2,3);
    menu->addText(3,0,1,2,4,"Y");
    menu->addTextInput(3,1,1,2,5);
    menu->addText(4,0,1,2,6,"Z");
    menu->addTextInput(4,1,1,2,7);

    menu->addText(5,0,1,4,6,"RAIOS");
    menu->addText(6,0,1,1,8,"R1");
    menu->addTextInput(6,1,1,1,9);
    menu->addText(6,2,1,1,10,"R2");
    menu->addTextInput(6,3,1,1,11);

    menu->addButton(7,0,1,2,12,"Cancelar",red);
    menu->addButton(7,1,1,2,13,"Adicionar");
    
    menu->addClickFunction(menuClickTorus); //Adiciona a função chamada ao se clicar no menu
    
    menu->generate(); //Gera o menu

    return menu;
}

frame* newIcosahedron()
{
    frame* menu = new frame(1.2f, 1.0f, 7, 4, -0.5, 0.5); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};
    
    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ADICIONAR ICOSAEDRO");

    menu->addText(1,0,1,4,1,"PONTO CENTRAL");
    menu->addText(2,0,1,2,2,"X");
    menu->addTextInput(2,1,1,2,3);
    menu->addText(3,0,1,2,4,"Y");
    menu->addTextInput(3,1,1,2,5);
    menu->addText(4,0,1,2,6,"Z");
    menu->addTextInput(4,1,1,2,7);

    menu->addText(5,0,1,2,8,"ARESTA");
    menu->addTextInput(5,1,1,2,9);

    menu->addButton(6,0,1,2,10,"Cancelar",red);
    menu->addButton(6,1,1,2,11,"Adicionar");
    
    menu->addClickFunction(menuClickIcosahedron); //Adiciona a função chamada ao se clicar no menu
    
    menu->generate(); //Gera o menu

    return menu;
}

void menuClickRot(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 4: //Botão Rodar
            {
                std::string angleStr = (w->getMenu())->getTextInput(2); //Obtém o texto da caixa de texto 2.

                if(angleStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat angle = 0.0f;

                    //Converte as strings para float.
                    angle = stof(angleStr);

                    //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                    {
                        //Caso não tenha colidido
                    
                        geometry* geo = w->getSelectedShape();
                        geo->rotate(angle,Y_AXIS);

                        w->getMenu()->clear(); //Limpa os dados do menu
                        w->setMenu(5, HIDDEN);//Seta o menu para 5.
                    }
                }
            }break;
        
            case 3: //Botão Cancelar;
                w->setMenu(5);//Seta o menu para 5.
            break;
        }
    }
}

void menuClickTrans(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 8: //Botão Transladar
            {
                std::string dxStr = (w->getMenu())->getTextInput(2); //Obtém o texto da caixa de texto 2.
                std::string dyStr = (w->getMenu())->getTextInput(4); //Obtém o texto da caixa de texto 2.
                std::string dzStr = (w->getMenu())->getTextInput(6); //Obtém o texto da caixa de texto 2.

                if(dxStr.size()!= 0 && dyStr.size()!= 0 && dzStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat dx = 0.0f, dy = 0.0f, dz = 0.0f;

                    //Converte as strings para float.
                    dx = stof(dxStr);
                    dy = stof(dyStr);
                    dz = stof(dzStr);

                    //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                    {
                        //Caso não tenha colidido
                    
                        geometry* geo = w->getSelectedShape();
                        geo->translate(dx, dy, dz);

                        w->getMenu()->clear(); //Limpa os dados do menu
                        w->setMenu(5, HIDDEN);//Seta o menu para 5.
                    }
                }
            }break;
        
            case 7: //Botão Cancelar;
                w->setMenu(5);//Seta o menu para 5.
            break;
        }
    }
}

void menuClickScale(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 4: //Botão Escala
            {
                std::string scaleStr = (w->getMenu())->getTextInput(2); //Obtém o texto da caixa de texto 2.

                if(scaleStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat scale = 0.0f;

                    //Converte as strings para float.
                    scale = stof(scaleStr);

                    if(scale<0.0001f)
                    {
                        w->showPopUp("Escala deve ser > 0");
                    }else
                    {
                        //if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                        {
                            //Caso não tenha colidido
                            geometry* geo = w->getSelectedShape();
                            geo->scale(scale, scale, scale);

                            w->getMenu()->clear(); //Limpa os dados do menu
                            w->setMenu(5, HIDDEN);//Seta o menu para 5.
                        }
                    }
                }
            }break;
        
            case 3: //Botão Cancelar;
                w->setMenu(5);//Seta o menu para 5.
            break;
        }
    }
}

