#include "renderer.h"
#include <QTextStream>
#include <QOpenGLBuffer>
#include <cmath>


// constructor
Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent), m_mode(FACE), rotX(1.0), rotY(1.0), rotZ(1.0), theta(0.0), alpha(0.0), beta(0.0), toScale(false), modelScale(1.0), gameStart(true)
{
    m_pGame = new Game(10, 24);     //game object
    view_matrix.translate(0.0f, 0.0f, -40.0f);

    this->gTimer = new QTimer(this);  //timer object
    connect(this->gTimer, SIGNAL(timeout()), this, SLOT(timer_tick()));
    this->setSpeedInterval(500); // 500 ms

}

// constructor
Renderer::~Renderer()
{

}

// called once by Qt GUI system, to allow initialization for OpenGL requirements
void Renderer::initializeGL()
{
    // Qt support for inline GL function calls
	initializeOpenGLFunctions();
    glDisable(GL_CULL_FACE);

    // sets the background clour
    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // links to and compiles the shaders, used for drawing simple objects
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "per-fragment-phong.vs.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "per-fragment-phong.fs.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("position_attr");
    m_colAttr = m_program->attributeLocation("colour_attr");
    m_norAttr = m_program->attributeLocation("normal_attr");
    m_PMatrixUniform = m_program->uniformLocation("proj_matrix");
    m_VMatrixUniform = m_program->uniformLocation("view_matrix");
    m_MMatrixUniform = m_program->uniformLocation("model_matrix");
    m_programID = m_program->programId();
    generateBorderTriangles();
    generateBox();
}

