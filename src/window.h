/**
 * @file window.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Declaração da classe window e classes associadas.
 * @version 0.1
 * @date 2022-09
 * 
 */

#ifndef WINDOW_H
#define WINDOW_H

#include<GL/glew.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include"scene.h"
#include"geometry.h"
#include<string>
#include<vector>
#include<map>
#include<utility>
#include"gui.h"
#include"camera.h"


/**
 * @brief Cor da caixa de seleção.
 */
#define SELECTION_R 0.6316f
#define SELECTION_G 0.0549f
#define SELECTION_B 0.7882f

/**
 * @enum inputType
 * @brief Representação do dispositivo de entrada atual.
 * @var NONE: Nenhum dispositivo de entrada está sendo requerido.
 * @var MOUSE: Define o mouse como dispositivo de entrada atual.
 * @var KEYBOARD: Define o teclado como dispositivo de entrada atual.
 */
typedef enum{NONE= 0, MOUSE = 1, KEYBOARD = 2}inputType;

/**
 * @enum menuState
 * @brief Representação do estado do menu atual.
 * @var HIDDEN: Menu atual não está sendo exibido.
 * @var VISIBLE: Menu atual está sendo exibido.
 */
typedef enum{HIDDEN = 0, VISIBLE = 1}menuState;


/**
 * @brief Classe window - Define uma abstração para uma janela de desenho.
 */
class window
{
    private:
        scene* vision; //Ponteiro para a cena da janela.
        solid* selectedShape; //Ponteiro para a forma selecionada.
        camera* cam;

        inputType inType; //Tipo de entrada atual.

        GLfloat aspectRatio; //Razão de aspecto da tela.

        unsigned int selectedShapeID; //ID da forma selecionada.

        sceneIterator selIterator;

        int waitingInput; //Quantidade de entradas que a janela está esperando.
        int width,height; //Altura e largura da tela.
        int currentMenu; //ID do menu atual.

        std::map<int,frame*> menu; //Armazena os menus e os correspondentes IDs.
        std::pair<int,bool> tempMenu; //Armazenamento temporário para lidar com popUps

    public:
        //Construtor para a classe window
        window(int width, int height, const char* title, int* argc, char** argv);
         
        ~window(); //Destrutor para a classe window.

        void input(); //Método de entrada de dados
        void init(); //Método de inicialização do tela
        void deleteShape(); //Meétodo para deletar uma forma
        void select(std::pair<unsigned int, solid*>); //Método para selecionar uma forma
        void clearSelection(); //Desseleciona a forma selecionada

        void updateSelection(); //Método de atualização da cena exibida na tela.

        void showPopUp(const char* text); //Mostra um popUp.
        void closePopUp(); //Fecha o popUp.

        void setMenu(int ID, menuState = VISIBLE, bool clear = 1); //Seta o menu atual.
        void addShape(solid* shape); //Adiciona uma forma.

        frame* getMenu(); //Getter para o menu atual.
        
        int getWaiting();
        void setWaiting(int waiting);
        void decWaiting();

        solid* getSelectedShape();

        scene* getScene();

        camera* getCam();

        void setInputType(inputType);
        inputType getInputType();

        static void draw(); //Método para desenhar os objetos da cena.
        static void mouseClick(int button, int state,int x, int y); //Método chamado ao clicar na tela.
        static void keyp(unsigned char key, int x, int y); //Método chamado ao pressionar uma tecla.
        static void specialKeyp(int key, int x, int y); //Método chamado ao pressionar uma tecla especial.
        static void resize(int w, int h); //Método chamado ao redimensionar a tela.
        static void mouseMove(int x, int y); //Método para lidar com o movimento do mouse.
};

#endif //WINDOW_H
