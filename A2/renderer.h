#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <math.h>
#include "algebra.h"
#include <QMatrix4x4>
#include <a2.h>


#define PI 4*atan(1)

// The "main" OpenGL widget
class Renderer : public QOpenGLWidget, protected QOpenGLFunctions {

    // informs the qmake that a Qt moc_* file will need to be generated
    Q_OBJECT

public:
    // constructor
    Renderer(QWidget *parent = 0);

    // destructor
    virtual ~Renderer();
    
    // Mode enum
	enum Mode {
        NOMODE,
		VIEWROTATE,
		VIEWTRANSLATE,
		VIEWPERSPECTIVE,
		MODELROTATE,
		MODELTRANSLATE,
		MODELSCALE,
		VIEWPORT
	};
	
    // sets the mode we are using
    void set_mode(Mode mode);


	// A useful function that forces this widget to rerender. If you
	// want to render a new frame, do not call paintGL
	// directly. Instead call this, which will cause an paintGL
	// call when the time is right.
	void invalidate();

	// *** Fill in these functions (in viewer.cpp) ***

    // Set the parameters of the current perspective projection using the semantics of gluPerspective().
    void set_perspective(double fov, double aspect,double near, double far);

    //sets rotation for View Interaction Mode
    void set_viewRotate();

    //sets Translation for View Interaction Mode
    void set_viewTranslate();

    //sets Prespective for View Interaction Mode
    void set_viewPrespective();

    //sets rotation for Model Interaction Mode
    void set_modelRotate();

    //sets Translation for Model Interaction Mode
    void set_modelTranslate();

    //sets scaling for Model Interaction Mode
    void set_modelScale();

    //sets Viewport Mode
    void set_viewPort();


public slots:

    //reset the application state
    void    reset();



protected:

	// Events we implement
	// Note that we could use Qt's "signals and slots" mechanism
	// instead, but for many classes there's a convenient member
	// function one just needs to define that'll be called with the
	// event.

	// override fundamental drawing functions

	// Called when OpenGL is first initialized
	void initializeGL();

	// Called when the window needs to be redrawn
	void paintGL();

	// Called when the window is resized
	void resizeGL(int w, int h);

	// override mouse event functions

	// Called when a mouse button is pressed
	virtual void mousePressEvent(QMouseEvent * event);

	// Called when a mouse button is released
	virtual void mouseReleaseEvent(QMouseEvent * event);

	// Called when the mouse moves
    virtual void mouseMoveEvent(QMouseEvent * event);


private:

    Triangle m_demoTriangle;

	// *** Fill me in ***
	// You will want to declare some more matrices here

    void initializeEverything();

    //initilizes cube
    void initializeCube();

    //draw the cube
    void unitCube();

    void initializeGnomon();

    //draw the model gnomon
    void modelGnomon();

    void worldGnomon();

	// Used to draw a 3D line in the 2D window
    void draw_line2D( Point3D left, Point3D right );

	// Applies projective transform to a point
    Point3D project(Point3D point);

	// Normalizes a point to the viewing window
    Point2D normalize(Point3D point);

    //mode
    Mode m_mode;

    //buttons pressed and their positions
    bool m_buttonX, m_buttonZ, m_buttonY;

    //view port position
    int  m_ViewXpos, m_ViewYpos;

    //original coordinate position
    int  m_xPosition, m_yPosition;

    //temporary x position
    int  m_tempX;

    //bools to check if initilize or view is set or not
    bool m_init;
    bool m_view;

	// Viewport corners
	Point2D     m_viewport[4];

	// FOV value, default 30
    double fov;

	// Clipping planes
    double nearPlane;
    double farPlane;

	// Transformation matrices
	Matrix4x4   matrix_project;
	Matrix4x4   matrix_model;
	Matrix4x4   matrix_view;
	Matrix4x4   matrix_scale;

	// Stores the unit cube and the transformed unit cube
	Point3D     m_unitCube[8];
	Point3D     m_unitCubeTrans[8];

	// Stores gnomons
	Point3D     m_gnomon[4];
	Point3D     m_gnomonTrans[4];



};

#endif
