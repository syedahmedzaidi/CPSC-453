#include "window.h"
#include "renderer.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("CPSC453: Tetromino Apocalypse");

    // Create the main drawing object
    renderer = new Renderer();

    // Create the actions to be used by the menus
    createActions();

    // Create the menus
    // Setup the file menu
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mQuitAction);  // add quitting
    mFileMenu->addAction(newGameAction); //add new game
    mFileMenu->addAction(resetGameAction);
    
    //setup draw menu
    drawMenu = menuBar()-> addMenu(tr("&Draw"));
    drawMenu->addAction(wireFrameAction);  // add wireframe mode
    drawMenu->addAction(faceAction); //add face mode
    drawMenu->addAction(multicolorAction); //add multicolor mode
    
    //setup game menu
    gameMenu = menuBar()-> addMenu(tr("&Game"));
    gameMenu->addAction(pauseAction);  // add pause game
    gameMenu->addAction(speedUpAction); //add speed up
    gameMenu->addAction(speedDownAction); //add speed down
    gameMenu->addAction(autoIncreaseAction); //add auto increase

    // Setup the application's widget collection
    QVBoxLayout * layout = new QVBoxLayout();

    // Add renderer
    layout->addWidget(renderer);
    renderer->setMinimumSize(300, 600);

    QWidget * mainWidget = new QWidget();
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

}

// helper function for creating actions
void Window::createActions()
{
    drawActionGroup = new QActionGroup(this);
    gameActionGroup = new QActionGroup(this);

    // Quits the application
    mQuitAction = new QAction(tr("&Quit"), this);
    mQuitAction->setShortcut(QKeySequence(Qt::Key_Q));
    mQuitAction->setStatusTip(tr("Quits the application"));
    connect(mQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    // Creates a new game
    newGameAction = new QAction(tr("&New Game"), this);
    newGameAction->setShortcut(QKeySequence(Qt::Key_N));
    newGameAction->setStatusTip(tr("Start a new game"));
    connect(newGameAction, SIGNAL(triggered()), renderer, SLOT(newGame()));
    
    // Resets the game
    resetGameAction = new QAction(tr("&Reset"), this);
    resetGameAction->setShortcut(QKeySequence(Qt::Key_R));
	resetGameAction->setStatusTip(tr("reset the game game"));
    connect(resetGameAction, SIGNAL(triggered()), renderer, SLOT(resetView()));
    
    // Draws the game in wireframe mode
    wireFrameAction = new QAction(tr("&Wireframe Mode"), this);
    wireFrameAction->setShortcut(QKeySequence(Qt::Key_W));
	wireFrameAction->setStatusTip(tr("Draw game in wireframe mode"));
    wireFrameAction->setCheckable(true);
    connect(wireFrameAction,SIGNAL(triggered()), renderer, SLOT(setWireMode()));
    
    // Draws the game in face mode
    faceAction = new QAction(tr("&Face Mode"), this);
    faceAction->setShortcut(QKeySequence(Qt::Key_F));
	faceAction->setStatusTip(tr("Draw game in face mode"));
    faceAction->setCheckable(true);
    faceAction->setChecked(true);
    connect(faceAction,SIGNAL(triggered()), renderer, SLOT(setFaceMode()));

    
    //draws the game in Multicolor
    multicolorAction = new QAction(tr("&Multicolor Mode"), this);
    multicolorAction->setShortcut(QKeySequence(Qt::Key_M));
	multicolorAction->setStatusTip(tr("Draw game in multicolor"));
    multicolorAction->setCheckable(true);
    connect(multicolorAction,SIGNAL(triggered()), renderer, SLOT(setMultiMode()));

    //drawAction group radio button
    drawActionGroup->addAction(wireFrameAction);
    drawActionGroup->addAction(faceAction);
    drawActionGroup->addAction(multicolorAction);

    // Pauses the game
    pauseAction = new QAction(tr("&Pause"), this);
    pauseAction->setShortcut(QKeySequence(Qt::Key_P));
	pauseAction->setStatusTip(tr("Pause the game"));
    pauseAction->setCheckable(true);
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pause()));
    
    // Increase the speed of the game
    speedUpAction = new QAction(tr("&Speed Up"), this);
    speedUpAction->setShortcut(QKeySequence(Qt::Key_PageUp));
	speedUpAction->setStatusTip(tr("Increase the speed of gameplay"));
    speedUpAction->setCheckable(true);
    connect(speedUpAction, SIGNAL(triggered()), this, SLOT(increaseSpeed()));
    
    // Decreases the speed of the game
    speedDownAction = new QAction(tr("&Speed Down"), this);
    speedDownAction->setShortcut(QKeySequence(Qt::Key_PageDown));
	speedDownAction->setStatusTip(tr("Decrease the speed of gameplay"));
    speedDownAction->setCheckable(true);
    connect(speedDownAction, SIGNAL(triggered()), this, SLOT(decreaseSpeed()));
    
    //auto increase the speed of the game
    autoIncreaseAction = new QAction(tr("&Auto Increase"), this);
    autoIncreaseAction->setShortcut(QKeySequence(Qt::Key_A));
	autoIncreaseAction->setStatusTip(tr("Auto Increase the speed"));
    autoIncreaseAction->setCheckable(true);
    connect(autoIncreaseAction, SIGNAL(triggered()), this, SLOT(autoIncreaseBool()));

    //game action radio group
    gameActionGroup->addAction(pauseAction);
    gameActionGroup->addAction(speedUpAction);
    gameActionGroup->addAction(speedDownAction);
    gameActionGroup->addAction(autoIncreaseAction);

}

//key press events for determining which key is pressed
void Window::keyPressEvent(QKeyEvent *keyevent)
{

    switch(keyevent->key())
    {

        case Qt::Key_Up:
            renderer->rotateCCW();
            break;
        case Qt::Key_Down:
            renderer->rotateCW();
            break;
        case Qt::Key_Left:

            renderer->moveLeft();

            break;
        case Qt::Key_Right:
            renderer->moveRight();
            break;
        case Qt::Key_Space:
            renderer->drop();
            break;

    }
}

//increases speed
void Window::increaseSpeed()
{
    renderer->setSpeedInterval(150);

}

//decreases speed
void Window::decreaseSpeed()
{
    renderer->setSpeedInterval(650);

}
//auto increases speed
void Window::autoIncreaseBool()
{


        autoBool = true;
        autoIncreaseSpeed();


}

//auto increases speed helper function
void Window::autoIncreaseSpeed()
{
    QTextStream cout(stdout);
    cout << "Speed increased "<< renderer->speedOfPiece << ".\n";

    while(autoBool == true)   //is autoBool true
    {
        if(renderer->speedOfPiece > 0)     //is speedOfPiece is greater than 0.
        {
            int x = renderer->speedOfPiece;
            x -= 5;
            renderer->setSpeedInterval(x);
            autoIncreaseSpeed();            //calls the function again
        }
        else
        {
            autoBool = false;
            QTextStream cout(stdout);
            cout << "Speed increased too much!! "<< renderer->speedOfPiece << ".\n";
        }
    }

}

//pauses the game
void Window::pause()
{
    renderer->pauseGame();      //callls pauseGame function from renderer class
}

// destructor
Window::~Window()
{
    delete renderer;
}
