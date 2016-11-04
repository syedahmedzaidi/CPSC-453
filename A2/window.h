#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>

#include "renderer.h"

class Window : public QMainWindow
{
    // informs the qmake that a Qt moc_* file will need to be generated
    Q_OBJECT

public:
    // constructor
    Window(QWidget *parent = 0);

    // destructor
    ~Window();

public slots:
    void status_viewRotate();
    void status_viewTranslate();
    void status_viewPrespective();
    void status_modelRotate();
    void status_modelTranslate();
    void status_modelScale();
    void status_viewPort();

protected:

private:
	// The main OpenGL area
    Renderer* renderer;

	// Menu items and actions
	QMenu * mFileMenu;
	QAction * mQuitAction;
	QAction * mResetAction;
	
	
	QMenu * mModeMenu;

    //submenu
    QMenu * mViewMenu;
    QMenu * mModelMenu;
    QMenu * mViewPortMenu;

	QAction * viewRotate;
	QAction * viewTranslate;
	QAction * viewPrespective;

	QAction * mRotate;
	QAction * mTranslate;
	QAction * mScale;

	QAction * viewPort;

    QActionGroup * modeActionGroup;

    QLabel *statusLabel;

	
	
	//Label m_infobar;

	// helper function for creating actions
	void createActions();
    void createStatusBar();
};

#endif
