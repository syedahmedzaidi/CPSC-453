#include "renderer.h"

#include <QTextStream>
#include "draw.h"
#include <QMatrix4x4>


using namespace std;

// constructor
Renderer::Renderer(QWidget *parent)
	: QOpenGLWidget(parent)
{
    m_init = true;
    reset();
}

// destructor
Renderer::~Renderer()
{
	// Nothing to do here right now.
}

void Renderer::invalidate()
{
	update();
}

void Renderer::set_mode( Mode mode )
{
	m_mode = mode;
    if( m_mode != mode )
    {
        m_mode = mode;
    }
    invalidate();
}

void Renderer::set_viewRotate()
{
    set_mode(VIEWROTATE);
}

void Renderer::set_viewTranslate()
{
    set_mode(VIEWTRANSLATE);
}

void Renderer::set_viewPrespective()
{
    set_mode(VIEWPERSPECTIVE);
}

void Renderer::set_modelRotate()
{
    set_mode(MODELROTATE);
}

void Renderer::set_modelTranslate()
{
    set_mode(MODELTRANSLATE);
}

void Renderer::set_modelScale()
{
    set_mode(MODELSCALE);
}

void Renderer::set_viewPort()
{
    set_mode(VIEWPORT);
}

void Renderer::set_perspective(double fov, double aspect,double near, double far)
{
    // Fill me in!
    Vector4D r1, r2, r3, r4;
    double   rfov, x1, x2;

	// Error check
	if( fov < 5   ) 
    {    
        fov = 5;
    }
	else 
        if ( fov > 160 ) 
        {    
            fov = 160;
        }
	// Calculate the fov in degrees related to the viewport
    rfov = 1 / tan( fov * PI / 360 );
    x1   = (far + near ) / (far - near);
    x2   = (-2 * far * near ) / (far - near);

	// Implement perspective matrix as described in course notes
	r1 = Vector4D( (rfov / aspect), 0, 0, 0);
	r2 = Vector4D( 0, rfov, 0, 0);
	r3 = Vector4D( 0, 0, x1, x2 );
	r4 = Vector4D( 0, 0, 1, 0);
    matrix_project = Matrix4x4(r1, r2, r3, r4);
}


// called once by Qt GUI system, to allow initialization for OpenGL requirements
void Renderer::initializeGL()
{
    // You might not have anything in here, might have viewport & matrix setup...

}


// called by the Qt GUI system, to allow OpenGL drawing commands
void Renderer::paintGL()
{
	// Here is where your drawing code should go.

	draw_init(width(), height());

    // Apply transformation to the model gnomon
    for( int i = 0; i < 4; i += 1 )
	{
		m_gnomonTrans[i] = matrix_view * matrix_model * m_gnomon[i];
	}

    // Draw the model gnomon
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[1] );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[2] );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[3] );

    // Draw the model gnomon
    set_colour( Colour(0.1, 1.0, 0.1) );
    modelGnomon();

    //apply transformation to model gnomon
    for (int i = 0; i < 4; i++) {
       m_gnomonTrans[i] = matrix_view * m_gnomon[i];
     }

    // Draw the world gnomon
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[1] );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[2] );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[3] );

    set_colour( Colour(0.1, 1.0, 0.1) );
    worldGnomon();


	// Draw the unit cube
	set_colour( Colour(0.1, 0.1, 0.1) );
    unitCube();


	// Initialize the viewport
    if (!m_view)
	{
        m_viewport[0] = ( Point2D(width() * 0.05, height() * 0.05) );
        m_viewport[1] = ( Point2D(width() * 0.95, height() * 0.05) );
        m_viewport[2] = ( Point2D(width() * 0.95, height() * 0.95) );
        m_viewport[3] = ( Point2D(width() * 0.05, height() * 0.95) );
        m_view = true;

	}
	// Draw the viewport
	set_colour( Colour(0.1, 0.1, 0.1) );
	draw_line( m_viewport[0], m_viewport[1] );
	draw_line( m_viewport[1], m_viewport[2] );
	draw_line( m_viewport[2], m_viewport[3] );
	draw_line( m_viewport[3], m_viewport[0] );

	// Finish drawing
	draw_complete();
	    
}

