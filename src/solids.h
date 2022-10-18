#ifndef SOLIDS_H
#define SOLIDS_H

#define _USE_MATH_DEFINES

#include"matrix.h"
#include"shaderProgram.h"
#include"geometry.h"
#include"boundBox.h"
#include<cmath>
#include<GL/glew.h>
#include<cstddef>
#include<vector>
#include<string>

#define VPOS 0
#define COLOR 1

//Cor inicial do Aramado
#define WF_R 1.000f
#define WF_G 0.000f
#define WF_B 0.000f 

//Resolução torus
#define HDIV 16
#define VDIV 16
#define HRES 2*M_PI/float(HDIV)
#define VRES 2*M_PI/float(VDIV)


typedef enum {X_AXIS=0, Y_AXIS, Z_AXIS}AXIS;

/**
 * @brief Código fonte para o o shader de vértices
 * 
 */
const std::string solidVertexShader = 
        "#version 330 core\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout (location = 1) in vec3 normals;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "void main(){\n"
        "gl_Position = projection*view*model*vec4(pos, 1.0f);\n"
        "Normal = mat3(transpose(inverse(model)))*normals;\n"
        "FragPos = vec3(model*vec4(pos,1.0f));\n"
        "}\n";

/**
 * @brief Código fonte para o shader de fragmentos.
 * 
 */
const std::string solidFragmentShader = 
        "#version 330 core\n"
        "uniform vec3 color;\n"
        "uniform vec3 lightPos;\n"
        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "float ambient = 0.2;\n"
        "vec3 norm = normalize(Normal);\n"
        "vec3 lightDir = normalize(lightPos - FragPos);\n"
        "float diff = max(dot(norm, lightDir), -dot(norm, lightDir));\n"
        "vec3 diffuse = vec3(diff,diff,diff)\n;"
        "vec3 reflectDir = reflect(-lightDir, norm);\n"
        "float spec = pow(max(dot(lightDir,reflectDir), 0.0f),32);\n"
        "vec3 specular = 0.4*vec3(spec,spec,spec);\n"
        "vec3 result = (ambient+diffuse+spec)*color;\n"
        "FragColor = vec4(result,1.0f);\n"
        "}\n";


std::vector<int> triangleDivision(std::vector<int>::iterator it, std::vector<GLfloat> &vertices, std::vector<bool> &visited);


class solid : public geometry
{

    protected:
        bool isWireFrame;
        bool isSolid;
        bool isBoundBox;

        GLuint EBO;
        GLuint VAO;
        GLuint positionVBO;
        GLuint normalVBO;

        matrix modelMatrix;
        matrix viewMatrix;
        matrix projectionMatrix;

        boundBox* box;

        std::vector<GLfloat> wireFrameColor;
        
        std::vector<int> indices;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> cameraPos;
        

        GLuint modelLoc;
        GLuint viewLoc;
        GLuint projectionLoc;
        GLuint colorLoc;
        GLuint lightPosLoc;


        static shaderProgram* program;
        static int solidCount;

        void setupBuffers();

    public:
        solid();
        virtual ~solid();

        void setProjection(const matrix &projectionMatrix);
        void setView(const matrix &viewMatrix);

        void setWireFrame(bool state);
        void setSolid(bool state);
        void setBoundBox(bool state);

        bool getWireFrameState();
        bool getSolidState();
        bool getBoundBoxState();

        void setColor(GLfloat r, GLfloat g,  GLfloat b, GLfloat a = 1.0f);
        void resetColor();

        void rotate(GLfloat degrees, AXIS a);
        void translate(GLfloat dx, GLfloat dy, GLfloat dz);
        void scale(GLfloat sx, GLfloat sy, GLfloat sz);

        bool collision(GLfloat x, GLfloat y, GLfloat z);
        bool collision(solid* other);

        void generateNormals();

        void setCameraPos(vec3 &cameraPos);

        boundBox* getBoundBox();

        virtual void draw();
};


class cube : public solid
{
    public:
        cube(GLfloat size, std::vector<GLfloat> &center);
};

class hexahedron : public solid
{
    public:
        hexahedron(GLfloat xSize, GLfloat ySize, GLfloat zSize, std::vector<GLfloat> &center);
};

class icosahedron : public solid
{
    public:
        icosahedron(GLfloat size, std::vector<GLfloat> &center);
};

class icosphere : public solid
{
    public:
        icosphere(GLfloat radius, std::vector<GLfloat> &center, int depth = 3);
        void subdivide(int depth);
};

class torus : public solid
{
    public:
        torus(GLfloat discRadius, GLfloat circleRadius, std::vector<GLfloat> &center);
};


#endif //SOLIDS_H