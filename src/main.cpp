/**
 * @file main.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Função principal, inicializa o programa.
 * @version 0.1
 * @date 2022-09
 */

#include"GL/glew.h"
#include"GL/freeglut.h"
#include<iostream>
#include<glm/glm.hpp>
#include"window.h"



int main(int argc, char** argv)
{
    try{
        //Cria uma janela com tamanho inicial 250x250
        window a(800,800, std::string("Hello Solids").c_str(), &argc, argv);
        //Inicializa a janela e a exibe na tela
        a.init(); 
    }catch(std::string b){
        //Exibe as exceções no terminal
        std::cout<<b<<std::endl;
    };

    return 0;
}