// called by the Qt GUI system, to allow OpenGL to respond to widget resizing
void Renderer::resizeGL(int width, int height)
{
    // width() and height() are better methods to use
    Q_UNUSED(width); Q_UNUSED(height);


    // You might not have anything in here, might have viewport setup...
}

// override mouse press event
void Renderer::mousePressEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";
    if ( m_mode == VIEWPORT )
    {
        m_ViewYpos = event->y();
        m_yPosition  = event->y();
    }

    /*switch(event->button())
	{
	case 1:
        m_buttonX = true;
		break;
	case 2:
        m_buttonZ = true;
		break;
    case 4:
        m_buttonY = true;
		break;
	default:
		break;
	}*/

    if(event->button() == 1)
    {
        m_buttonX = true;
    }

    if(event->button() == 2)
    {
        m_buttonZ = true;
    }

    if (event->button() == 4)
    {
        m_buttonY = true;
    }

	// Capture initial mouse position 
    m_ViewXpos = event->x();
    m_xPosition  = event->x();
    m_tempX = event->x();

	invalidate();
}


// override mouse release event
void Renderer::mouseReleaseEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " released.\n";

    if ( m_mode == VIEWPORT && m_buttonX )
	{
        m_xPosition = event->x();
        m_yPosition = event->y();

		// Error check
		if ( m_ViewXpos < 0 )
        {
            m_ViewXpos = 0;
        }
        if ( m_ViewXpos > width() )
        {
            m_ViewXpos = width();
        }
		if ( m_xPosition  < 0 )
        {
            m_xPosition  = 0;
        }
        if ( m_xPosition  > width() )
        {
            m_xPosition  = width();
		}
        if ( m_ViewYpos < 0 )
        { 
            m_ViewYpos = 0;
        }
        if ( m_ViewYpos > width() )
        { 
            m_ViewYpos = width();
        }
		if ( m_yPosition  < 0 ) 
        {    
            m_yPosition  = 0;
        }
        if ( m_yPosition  > width() ) 
        {
            m_yPosition  = width();
        }

		//set the points
        int x1, x2, y1, y2;
        x1 = min( m_ViewXpos, m_xPosition );
        x2 = max( m_ViewXpos, m_xPosition );
        y1 = min( m_ViewYpos, m_yPosition );
        y2 = max( m_ViewYpos, m_yPosition );

		// Update viewport
		m_viewport[0] = ( Point2D(x1, y1) );
		m_viewport[1] = ( Point2D(x2, y1) );
		m_viewport[2] = ( Point2D(x2, y2) );
		m_viewport[3] = ( Point2D(x1, y2) );

        invalidate();


    }

	// Which button(s) released?
    /* (event->button())
	{
	case 1:
        m_buttonX = false;
		break;
	case 2:
        m_buttonZ = false;
		break;
    case 4:
        m_buttonY = false;
		break;
	default:
		break;
	}*/

    if(event->button() == 1)
    {
        m_buttonX = false;
    }

    if(event->button() == 2)
    {
        m_buttonZ = false;
    }

    if (event->button() == 4)
    {
        m_buttonY = false;
    }

}


