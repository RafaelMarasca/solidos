/**
 * @file scene.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Declaração da classe scene
 * @version 0.1
 * @date 2022-09
 * 
 */

#ifndef SCENE_H
#define SCENE_H

#include<vector>
#include<string>
#include<map>
#include<utility>
#include<GL/glew.h>
#include"geometry.h"
#include"axis.h"
#include"matrix.h"

//Definição do tipo iterator para a classe scene
typedef std::map<unsigned int, geometry*>::iterator sceneIterator;

/**
 * @brief Classe scene - Uma cena é um conjunto de objetos geométricos a 
 * serem mostrados na tela
 * 
 */
class scene
{
    private:
        std::map<unsigned int,geometry*> objects; //Mapa de formas geométricas com seus respectivos IDs
        axis* theAxis; //Ponteiro para o eixo
        unsigned int nextID; //Valor que define o ID a ser atribuído à próxima forma a ser inserida na cena

    public:
        scene(); //Construtor para a classe scene
        ~scene(); //Destrutor para a classe scene

        std::pair<unsigned int, geometry*> getObject(unsigned int id); //Getter para um objeto da cena.
        std::pair<unsigned int, geometry*> checkCollision(float x, float y); //Checa colisão entre um objeto da cena e um ponto da tela

        geometry* removeObject(unsigned int id); //Remove um objeto da cena
       
        sceneIterator begin(); //Retorna um iterator para o elemento inicial da cena
        sceneIterator end(); //Retorna um iterator para o elemento final da cena

        void draw(); //Desenha todos os objetos da cena
        unsigned int addObject(geometry* object); //Adiciona um objeto na cena.
       
        std::pair<unsigned int, geometry*> checkCollision(geometry* geo); //Verifica se há colisão de um objeto da cena com um objeto passado por argumento

        void updateCam(const matrix &view, const matrix &projection);
};



#endif //SCENE_H
