#include <QtGui/QApplication>
#include <iostream>
#include "include/mainwindow.h"
#include <stdio.h>
#include "include/bridgetabs.h"

int main(int argc, char *argv[], char** envp)
{

    QApplication a(argc, argv);
    //MainWindow w;
    BridgeTabs w;
    w.newConfig();
    w.show();
    
    return a.exec();
}
