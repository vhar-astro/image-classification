#include "MainWindow.h"
#include "ObjectDetectionWindow.h"
#include "ModeSelectionDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application metadata
    QApplication::setApplicationName("ML Dataset Preparation Tool");
    QApplication::setApplicationVersion("2.0");
    QApplication::setOrganizationName("ML Tools");

    // Show mode selection dialog
    ModeSelectionDialog modeDialog;
    if (modeDialog.exec() == QDialog::Accepted) {
        if (modeDialog.selectedMode() == ModeSelectionDialog::ImageClassification) {
            // Launch Image Classification mode
            MainWindow *window = new MainWindow();
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        } else {
            // Launch Object Detection mode
            ObjectDetectionWindow *window = new ObjectDetectionWindow();
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        }

        return app.exec();
    }

    // User cancelled mode selection
    return 0;
}

