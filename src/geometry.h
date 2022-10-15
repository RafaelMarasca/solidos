#ifndef GEOMETRY_H
#define GEOMETRY_H


#include"matrix.h"
#include"shaderProgram.h"
#include<GL/glew.h>
#include<cstddef>
#include<vector>
#include<string>
#include<glm/glm.hpp>



//Cor inicial da Forma
#define GEOMETRY_R 0.329f
#define GEOMETRY_G 0.341f
#define GEOMETRY_B 0.486f 

//Cor inicial do Aramado
#define WF_R 1.000f
#define WF_G 0.000f
#define WF_B 0.000f 

typedef enum {X_AXIS=0, Y_AXIS, Z_AXIS}AXIS;


/**
 * @brief Código fonte para o o shader de vértices
 * 
 */
const std::string vertexShaderSource = 
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
const std::string fragmentShaderSource = 
        "#version 330 core\n"
        "uniform vec4 color;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "FragColor = color;\n"
        "}\n";

class geometry
{

    private:
        static unsigned int geometryCount;
        void setup();

    protected:
        void setupCollisionBox();

        bool wireFrame;
        bool solid;
        
        static shaderProgram* program;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;

        matrix modelMatrix;
        matrix viewMatrix;
        matrix projectionMatrix;

        std::vector<GLfloat> centralPoint; //Ponto central da forma
        std::vector<GLfloat> vertices; //Vetor de vértices da forma
        std::vector<GLfloat> color;
        std::vector<GLfloat> wireFrameColor;
        std::vector<int> indices;

        matrix collisionLB;
        matrix collisionRT;
        

        GLuint colorLoc;
        GLuint modelLoc;
        GLuint viewLoc;
        GLuint projectionLoc;

        GLenum usage;        

    public:
        geometry(std::vector<GLfloat> &vertices, std::vector<int> &indices, std::vector<GLfloat> &centralPoint, GLenum usage = GL_STATIC_DRAW); //Construtor para geometry
        geometry(GLenum usage = GL_STATIC_DRAW);
        virtual ~geometry(); //Destrutor para geometry

        void rotate(GLfloat degrees, AXIS a);
        void translate(GLfloat dx, GLfloat dy, GLfloat dz);
        void scale(GLfloat sx, GLfloat sy, GLfloat sz);

        void setProjection(matrix projectionMatrix);
        void setView(const matrix &viewMatrix);
        void setColor(GLfloat r, GLfloat g,  GLfloat b, GLfloat a = 1.0f);
        
        void setWireFrame(bool state);
        void setSolid(bool state);
        bool getWireFrameState();
        bool getSolidState();

        virtual bool collision(GLfloat x, GLfloat y, GLfloat z);
        virtual bool collision(geometry* other);

        virtual void resetColor();
        virtual void draw(); //Método virtual puro para desenho da forma.

};

class cube : public geometry
{
    public:
        cube(GLfloat size, std::vector<GLfloat> &center, GLenum usage = GL_STATIC_DRAW);
};

class hexahedron : public geometry
{
    public:
        hexahedron(GLfloat xSize, GLfloat ySize, GLfloat zSize, std::vector<GLfloat> &center, GLenum usage = GL_STATIC_DRAW);
};


class icosahedron : public geometry
{
    public:
        icosahedron(GLfloat size, std::vector<GLfloat> &center, GLenum usage = GL_DYNAMIC_DRAW);
};



class icosphere : public geometry
{
    public:
        icosphere(GLfloat radius, std::vector<GLfloat> &center, int depth = 3, GLenum usage = GL_DYNAMIC_DRAW);
        void subdivide(int depth);
};

class torus : public geometry
{
    public:
        torus(GLfloat discRadius, GLfloat circleRadius, std::vector<GLfloat> &center, GLenum usage = GL_STATIC_DRAW);
};

class boundBox : public geometry
{

};



#endif //GEOMETRY_H