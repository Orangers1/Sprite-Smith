/**
 * @file main.cpp
 * @brief Main function to set up and run application
 * @date 2025-03-31
 * Reviewed by: Grant Beck
 */

#include <QApplication>
#include "mainwindow.h"
#include "model.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    MainWindow w(nullptr, &m);
    w.show();
    return a.exec();
}
