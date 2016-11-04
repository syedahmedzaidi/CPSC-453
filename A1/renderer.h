/*
 * CPSC 453 - Introduction to Computer Graphics
 * Assignment 1
 *
 * Renderer - OpenGL widget for drawing scene
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QMouseEvent>
#include <game.h>
#include <QTimer>
#include <QKeyEvent>
#include <QColor>
using namespace std;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{

    // informs the qmake that a Qt moc_* file will need to be generated
    Q_OBJECT

public:
    // constructor
    Renderer(QWidget *parent = 0);

    //Game &game() {return m_game; }

    // destructor
    virtual ~Renderer();

    //moves the piece to left
    bool moveLeft();

    //moves the piece to right
    bool moveRight();

    //drops the piece
    bool drop();

    //rotates the piece clock-wise
    bool rotateCW();

    //rotates the piece counter clock-wise
    bool rotateCCW();

    //sets the speed interval to the integer value
    void setSpeedInterval(int s);

    //pauses the game
    void pauseGame();

    //the speed which speed interval is set on
    int speedOfPiece;



public slots:

    //sets Wire Frame mode
    void setWireMode();

    //sets Face mode
    void setFaceMode();

    //sets Mult Color mode
    void setMultiMode();

    //creates a new game
    void newGame();

    //resets the view to original position
    void resetView();

private slots:

    // tick method
     void timer_tick();

protected:

    //serve as arguments to see what Mode
    enum DrawMode {
        WIRE, FACE, MULTI
      };

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

    Game *m_pGame;          // pointer to game class
    // member variables for shader manipulation
    GLuint m_programID;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_norAttr;
    GLuint m_MMatrixUniform; // model matrix
    GLuint m_VMatrixUniform; // view matrix
    GLuint m_PMatrixUniform; // projection matrix
    QOpenGLShaderProgram *m_program;

    // for storing triangle vertices and colours
    vector<GLfloat> triVertices;
    vector<GLfloat> triColours;
    vector<GLfloat> triNormals;

    // for storing box vertices and colours
    vector<GLfloat> boxVertices;
    vector<GLfloat> boxNormals;
    vector<GLfloat> boxColours;

    //Draw mode variable used to determine what mode it is
    DrawMode m_mode;

    // view matrix declared as member variable so it can be used throughout.
    QMatrix4x4 view_matrix;

    // boolean variable to see which axis is to be rotated on
    bool rotate_horizontal;
    bool rotate_vertical;
    bool rotate_3d;

    // variable used to hold initial x coordinate where mouse is pressed
    float rotX;
    float rotY;
    float rotZ;

    // rotation variables
    float theta;
    float alpha;
    float beta;

    bool keepRotate; // boolean value to see if we want to keep rotation. Persistance

    int scalePosition; //original x coordinate where mouse is pressed
    float modelScale;  //value on which scale will be performed on
    bool toScale;      //boolean value on which we will determine if we want to scale or not


    bool gameStart;   //boolean value to see if game is started or not

    QTimer* gTimer;     //Qtimer pointer that contains the time interval

    // helper function for loading shaders
    GLuint loadShader(GLenum type, const char *source);

    // helper function for drawing bordering triangles
    void generateBorderTriangles();

    // helper function for drawing cube
    void generateBox();

    //sets m_mode variable
    void setMode(DrawMode m);

    //sets new color into vector boxColours
    void setNewColor(QColor x);

};

#endif // RENDERER_H