// override mouse move event
void Renderer::mouseMoveEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Motion at " << event->x() << ", " << event->y() << ".\n";

    if ( m_buttonX || m_buttonZ || m_buttonY )
	{
		m_tempX = m_xPosition;
        m_xPosition  = event->x();

		switch ( m_mode )
		{
		case VIEWROTATE:
            if ( m_buttonX ) //left  x
            {
                matrix_view = matrix_view * rotation( (m_tempX - m_xPosition) / 100.0, 'x' ).invert();
            }
            if ( m_buttonY ) //middle y
			{
                matrix_view= matrix_view * rotation( (m_tempX - m_xPosition) / 100.0, 'y' ).invert();
			}
            if ( m_buttonZ ) //right z
			{
                matrix_view = matrix_view * rotation( (m_tempX - m_xPosition) / 100.0, 'z' ).invert();
			}

			break;
		case VIEWTRANSLATE:
            if ( m_buttonX )
			{
                matrix_view = matrix_view * translation(Vector3D(( m_tempX - m_xPosition )/100.0, 0.0, 0.0) ).invert();
			}
            if ( m_buttonY )
			{
                matrix_view = matrix_view * translation( Vector3D(0.0, ( m_tempX - m_xPosition ) /100.0, 0.0) ).invert();
			}
            if ( m_buttonZ )
			{
                matrix_view = matrix_view * translation( Vector3D(0.0, 0.0 , ( m_tempX - m_xPosition ) / 100.0) ).invert();
			}
			break;
		case VIEWPERSPECTIVE:
            if ( m_buttonX )
			{
                fov= fov - (( m_tempX - m_xPosition ) / 10.0);
                set_perspective( fov, 1, nearPlane, farPlane );
			}
            if ( m_buttonY )
			{
                nearPlane = nearPlane - (( m_tempX - m_xPosition ) / 10.0);
			}
            if ( m_buttonZ )
			{
                farPlane = farPlane - (( m_tempX - m_xPosition ) / 10.0);
			}
			break;
		case MODELROTATE:
            if ( m_buttonX )
            {
                matrix_model = matrix_model * rotation( (m_tempX - m_xPosition) / 100.0, 'x' ).invert();
            }
            if ( m_buttonY )
			{
				matrix_model = matrix_model * rotation( (m_tempX - m_xPosition) / 100.0, 'y' ).invert();
			}
            if ( m_buttonZ )
			{
				matrix_model = matrix_model * rotation( (m_tempX - m_xPosition) / 100.0, 'z' ).invert();
			}
			break;
		case MODELTRANSLATE:
            if ( m_buttonX )
			{
                matrix_model = matrix_model * translation( Vector3D(( m_tempX - m_xPosition ) / -100.0, 0.0, 0.0) );
			}
            if ( m_buttonY )
			{
                matrix_model = matrix_model * translation( Vector3D(0.0, ( m_tempX - m_xPosition ) / -100.0, 0.0) );
			}
            if ( m_buttonZ )
			{
                matrix_model = matrix_model * translation( Vector3D(0.0, 0.0, ( m_tempX - m_xPosition ) / -100.0) );
			}
			break;
		case MODELSCALE:
            if ( m_buttonX )
			{
                matrix_scale = matrix_scale * scaling( Vector3D(1.0 +((m_tempX - m_xPosition)/100.0), 1.0, 1.0) ).invert();
			}
            if ( m_buttonY )
			{
                matrix_scale = matrix_scale * scaling( Vector3D(1.0, 1.0 +( m_tempX - m_xPosition ) / 100.0, 1.0) ).invert();
			}
            if ( m_buttonZ )
			{
                matrix_scale = matrix_scale * scaling( Vector3D(1.0, 1.0, 1.0 +( m_tempX - m_xPosition ) / 100.0) ).invert();
			}
			break;
		default:
			break;
		}

		invalidate();
	}

}

//initialize the cube
void Renderer::initializeCube()
{
        m_unitCube[0] = ( Point3D( 1.0, -1.0, -1.0) );
        m_unitCube[1] = ( Point3D(-1.0, -1.0, -1.0) );
        m_unitCube[2] = ( Point3D(-1.0,  1.0, -1.0) );
        m_unitCube[3] = ( Point3D( 1.0,  1.0, -1.0) );
        m_unitCube[4] = ( Point3D(-1.0, -1.0,  1.0) );
        m_unitCube[5] = ( Point3D( 1.0, -1.0,  1.0) );
        m_unitCube[6] = ( Point3D( 1.0,  1.0,  1.0) );
        m_unitCube[7] = ( Point3D(-1.0,  1.0,  1.0) );

        //populates for transformation of cubes
        for ( int i = 0; i < 8; i += 1 )
        {
            m_unitCubeTrans[i] = ( Point3D() );
        }
}