// called by the Qt GUI system, to allow OpenGL drawing commands
void Renderer::paintGL()
{

    // Clear the screen buffers

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the current shader program

    glUseProgram(m_programID);

    // Modify the current projection matrix so that we move the
    // camera away from the origin.  We'll draw the game at the
    // origin, and we need to back up to see it.


    if(keepRotate)
        {

            if(rotate_horizontal){

                view_matrix.rotate(theta, 1.0f,0.0f,0.0f);      //rotate matrix about theta

            }

            if(rotate_vertical){
                 view_matrix.rotate(alpha,0.0f,1.0f,0.0f);  //rotate matrix about alpha

            }

            if(rotate_3d){
                view_matrix.rotate(beta, 0.0f, 0.0f, 1.0f);  //rotate matrix about beta

            }

        } //end of keepRotate

    glUniformMatrix4fv(m_VMatrixUniform, 1, false, view_matrix.data());
    // Not implemented: set up lighting (if necessary)
    // Not implemented: scale and rotate the scene
    QMatrix4x4 model_matrix;

    // You'll be drawing unit cubes, so the game will have width
    // 10 and height 24 (game = 20, stripe = 4).  Let's translate
    // the game so that we can draw it starting at (0,0) but have
    // it appear centered in the window.

    model_matrix.translate(-5.0f, -12.0f, 0.0f);
    glUniformMatrix4fv(m_MMatrixUniform, 1, false, model_matrix.data());

    if (m_mode == WIRE) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);   //Wire frame mode draw
        this->update();
      }
      else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );   //Face mode draw
        this->update();
      }

    // Not implemented: actually draw the current game state.
    // Here's some test code that draws red triangles at the
    // corners of the game board.

        // draw border
        if (triVertices.size() > 0)
        {
            // pass in the list of vertices and their associated colours
            // 3 coordinates per vertex, or per colour
            glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, &triVertices[0]);
            glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, &triColours[0]);
            glVertexAttribPointer(m_norAttr, 3, GL_FLOAT, GL_FALSE, 0, &triNormals[0]);

            glEnableVertexAttribArray(m_posAttr);
            glEnableVertexAttribArray(m_colAttr);
            glEnableVertexAttribArray(m_norAttr);

            // draw triangles
            glDrawArrays(GL_TRIANGLES, 0, triVertices.size()/3); // 3 coordinates per vertex

            glDisableVertexAttribArray(m_norAttr);
            glDisableVertexAttribArray(m_colAttr);
            glDisableVertexAttribArray(m_posAttr);
        }

        //box + Wall

        for(int i=0; i < 10; i++)
        {

                QMatrix4x4 cube_matrix(model_matrix);
                glEnableVertexAttribArray(m_posAttr);
                glEnableVertexAttribArray(m_norAttr);
                glVertexAttribPointer(m_posAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                glVertexAttribPointer(m_norAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                cube_matrix.translate( (float) i, -1.0, 0.0f);
                glUniformMatrix4fv(m_MMatrixUniform,1,false,cube_matrix.data());
                glDrawArrays(GL_TRIANGLES,0,boxVertices.size()/3);
                glFinish();

       }

       for(int i=0; i < 24; i++)
       {
           QMatrix4x4 cube_matrix(model_matrix);
           glEnableVertexAttribArray(m_posAttr);
           glEnableVertexAttribArray(m_norAttr);
           glVertexAttribPointer(m_posAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
           glVertexAttribPointer(m_norAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
           cube_matrix.translate( -1, (float) i-1, 0.0f);
           glUniformMatrix4fv(m_MMatrixUniform,1,false,cube_matrix.data());
           glDrawArrays(GL_TRIANGLES,0,boxVertices.size()/3);
           glFinish();
       }

       for(int i=0; i < 24; i++)
       {
           QMatrix4x4 cube_matrix(model_matrix);
           glEnableVertexAttribArray(m_posAttr);
           glEnableVertexAttribArray(m_norAttr);
           glVertexAttribPointer(m_posAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
           glVertexAttribPointer(m_norAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
           cube_matrix.translate( 10.0, (float) i-1, 0.0f);
           glUniformMatrix4fv(m_MMatrixUniform,1,false,cube_matrix.data());
           glDrawArrays(GL_TRIANGLES,0,boxVertices.size()/3);
           glFinish();
       }
// well ends

       //game play
       for(int c = 0; c < 24; c++){
           for(int r = 0; r < 10; r++)
           {
                int block = m_pGame->get(c, r);
                if(block >= 0)
                {

                    if(m_mode == MULTI)
                    {
                         QMatrix4x4 matrix;
                         glEnableVertexAttribArray(m_posAttr);                       
                         glEnableVertexAttribArray(m_norAttr);
                         glVertexAttribPointer(m_posAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                         glVertexAttribPointer(m_norAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                         matrix.translate(-5+r,-12+c,0);
                         glUniformMatrix4fv(m_MMatrixUniform,1,false, matrix.data());
                         glDrawArrays(GL_TRIANGLES,6,boxVertices.size()/3);
                         glFinish();
                  } //end of MULTI if

                    if(m_mode == FACE)
                    {
                        switch(block) {
                                  case 0:
                                    setNewColor(Qt::red);  //set red
                                    break;
                                  case 1:
                                    setNewColor(Qt::green); //set green
                                    break;
                                  case 2:
                                    setNewColor(Qt::blue); //set blue
                                    break;
                                  case 3:
                                    setNewColor(Qt::cyan); //set cyan
                                    break;
                                  case 4:
                                    setNewColor(Qt::magenta); //set magenta
                                    break;
                                  case 5:
                                    setNewColor(Qt::yellow); //set yellow
                                    break;
                                  case 6:
                                    setNewColor(Qt::green); //set green
                                    break;
                                  default:
                                    setNewColor(Qt::black); //set black
                        }
                        QMatrix4x4 matrix;
                        glEnableVertexAttribArray(m_posAttr);
                        glEnableVertexAttribArray(m_colAttr);
                        glEnableVertexAttribArray(m_norAttr);
                        glVertexAttribPointer(m_posAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, &boxColours[0]);
                        glVertexAttribPointer(m_norAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                        matrix.translate(-5+r,-12+c,0);
                        glUniformMatrix4fv(m_MMatrixUniform,1,false, matrix.data());
                        glDrawArrays(GL_TRIANGLES,0,boxVertices.size()/3);
                        glDisableVertexAttribArray(m_colAttr);
                        glFinish();
                    } //end of FACE if

                  if(m_mode == WIRE)
                  {
                      QMatrix4x4 matrix;
                      glEnableVertexAttribArray(m_posAttr);
                      glEnableVertexAttribArray(m_norAttr);
                      glVertexAttribPointer(m_posAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                      glVertexAttribPointer(m_norAttr,3,GL_FLOAT,GL_FALSE,0,&boxVertices[0]);
                      matrix.translate(-5+r,-12+c,0);
                      glUniformMatrix4fv(m_MMatrixUniform,1,false, matrix.data());
                      glDrawArrays(GL_TRIANGLES,0,boxVertices.size()/3);
                      glFinish();

                  } //end of WIRE if
                }//end of if block >0
            }//end of for loop for row
       } //end of for loop for columns

    m_program->release();
} //end of paintGL

// called by the Qt GUI system, to allow OpenGL to respond to widget resizing
void Renderer::resizeGL(int w, int h)
{
    // width and height are better variables to use
    Q_UNUSED(w); Q_UNUSED(h);

    // update viewing projections
    glUseProgram(m_programID);

    // Set up perspective projection, using current size and aspect
    // ratio of display
    QMatrix4x4 projection_matrix;
    projection_matrix.perspective(40.0f, (GLfloat)width() / (GLfloat)height(),
                                  0.1f, 1000.0f);
    glUniformMatrix4fv(m_PMatrixUniform, 1, false, projection_matrix.data());

    glViewport(0, 0, width(), height());

}

// computes the vertices and corresponding colours-per-vertex for a quadrilateral
// drawn from (x1, y1) to (x2, y2)
// Note: the magic numbers in the vector insert commands should be better documented
void Renderer::generateBorderTriangles()
{
    // make sure array lists are clear to start with
    triVertices.clear();
    triColours.clear();

    // add vertices to rectangle list
    float vectList [] = {
        0.0, 0.0, 0.0,  // bottom left triangle
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,

        9.0, 0.0, 0.0,  // bottom right triangle
        10.0, 0.0, 0.0,
        10.0, 1.0, 0.0,

        0.0, 19.0, 0.0, // top left triangle
        1.0, 20.0, 0.0,
        0.0, 20.0, 0.0,

        10.0, 19.0, 0.0,    // top right triangle
        10.0, 20.0, 0.0,
        9.0, 20.0, 0.0 };
    triVertices.insert(triVertices.end(), vectList, vectList + 3*4*3); // 36 items in array

    // shader supports per-vertex colour; add colour for each vertex add colours to colour list - use current colour
    QColor borderColour = Qt::red;
    float colourList [] = { (float)borderColour.redF(), (float)borderColour.greenF(), (float)borderColour.blueF() };
    float normalList [] = { 0.0f, 0.0f, 1.0f }; // facing viewer
    for (int v = 0; v < 4 * 3; v++)
    {
        triColours.insert(triColours.end(), colourList, colourList + 3); // 3 coordinates per vertex
        triNormals.insert(triNormals.end(), normalList, normalList + 3); // 3 coordinates per vertex
    }

}

void Renderer::generateBox(){
    boxColours.clear();

    //vertices list for cube PROVIDED BY TA of TUTORIAL 4
    float vertList [] =
    {
        ////////Front////////
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,

        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        ////////Right////////
        1.0, 0.0, 0.0,
        1.0, 0.0, -1.0,
        1.0, 1.0, -1.0,

        1.0, 1.0, -1.0,
        1.0, 1.0, 0.0,
        1.0, 0.0, 0.0,

        ////////Back////////
        1.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        1.0, 1.0, -1.0,

        1.0, 1.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 1.0, -1.0,

        ////////Left////////
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, -1.0,

        0.0, 0.0, -1.0,
        0.0, 1.0, -1.0,
        0.0, 1.0, 0.0,

        ////////Top////////
        0.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, -1.0,

        1.0, 1.0, -1.0,
        0.0, 1.0, -1.0,
        0.0, 1.0, 0.0,

        ////////Bottom////////
        0.0, 0.0, 0.0,
        0.0, 0.0, -1.0,
        1.0, 0.0, -1.0,

        1.0, 0.0, -1.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 0.0
    };

    //insert vertices into boxVertices vector
    boxVertices.insert(boxVertices.end(),vertList,vertList+12*3*3);

// normal coordinates, PROVIDED BY TA of TUTORIAL 4
float normals[] =
    {
        0.0, 0.0, 1.0,//Front


        1.0, 0.0, 0.0,//Right

        0.0, 0.0, -1.0,//Back
        -1.0, 0.0, 0.0,//Left
        0.0, 1.0, 0.0,//Top
        0.0, 0.0, -1.0//Bottom
    };

//for loop to insert it for Box normal.
    for(unsigned int i = 0; i<6; ++i)
    {
            boxNormals.insert(boxNormals.end(), normals + 3*i, normals+ 3*i);
    }

}

// override mouse press event
void Renderer::mousePressEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";

    //value for scaling
    scalePosition = event->x();

        if(event->button() == 1)
        {
                rotate_horizontal = true;   //set rotate horizontal true
                rotX = event->x();          //saves initial x coordinate into rotX

                /*This function basically says that we will rotate it about X axis.*/
        }

        if(event->button() == 4)
        {
                rotate_vertical = true;     //set rotate vertical true
                rotY = event->x();          //saves initial x coordinate into rotY

                /*This function basically says that we will rotate it about Y axis.*/
        }

        if(event->button() == 2)
        {
                rotate_3d = true;           //set rotate Z to true
                rotZ = event->x();          //saves initial x coordinate into rotZ


                /*This function basically says that we will rotate it about Z axis.*/
        }

} //end of mouse press

// override mouse release event
void Renderer::mouseReleaseEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";
    if(event->button() == 1)
        {
             keepRotate = true;     //keep rotate to true
             toScale = false;

             update();
        }

    if(event->button() == 4)
        {
             keepRotate = true;     //keep rotate to true
             toScale = false;

             update();
        }
    if(event->button() == 2)
       {
             keepRotate = true;     //keep rotate to true
             toScale = false;

             update();
       }

} //end of mouse Release event

// override mouse move event
void Renderer::mouseMoveEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Motion at " << event->x() << ", " << event->y() << ".\n";

    //is shift pressed? if yes then
        if(event->modifiers()&Qt::SHIFT){
            toScale = true;
                    int scaleVal = event->x() - scalePosition;   //save the  difference of position
                    modelScale = modelScale *(1.0+0.001*scaleVal);   //scale it.
                    if (modelScale > 1.3) {     //set boundarys max
                          modelScale = 1.3;

                        }
                        else if (modelScale < 0.7 ) {         //set boundary min
                          modelScale = 0.7;

                        }
                    view_matrix.scale(1.0+0.001*scaleVal,1.0+0.001*scaleVal,1.0+0.001*scaleVal);    //scale the matrix
        } //end of shit if

        //is rotate horizontal true?
        if((!toScale) && rotate_horizontal)
        {
            theta = (event->x() - rotX) * 0.007;        //save the  difference of position * 0.007 for lowering the difference


            view_matrix.rotate(theta, 1.0f,0.0f,0.0f);  //rotate the matrix

        } //end of rotate horizontal if

        // is rotate vertical true?
        else if((!toScale) && rotate_vertical)
        {
            alpha = (event->x() - rotY) * 0.007;        //save the  difference of position * 0.007 for lowering the difference


            view_matrix.rotate(alpha, 0.0f,1.0f,0.0f);  //rotate the matrix

        }//end of rotate vertical

        // is rotate 3d true
        else if((!toScale) && rotate_3d)
        {
            beta = ( event->x() - rotZ) * 0.007;        //save the  difference of position * 0.007 for lowering the difference

            view_matrix.rotate(beta, 0.0f,0.0f,1.0f);   //rotate the matrix

        } //end of rotate 3d

    update(); //updates the screen
} //end if mouse move

void Renderer::timer_tick(){
    m_pGame->tick();        //calls tick. Heart of game play
    update();
}

//sets draw mode
void Renderer::setMode(DrawMode m){
    m_mode = m;
    repaint();

}

//sets wire mode
void Renderer::setWireMode(){

        setMode(WIRE);

}
//sets face mode
void Renderer::setFaceMode(){

    setMode(FACE);
}
//sets Multi mode
void Renderer::setMultiMode(){

    setMode(MULTI);

}
//new game
void Renderer::newGame(){

    m_pGame->reset();
    setSpeedInterval(500);
    gameStart = true;
}
// resets the view of the matrix
void Renderer::resetView(){

    keepRotate = false;
    rotate_horizontal = false;
    rotate_vertical = false;
    rotate_3d = false;
    toScale = false;
    rotX = 0.0;
    rotY = 0.0;
    rotZ = 0.0;
    theta = 0.0;
    alpha = 0.0;
    beta = 0.0;
    modelScale = 1.0;
    view_matrix.setToIdentity();        //sets view matrix back to identity
    view_matrix.translate(0.0f, 0.0f, -40.0f);  //translate it to original position
    update();
}
//pauses the game
void Renderer::pauseGame()
{
    gTimer->stop();
}
//sets speed interval
void Renderer::setSpeedInterval(int s)
{
        speedOfPiece = s;
        gTimer->start(s);

}
//move piece to left
bool Renderer::moveLeft()
{
    bool r = false;
    if(gameStart)
        r= m_pGame->moveLeft();


    return r;
}
//move piece to right
bool Renderer::moveRight()
{
    bool r = false;
    if(gameStart)
        r = m_pGame->moveRight();
    return r;
}
//drops the piece
bool Renderer::drop()
{
    bool r = false;
    if(gameStart)
        r = m_pGame->drop();
    return r;
}
//rotate the piece clock wise
bool Renderer::rotateCW()
{    // game control
    bool r = false;
    if(gameStart)
        r = m_pGame->rotateCW();
    return r;
}
//rotates the piece counter clock wise
bool Renderer::rotateCCW()
{
    bool r = false;
    if(gameStart)
        r = m_pGame->rotateCCW();
    return r;
}
//sets in the color provided to in in argument and inserts it to boxColor vector
void Renderer::setNewColor(QColor x)
{
    boxColours.clear();
    QColor boxColour = x;
    float colourList [] = { (float)boxColour.redF(), (float)boxColour.greenF(), (float)boxColour.blueF()};

    for(unsigned int i = 0; i<6; ++i)
    {
        boxColours.insert(boxColours.end(), colourList, colourList + 3*i); // 3 coordinates per vertex
    }


}

