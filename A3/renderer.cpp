#include "renderer.h"
#include <QTextStream>
#include <QOpenGLBuffer>
#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

using namespace glm;
// constructor
Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mode = WIRED_RENDERER;

    isTextured = true;

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
        // sets the background clour
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // links to and compiles the shaders, used for drawing simple objects
        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "textured-phong.vs.glsl");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "textured-phong.fs.glsl");
        m_program->link();
        m_posAttr = m_program->attributeLocation("position_attr");
        m_colAttr = m_program->attributeLocation("colour_attr");
        m_norAttr = m_program->attributeLocation("normal_attr");
        m_uvAttr = m_program->attributeLocation("texcoords_attr");
        m_PMatrixUniform = m_program->uniformLocation("proj_matrix");
        m_VMatrixUniform = m_program->uniformLocation("view_matrix");
        m_MMatrixUniform = m_program->uniformLocation("model_matrix");
        m_TextureUniform = m_program->uniformLocation("texObject");

        m_programID = m_program->programId();

        glGenTextures(1, &m_boardTexture); // Generate a texture handle
           glActiveTexture(GL_TEXTURE0); // Make sure we're using texture unit 0
           glBindTexture(GL_TEXTURE_2D, m_boardTexture); // bind the texture handle

           //Load the image
           image2.load("checkered.jpg");
           image2 = image2.convertToFormat(QImage::Format_RGBA8888); // Convert it to a usable format

           // Write it to the GPU

           glTexImage2D(GL_TEXTURE_2D,
                        0,
                        GL_RGBA,
                        image2.width(),
                        image2.height(),
                        0,
                        GL_RGBA,
                        GL_UNSIGNED_INT_8_8_8_8_REV,
                        (const GLvoid *)image2.bits());

           // Set the filtering parameters
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

           generateBoard();


}

// called by the Qt GUI system, to allow OpenGL drawing commands
void Renderer::paintGL()
{
    // Clear the screen buffers

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set the current shader program
    //glRotatef(40,1,1,0);

    glUseProgram(m_programID);


    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_boardTexture);
        glUniform1i(m_TextureUniform, 0);  // Give it the 0'th texture unit

        //paint board
        if (boardVerts.size() > 0)
            {
                // pass in the list of vertices and their associated colours
                // 3 coordinates per vertex, or per colour
                glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, &boardVerts[0]);
                glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, &boardCol[0]);
                glVertexAttribPointer(m_norAttr, 3, GL_FLOAT, GL_FALSE, 0, &boardNormals[0]);
                glVertexAttribPointer(m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, &board_uvs[0]);

                glEnableVertexAttribArray(m_posAttr);
                glEnableVertexAttribArray(m_colAttr);
                glEnableVertexAttribArray(m_norAttr);
                glEnableVertexAttribArray(m_uvAttr);

                // draw triangles
                glDrawArrays(GL_TRIANGLES, 0, boardVerts.size()/3); // 3 coordinates per vertex

                glDisableVertexAttribArray(m_norAttr);
                glDisableVertexAttribArray(m_colAttr);
                glDisableVertexAttribArray(m_posAttr);
                glDisableVertexAttribArray(m_uvAttr);
            }

    // Modify the current projection matrix so that we move the
    // camera away from the origin.  We'll draw the model at the
    // origin, and we need to back up to see it or scale vertices
    // so it fits in the viewing area.


    QMatrix4x4 view_matrix;

    view_matrix.translate(x_tr, -5.0f+y_tr, -100.0f+z_tr);

    glUniformMatrix4fv(m_VMatrixUniform, 1, false, view_matrix.data());


    // Not implemented: set up lighting (if necessary)

    QMatrix4x4 model_matrix;

    // Not implemented: scale, rotate and translate the scene
    model_matrix.translate(0.0f, 0.0f, -100.0f);

    glUniformMatrix4fv(m_MMatrixUniform, 1, false, model_matrix.data());



    // Not implemented: actually draw our character
    // Here's some test code that draws a simple triangle at the center

    renderObject();


    m_program->release();

}

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

// override mouse press event
void Renderer::mousePressEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
        cout << "Stub: Button " << event->button() << " pressed.\n";

           buttonPressed = event->button();
           buttonPressStart = event->x();

}

// override mouse release event
void Renderer::mouseReleaseEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";

}