void Renderer::unitCube()
{
    // Apply transformations to unit cube
    for( int i = 0; i < 8; i += 1 )
    {
        m_unitCubeTrans[i] = matrix_view   *
                             matrix_model *
                             matrix_scale   *
                             m_unitCube[i];
    }


    set_colour( Colour(0.1, 0.1, 0.1) );
    draw_line2D( m_unitCubeTrans[0], m_unitCubeTrans[1] );
    draw_line2D( m_unitCubeTrans[0], m_unitCubeTrans[3] );
    set_colour( Colour(0.1, 0.1, 0.1) );
    draw_line2D( m_unitCubeTrans[0], m_unitCubeTrans[5] );
    set_colour( Colour(0.1, 0.1, 0.1) );
    draw_line2D( m_unitCubeTrans[1], m_unitCubeTrans[2] );
    set_colour( Colour(0.1, 0.1, 0.1) );
    draw_line2D( m_unitCubeTrans[1], m_unitCubeTrans[4] );
    set_colour( Colour(0.1, 0.1, 0.1) );
    draw_line2D( m_unitCubeTrans[2], m_unitCubeTrans[3] );
    set_colour( Colour(0.1, 0.1, 0.1) );
    draw_line2D( m_unitCubeTrans[2], m_unitCubeTrans[7] );
    draw_line2D( m_unitCubeTrans[3], m_unitCubeTrans[6] );
    draw_line2D( m_unitCubeTrans[4], m_unitCubeTrans[5] );
    draw_line2D( m_unitCubeTrans[4], m_unitCubeTrans[7] );
    draw_line2D( m_unitCubeTrans[5], m_unitCubeTrans[6] );
    draw_line2D( m_unitCubeTrans[6], m_unitCubeTrans[7] );
    set_colour( Colour(0.1, 0.1, 0.1) );
}
//initialize the gnomon
void Renderer::initializeGnomon()
{
    m_gnomon[0] = ( Point3D(0.0, 0.0, 0.0) );
    m_gnomon[1] = ( Point3D(0.5, 0.0, 0.0) );
    m_gnomon[2] = ( Point3D(0.0, 0.5, 0.0) );
    m_gnomon[3] = ( Point3D(0.0, 0.0, 0.5) );
    for ( int i = 0; i < 4; i += 1 )
    {
        m_gnomonTrans[i] = ( Point3D() );
    }
}

void Renderer::modelGnomon()
{
    // Apply transformation to the modelling gnomon
    for( int i = 0; i < 4; i += 1 )
    {
        m_gnomonTrans[i] = matrix_view * matrix_model * m_gnomon[i];
    }

    // Draw the modelling gnomon
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[1] );
    set_colour( Colour(1.0, 0.1, 0.1) );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[2] );
    set_colour( Colour(0.1, 0.1, 1.0) );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[3] );
}

void Renderer::worldGnomon()
{
    //draw the world Gnomon
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[1] );
    set_colour( Colour(1.0, 0.1, 0.1) );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[2] );
    set_colour( Colour(0.1, 0.1, 1.0) );
    draw_line2D( m_gnomonTrans[0], m_gnomonTrans[3] );
}

void Renderer::draw_line2D ( Point3D left, Point3D right )
{
    //Bool value to check if we should draw this line
    // false if it is outside of clipping area
    bool draw = true;

    //clip to near plane first
    double clip_nearLeft = ( left  - Point3D(0.0, 0.0, nearPlane) ).dot(Vector3D(0.0, 0.0, 1.0));
    double clip_nearRight = ( right - Point3D(0.0, 0.0, nearPlane) ).dot(Vector3D(0.0, 0.0, 1.0) );
    if ( clip_nearLeft < 0.0 && clip_nearRight < 0.0 )
    {
        draw = false;
    }
    else
    {
        if ( clip_nearLeft < 0.0 || clip_nearRight < 0.0 )
        {
            double t = clip_nearLeft / ( clip_nearLeft - clip_nearRight );
            if ( clip_nearLeft < 0.0 )
            {
                left = left + t * ( right - left );
            }
            else
            {
                right = left + t * ( right - left );
            }
        }
    }

    // Clip to the far plane
    if ( draw )
    {
        double clip_farLeft = ( left  - Point3D(0.0, 0.0, farPlane) ).dot(Vector3D(0.0, 0.0, -1.0));
        double clip_farRight = ( right - Point3D(0.0, 0.0, farPlane) ).dot(Vector3D(0.0, 0.0, -1.0));
        if ( clip_farLeft < 0.0 && clip_farRight < 0.0 )
        {
            draw = false;
        }
        else
        {
            if ( clip_farLeft < 0.0 || clip_farRight < 0.0 )
            {
                double t = clip_farLeft / ( clip_farLeft - clip_farRight );
                if ( clip_farLeft < 0.0 )
                {
                    // A + t(B-A)
                    left = left + t *(right - left);
                }
                else
                {
                    right = left + t * ( right - left );
                }
            }
        }
    }

    // Now transform and clip to the viewport
    if( draw )
    {
        // project then normalize each of the points
        Point2D nleft  = normalize( project(left)  );
        Point2D nright = normalize( project(right) );

        // clip to the viewing cube (the viewport)
        for ( int i = 0; i < 4; i++ )
        {
            Point2D normal;
            double clip_viewLeft, clip_viewRight;
            switch( i )
            {
            case 0:
                clip_viewLeft = nleft[1]  - m_viewport[0][1];
                clip_viewRight = nright[1] - m_viewport[0][1];
                break;
            case 1:
                clip_viewLeft = -1.0 * ( nleft[0]  - m_viewport[1][0] );
                clip_viewRight = -1.0 * ( nright[0] - m_viewport[1][0] );
                break;
            case 2:
                clip_viewLeft = -1.0 * ( nleft[1]  - m_viewport[2][1] );
                clip_viewRight = -1.0 * ( nright[1] - m_viewport[2][1] );
                break;
            case 3:
                clip_viewLeft = nleft[0]  - m_viewport[3][0];
                clip_viewRight = nright[0] - m_viewport[3][0];
                break;
            default:
                clip_viewLeft = -1.0;
                clip_viewRight = -1.0;
                break;
            }

            if ( clip_viewLeft < 0.0 && clip_viewRight < 0.0 )
            {
                draw = false;
            }
            else
            {
                if ( clip_viewLeft < 0.0 || clip_viewRight < 0.0 )
                {
                    double t = clip_viewLeft / ( clip_viewLeft - clip_viewRight );
                    if ( clip_viewLeft < 0.0 )
                    {
                        nleft[0]  = nleft[0] + t * ( nright[0] - nleft[0] );
                        nleft[1]  = nleft[1] + t * ( nright[1] - nleft[1] );
                    }
                    else
                    {
                        nright[0] = nleft[0] + t * ( nright[0] - nleft[0] );
                        nright[1] = nleft[1] + t * ( nright[1] - nleft[1] );
                    }
                }
            }
        }

        //draw the line
        if ( draw )
        {
            draw_line( nleft, nright );
        }
    }
}

