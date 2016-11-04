/*
 * CPSC 453 - Introduction to Computer Graphics
 * Assignment 2
 *
 * Entry point for execution into the program
 */

#include "window.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    Window w;
    w.show();

    return a.exec();
}

