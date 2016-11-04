/*
 * CPSC 453 - Introduction to Computer Graphics
 * Assignment 1
 *
 * Window - Fundamental GUI for interacting within the scene
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QActionGroup>
#include <QTimer>
#include <renderer.h>
//#include <QtMultimedia\QMediaPlayer>

class Renderer;

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

    void increaseSpeed();   //increases the speed of the game
    void decreaseSpeed();   //decreases the speed of the game
    void pause();           //pauses the game
    void autoIncreaseBool();  //sets autoIncrease to true


protected:
    virtual void keyPressEvent(QKeyEvent *keyevent);    //key press event. See which button is pressed

private:
    // Main widget for drawing
    Renderer *renderer;

    // Menu items and actions
    QMenu * mFileMenu;
    QAction * mQuitAction;
    QAction * newGameAction;
    QAction * resetGameAction;
    
    //draw menu
    QMenu * drawMenu;
    QAction * wireFrameAction;
    QAction * faceAction;
    QAction * multicolorAction;
    
    //game menu
    QMenu * gameMenu;
    QAction * pauseAction;
    QAction * speedUpAction;
    QAction * speedDownAction;
    QAction * autoIncreaseAction;

    //Radio button group for drawing
    QActionGroup * drawActionGroup;
    //Radio button group for game options
    QActionGroup * gameActionGroup;

    

    // helper function for creating actions
    void createActions();
    //auto increase the speed
    void autoIncreaseSpeed();
    bool autoBool; //boolean value for setting autoIncreaseSpeed

};

#endif // WINDOW_H
