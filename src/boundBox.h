#ifndef BOUND_BOX_H
#define BOUND_BOX_H

#include "matrix.h"
#include "geometry.h"
#include<vector>

//Cor inicial da caixa de colisão
#define BB_R 0.000f
#define BB_G 0.6235f
#define BB_B 0.7176f 

/**
 * @brief Código fonte para o o shader de vértices
 * 
 */
const std::string boxVertexShader = 
        "#version 330 core\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main(){\n"
        "gl_Position = projection*view*model*vec4(pos*0.9999f, 1.0f);\n"
        "}\n";

/**
 * @brief Código fonte para o shader de fragmentos.
 * 
 */
const std::string boxFragmentShader = 
        "#version 330 core\n"
        "uniform vec4 color;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "FragColor = color;\n"
        "}\n";



class boundBox : public geometry
{
    private:
        vec3 max;
        vec3 min;

        matrix modelMatrix;
        matrix viewMatrix;
        matrix projectionMatrix;

        std::vector<int> indices;

        GLuint modelLoc;
        GLuint viewLoc;
        GLuint projectionLoc;
        GLuint colorLoc;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;

        static shaderProgram* program;
        static int boxCount;

        void setupBuffers();
        void updateBuffers();

    public:
        boundBox();
        virtual ~boundBox();

        void update(std::vector<GLfloat> &v, std::vector<GLfloat> &center , matrix &model);
        void updateVertices();
        vec3 getMax();
        vec3 getMin();

        void setProjection(const matrix &projectionMatrix);
        void setView(const matrix &viewMatrix);

        virtual void draw();

};

#endif //BOUND_BOX_H