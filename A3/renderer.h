/*
 * CPSC 453 - Introduction to Computer Graphics
 * Assignment 1
 *
 * Renderer - OpenGL widget for drawing scene
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <objModel.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QMouseEvent>
#include<QTimer>



using namespace std;
using namespace glm;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{

    // informs the qmake that a Qt moc_* file will need to be generated
    Q_OBJECT

public:
    // constructor
    Renderer(QWidget *parent = 0);

    // destructor
    virtual ~Renderer();

private slots:
    void openModel();
    void openSubModel();
    void openTextureModel();
    void setWireType();
    void setFaceType();
    void setSmoothType();

protected:

    // override fundamental drawing functions

    // Called when OpenGL is first initialized
    void initializeGL();

    // Called when the window is resized
    void resizeGL(int w, int h);

    // Called when the window needs to be redrawn
    void paintGL();

    // override mouse event functions

    // Called when a mouse button is pressed
    virtual void mousePressEvent(QMouseEvent * event);

    // Called when a mouse button is released
    virtual void mouseReleaseEvent(QMouseEvent * event);

    // Called when the mouse moves
    virtual void mouseMoveEvent(QMouseEvent * event);

private:
    QString filenameModel;
    QString fileTexturedModel;


    // member variables for shader manipulation
        GLuint m_programID;
        GLuint m_posAttr;
        GLuint m_colAttr;
        GLuint m_norAttr;
        GLuint m_uvAttr;

        GLuint m_TextureUniform;

        GLuint m_MMatrixUniform; // model matrix
        GLuint m_VMatrixUniform; // view matrix
        GLuint m_PMatrixUniform; // projection matrix;

        vector<GLfloat> boardVerts;
        vector<GLfloat> boardNormals;
        vector<GLfloat> boardCol;
        vector<vec2> board_uvs;

    vector<ObjModel> models;

    QOpenGLShaderProgram *m_program;

    // for storing triangle vertices and colours
    vector<GLfloat> triVertices;
    vector<GLfloat> triColours;
    vector<GLfloat> triNormals;

     ObjModel* model;

    static const int WIRED_RENDERER = 0;
    static const int FACE_RENDERER = 1;
    static const int SMOOTH_RENDERER = 2;

    bool isTextured;

        int buttonPressed;
            float buttonPressStart;
            float z_tr=0;
            float x_tr=0;
            float y_tr=0;
    QImage image2;
    GLuint m_boardTexture;
    int mode;

    // helper function for loading shaders
    GLuint loadShader(GLenum type, const char *source);


    // helper function for drawing bordering triangles
    void generateExampleTriangle();


    void setupVBO();

    void renderObject();

    void generateBoard();


};

#endif // RENDERER_H
