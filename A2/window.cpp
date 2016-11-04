#include "window.h"

#include <QMenuBar>
#include <QApplication>

Window::Window(QWidget * parent) :
    QMainWindow(parent)
{
	setWindowTitle("CPSC453: Boxes vs the World!");

	// Create the main drawing object
	renderer = new Renderer();
	renderer->setMinimumSize(300, 300);
	setCentralWidget(renderer);

	// Create the actions to be used by the menus
	createActions();
    createStatusBar();

	// Create the menus
	// Setup the file menu
	mFileMenu = menuBar()->addMenu(tr("&File"));
	mFileMenu->addAction(mQuitAction);  // add quitting
    mFileMenu->addAction(mResetAction); // add resetting

    mModeMenu = menuBar()->addMenu(tr("&Mode"));
    mViewMenu = mModeMenu->addMenu(tr("&View Mode"));
    mViewMenu->addAction(viewRotate);
    mViewMenu->addAction(viewTranslate);
    mViewMenu->addAction(viewPrespective);

    mModelMenu = mModeMenu->addMenu(tr("&Model Mode"));
    mModelMenu->addAction(mRotate);
    mModelMenu->addAction(mTranslate);
    mModelMenu->addAction(mScale);

    mModeMenu->addAction(viewPort);

}

// destructor
Window::~Window()
{
    delete renderer;
}

// helper function for creating actions
void Window::createActions()
{
    modeActionGroup = new QActionGroup(this);

    // Quits the application
    mQuitAction = new QAction(tr("&Quit"), this);
    mQuitAction->setShortcut(QKeySequence(Qt::Key_Q));
    mQuitAction->setStatusTip(tr("Quits the application"));
    connect(mQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    mResetAction = new QAction(tr("&Reset"), this);
    mResetAction->setShortcut((QKeySequence(Qt::Key_A)));
    mResetAction->setStatusTip(tr("Resets to initial state"));
    connect(mResetAction, SIGNAL(triggered()), renderer, SLOT(reset()));

    viewRotate = new QAction(tr("&Rotate"), this);
    viewRotate->setShortcut((QKeySequence(Qt::Key_O)));
    viewRotate->setStatusTip(tr("Rotate according to eyepoint"));
    connect(viewRotate, SIGNAL(triggered()), this, SLOT(status_viewRotate()));

    viewTranslate = new QAction(tr("&Translate"), this);
    viewTranslate->setShortcut((QKeySequence(Qt::Key_N)));
    viewTranslate->setStatusTip(tr("Translate according to eyepoint"));
    connect(viewTranslate, SIGNAL(triggered()), this, SLOT(status_viewTranslate()));

    viewPrespective = new QAction(tr("&Prespective"), this);
    viewPrespective->setShortcut((QKeySequence(Qt::Key_P)));
    viewPrespective->setStatusTip(tr("Change prespective according to eyepoint"));
    connect(viewPrespective, SIGNAL(triggered()), this, SLOT(status_viewPrespective()));

    mRotate = new QAction(tr("&Rotate"), this);
    mRotate->setShortcut((QKeySequence(Qt::Key_R)));
    mRotate->setStatusTip(tr("Rotate according to the box"));
    connect(mRotate, SIGNAL(triggered()), this, SLOT(status_modelRotate()));

    mTranslate = new QAction(tr("&Translate"), this);
    mTranslate->setShortcut((QKeySequence(Qt::Key_T)));
    mTranslate->setStatusTip(tr("Translate according to box"));
    connect(mTranslate, SIGNAL(triggered()), this, SLOT(status_modelTranslate()));

    mScale = new QAction(tr("&Scale"), this);
    mScale->setShortcut((QKeySequence(Qt::Key_P)));
    mScale->setStatusTip(tr("Scale according to the box"));
    connect(mScale, SIGNAL(triggered()), this, SLOT(status_modelScale()));

    viewPort = new QAction(tr("&View Port"), this);
    viewPort->setShortcut((QKeySequence(Qt::Key_V)));
    viewPort->setStatusTip(tr("View Port Mode"));
    connect(viewPort, SIGNAL(triggered()), this, SLOT(status_viewPort()));


}
void Window::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void Window::status_viewRotate()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("View: Rotate"));
    renderer->set_viewRotate();
}

void Window::status_viewTranslate()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("View: Translate"));
    renderer->set_viewTranslate();

}

void Window::status_viewPrespective()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("View: Prespective"));
    renderer->set_viewPrespective();
}

void Window::status_modelRotate()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("Model: Rotate"));
    renderer->set_modelRotate();
}

void Window::status_modelTranslate()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("Model: Translate"));
    renderer->set_modelTranslate();
}

void Window::status_modelScale()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("Model: Scale"));
    renderer->set_modelScale();
}

void Window::status_viewPort()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(tr("Viewport"));
    renderer->set_viewPort();
}
