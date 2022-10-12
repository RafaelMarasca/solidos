/**
 * @file shaderProgram.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Declaração da classe shaderProgram
 * @version 0.1
 * @date 2022-09
 * 
 */

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include<GL/glew.h>
#include<string>
    
/**
 * @brief Classe shaderProgram - Define uma abstração para o programa de shader
 * 
 */
class shaderProgram
{
    private:
        GLuint shaderProgramId; //Armazena o id do programa compilado
    
        void compile(const char* vertexShaderSource, const char* fragmentShaderSource); //Compila o programa


    public:
        shaderProgram(const std::string vertexShaderSource, const std::string fragmentShaderSource); //construtor para a classe shaderProgram
        ~shaderProgram(); //Destrutor para a classe shaderProgram
        
        void use(); //Método para o uso do programa

        GLuint getLoc(const char* loc); //Getter para a localização de algum uniform;
};

#endif //SHADER_PROGRAM_H
