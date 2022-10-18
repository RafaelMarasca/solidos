#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<string>
#include"shaderProgram.h"

class graphics
{
    protected:
        std::string vertexShader;
        std::string fragmentShader;
        shaderProgram* program;


    public:
        graphics(std::string vertexShader, std::string fragmentShader);
    
};





#endif //GRAPHICS_H