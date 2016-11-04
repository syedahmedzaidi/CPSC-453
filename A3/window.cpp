#include "window.h"
#include "renderer.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("CPSC453: Creature Loader");

    // Create the main drawing object
    renderer = new Renderer();

    // Create the actions to be used by the menus
    createActions();

    // Create the menus
    // Setup the file menu
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mQuitAction);  // add quitting
    mFileMenu->addAction(mLoadModel);
    mFileMenu->addAction(mLoadSubModel);
    mFileMenu->addAction(mLoadTexture);

    mDrawMenu = menuBar()->addMenu(tr("&Draw"));
    mDrawMenu->addAction(mWireFrame);
    mDrawMenu->addAction(mFaceMode);
    mDrawMenu->addAction(mSmoothMode);

    // Setup the application's widget collection
    QVBoxLayout * layout = new QVBoxLayout();

    // Add renderer
    layout->addWidget(renderer);
    renderer->setMinimumSize(800, 800);

    QWidget * mainWidget = new QWidget();
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

}

// helper function for creating actions
void Window::createActions()
{
    // Quits the application
    mQuitAction = new QAction(tr("&Quit"), this);
    mQuitAction->setShortcut(QKeySequence(Qt::Key_Q));
    mQuitAction->setStatusTip(tr("Quits the application"));
    connect(mQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    mLoadModel = new QAction(tr("&Load a Model"), this);
    mLoadModel->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    mLoadModel->setStatusTip(tr("Loads a model"));
    connect(mLoadModel, SIGNAL(triggered()), renderer, SLOT(openModel()));

    mLoadSubModel = new QAction(tr("&Load a Sub Model"), this);
    mLoadSubModel->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    mLoadSubModel->setStatusTip(tr("Loads a Sub model"));
    connect(mLoadSubModel, SIGNAL(triggered()), renderer, SLOT(openSubModel()));


    mWireFrame = new QAction(tr("&Wire Frame Mode"), this);
    mWireFrame->setShortcut(QKeySequence(Qt::Key_W));
    mWireFrame->setStatusTip(tr("Draws in Wire Frame Mode"));
    connect(mWireFrame, SIGNAL(triggered()), renderer, SLOT(setWireType()));

    mFaceMode = new QAction(tr("&Face Normal Mode"), this);
    mFaceMode->setShortcut(QKeySequence(Qt::Key_F));
    mFaceMode->setStatusTip(tr("Draws in Face Normal Mode"));
    connect(mFaceMode, SIGNAL(triggered()), renderer, SLOT(setFaceType()));

    mSmoothMode = new QAction(tr("&Vertex Normal Mode"), this);
    mSmoothMode->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    mSmoothMode->setStatusTip(tr("Draws in Vertex Normal Mode"));
    connect(mSmoothMode, SIGNAL(triggered()), renderer, SLOT(setSmoothType()));

    mLoadTexture = new QAction(tr("&Load a Texture"), this);
    mLoadTexture->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    mLoadTexture->setStatusTip(tr("Loads a Texture"));
    connect(mLoadTexture, SIGNAL(triggered()), renderer, SLOT(openTextureModel()));


}

// destructor
Window::~Window()
{
    delete renderer;
}
