#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("Image Classification Tool");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("ML Tools");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

