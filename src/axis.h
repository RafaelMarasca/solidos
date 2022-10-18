#ifndef AXIS_H
#define AXIS_H

#include "geometry.h"

/**
 * @brief Código fonte para o o shader de vértices
 * 
 */
const std::string axisVertexShader = 
        "#version 330 core\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 vColor;\n"
        "void main(){\n"
        "gl_Position = projection*view*vec4(pos*0.9999f, 1.0f);\n"
        "vColor = color;\n"
        "}\n";

/**
 * @brief Código fonte para o shader de fragmentos.
 * 
 */
const std::string axisFragmentShader = 
        "#version 330 core\n"
        "in vec3 vColor;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "FragColor = vec4(vColor, 1.0f);\n"
        "}\n";


class axis : public geometry
{
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;

        matrix viewMatrix;
        matrix projectionMatrix;
        
        std::vector<int> indices;

        GLuint viewLoc;
        GLuint projectionLoc;
        GLuint colorLoc;

        shaderProgram* program;

        void setup();

    public:
        axis();
        virtual ~axis();

        void setProjection(const matrix &projectionMatrix);
        void setView(const matrix &viewMatrix);

        void draw(); 

        void setupBuffers();

};


#endif //AXIS_H