// override mouse move event
void Renderer::mouseMoveEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
        cout << "Stub: Motion at " << event->x() << ", " << event->y() << ".\n";
    if (buttonPressed == 4){
            z_tr += event->x() - buttonPressStart;
            buttonPressStart = event->x();
        } else if (buttonPressed == 1) {
            x_tr += event->x() - buttonPressStart;
            buttonPressStart = event->x();
        }else if (buttonPressed == 2) {
            y_tr += event->x() - buttonPressStart;
            buttonPressStart = event->x();
        }
        update();
}

void Renderer::openModel()
{
        filenameModel = QFileDialog::getOpenFileName();

        if(filenameModel.isNull())
        {
            return;
        }
        else {
            model = new ObjModel(filenameModel.toStdString().c_str());
            model->setAttributes(m_posAttr,m_colAttr,m_norAttr);
            models.push_back(*model);
            update();
        }


}

void Renderer::openSubModel()
{

        filenameModel = QFileDialog::getOpenFileName();
        if(filenameModel.isNull())
        {
            return;
        }
        else {
            model = new ObjModel(filenameModel.toStdString().c_str());
            model->setAttributes(m_posAttr,m_colAttr,m_norAttr);
            models.push_back(*model);
            update();
        }

}

void Renderer::renderObject()
{

    if( mode == WIRED_RENDERER)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for (int i=0; i<models.size(); i++)
        {
                models.at(i).draw0();
                update();
        }

    }

    if(mode == FACE_RENDERER)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

       if(isTextured == false)
        {
            for(int i=0; i<models.size(); i++)
            {
                models.at(i).setupObject();
                models.at(i).draw1();
                update();
            }
        }
      if(isTextured == true)
      {
        for(int i=0; i<models.size(); i++)
        {

            models.at(i).setAttributesUV(m_posAttr,m_colAttr,m_norAttr, m_uvAttr, m_TextureUniform);
            models.at(i).setupObjectUV(fileTexturedModel);
            models.at(i).drawWithUV();
            update();
        }
      }

    }


    if(mode == SMOOTH_RENDERER )
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(isTextured == false)
        {
            for (int i=0; i<models.size(); i++)
            {

                   models.at(i).setupObject2();
                   models.at(i).draw2();
                   update();
            }
        }
        if(isTextured == true)
        {
            for(int i=0; i<models.size(); i++)
            {
                    models.at(i).setAttributesUV(m_posAttr,m_colAttr,m_norAttr, m_uvAttr, m_TextureUniform);
                    models.at(i).setupObjectUV2(fileTexturedModel);
                    models.at(i).drawWithUV2();
                    update();
            }
        }

    }
}

void Renderer::openTextureModel()
{
        fileTexturedModel = QFileDialog::getOpenFileName();

        if(fileTexturedModel.isNull())
        {
            return;
        }
        else {
           isTextured = true;
           update();
        }

}

void Renderer::setWireType() {

        mode = WIRED_RENDERER;
        update();
}

void Renderer::setFaceType() {

        mode = FACE_RENDERER;
        update();
}

void Renderer::setSmoothType()
{
    mode = SMOOTH_RENDERER;
    update();
}

void Renderer::generateBoard()
{
    boardVerts.clear();
       boardNormals.clear();
       boardCol.clear();

       float vectList [] = {
           -75.0,  -20.0,  50.0,
           -75.0,  -20.0, -20.0,
            75.0,  -20.0, -20.0,

           -75.0,  -20.0,  50.0,
            75.0,  -20.0,  50.0,
            75.0,  -20.0, -20.0,
       };
       boardVerts.insert(boardVerts.end(), vectList, vectList + 3*2*3); // 18 items in array

       // shader supports per-vertex colour; add WHITE colour for each vertex
       float colourList [] = {
           1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,

           1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 1.0f
       };

       boardCol.insert(boardCol.end(), colourList, colourList + 3*2*3); // 18 items in array

       // shader supports per-vertex normals; add normal for each vertex - use same normal for each vertex (eg. face normal)
       float normalList [] = { 1.0f, 1.0f, 1.0f }; // facing viewer
       for (int v = 0; v < 4 * 3; v++)
       {
           boardNormals.insert(boardNormals.end(), normalList, normalList + 3); // 3 coordinates per vertex
       }

       vec2 p1 = vec2(0,0);
       vec2 p2 = vec2(0,1);
       vec2 p3 = vec2(1,0);
       vec2 p4 = vec2(1,1);

       //push the uvs
       board_uvs.push_back(p2);
       board_uvs.push_back(p1);
       board_uvs.push_back(p3);

       board_uvs.push_back(p2);
       board_uvs.push_back(p4);
       board_uvs.push_back(p3);
}


