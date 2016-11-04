/*
 * CPSC 453 - Introduction to Computer Graphics
 * Assignment 3
 *
 * objModel - Class that handles and stores an OBJ model
 *
 * Note: The provided files are based off MD2 models, which have
 * specific animations as listed. A solitary OBJ file is not
 * animated, but the series of files provided have been generated
 * for each frame of the animation, in order
 */
#ifndef OBJMODEL_H
#define OBJMODEL_H
#include <QOpenGLFunctions>
#include <glm/glm.hpp>
#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QImage>

using namespace std;


// triangle
typedef struct
{
    GLuint index_xyz[3];    // indexes to triangle's vertices
    GLuint index_uv[3];     // indexes to triangles's coordinates
} tri;

// animation
typedef struct
{
        int first_frame;    // first frame of the animation
        int last_frame;     // number of frames
        int fps;            // number of frames per second

} anim_t;

// animation list
typedef enum {
        STAND,
        RUN,
        ATTACK,
        PAIN_A,
        PAIN_B,
        PAIN_C,
        JUMP,
        FLIP,
        SALUTE,
        FALLBACK,
        WAVE,
        POINT,
        CROUCH_STAND,
        CROUCH_WALK,
        CROUCH_ATTACK,
        CROUCH_PAIN,
        CROUCH_DEATH,
        DEATH_FALLBACK,
        DEATH_FALLFORWARD,
        DEATH_FALLBACKSLOW,
        BOOM,

        MAX_ANIMATIONS

} animType_t;



class ObjModel : protected QOpenGLFunctions {
    public:
        // constructor, destructor
        ObjModel ();
        ObjModel (const char *filename);
        virtual ~ObjModel() {};

        // loading model
        // TODO: You may want to update the LoadModel function to scale the model
        // to within a visible range
        bool LoadModel(const char *filename);

        int num_xyz;        // number of vertices
        int num_tris;       // number of triangles

        int skin_width;     // width of texture
        int skin_height;    // height of texture image

        vector<glm::vec3> m_vertices; // vertex array
        vector<glm::vec2> texs;  // texture coordinates
        vector<float> vertDraw; //vertex array which will be passed to opengl
        vector<float> colors; //colors of the model
        vector<float> faceNormals; //array of face normals
        vector<float> vertexNormals; //array of vertex normals
        vector<float> uvDraw;
        vector<tri> tris;   // triangles

        static anim_t animlist[21]; // static array, listing the animations in the provided models

        void draw0();
        void draw1();
        void draw2();
        void drawWithUV();
        void drawWithUV2();
        void setupObject();
        void setupObject2();
        void setupObjectUV2(QString texture);
        void setupObjectUV(QString texture);
        void setAttributes(GLuint m_posAttr, GLuint m_colAttr, GLuint m_norAttr);
        void setAttributesUV(GLuint m_posAttr, GLuint m_colAttr, GLuint m_norAttr, GLuint m_uvAttr, GLuint m_TextureUniform);


    private:
        void calculateFaceNormal(tri t);
        void addToVerticesArray(tri t);
        void addToUVArray(tri t);
        void colorTheObject(float r, float g, float b);
        void calculateVertexNormals();


        GLuint m_vbo;         // VBO for the model
        GLuint m_TextureUniform;
        GLuint m_testTexture;
        GLuint m_posAttr;
        GLuint m_colAttr;
        GLuint m_norAttr;
        GLuint m_uvAttr;

};

#endif