Point3D Renderer::project( Point3D point )
{
    // Project the point into the viewing plane
    Point3D point_p = matrix_project * point;

    return Point3D( (point_p[0] / point[2]), (point_p[1] / point[2]), (point_p[2] / point[2]) );
}

Point2D Renderer::normalize( Point3D point )
{
    // Normalize the point to the coordinates used in the viewing window 
    double x, y, z;
    x = ( point[0] + 1.0 ) / 2;
    y = ( point[1] + 1.0 ) / 2;
    z = point[2];

    double x1,y2;
    x1 = (point[0] / z + 1.5) * (m_viewport[2][0] - m_viewport[0][0]) /3 + m_viewport[0][0];
    y2 = (point[1] / z + 1.5) * (m_viewport[2][1] - m_viewport[0][1]) / 3 + m_viewport[0][1];

    return Point2D(x1, y2);
}

void Renderer::reset()
{
    if (!m_init)
    {
        set_mode( NOMODE );
    }

    // Default mouse information
    m_buttonX = false;
    m_buttonZ = false;
    m_buttonY = false;
    m_ViewXpos   = 0.0;
    m_xPosition    = 0.0;
    m_ViewYpos   = 0.0;
    m_yPosition    = 0.0;
    m_tempX   = 0.0;

    // Initialize viewport
    for ( int i = 0; i < 4; i += 1 )
    {
        m_viewport[i] = (Point2D());
    }

    // Default FOV of 30
    fov  = 30.0;

    // Set the default far and near plane values
    nearPlane = 2.0;
    farPlane  = 20.0;

    // Initialize all the transformation matrices
    matrix_project = Matrix4x4();
    matrix_model  = Matrix4x4();
    matrix_view    = Matrix4x4();
    matrix_scale    = Matrix4x4();

    // Start off by pushing the cube back into the screen
    Vector4D row1, row2, row3, row4;
    row1      = Vector4D( 1,   0,  0, 0 );
    row2      = Vector4D( 0,   1,  0, 0 );
    row3      = Vector4D( 0,   0,  1, 8 );
    row4      = Vector4D( 0,   0,  0, 1 );
    matrix_view = Matrix4x4( row1, row2, row3, row4 );

    initializeCube();
    initializeGnomon();

    m_view = false;
    m_init = false;

    // Initialize the perspective
    set_perspective( fov, 1, nearPlane, farPlane );
    invalidate();
}






