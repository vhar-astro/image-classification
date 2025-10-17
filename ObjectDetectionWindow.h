#ifndef OBJECTDETECTIONWINDOW_H
#define OBJECTDETECTIONWINDOW_H

#include "ImageCanvas.h"
#include "AnnotationManager.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QProgressBar>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

/**
 * @brief Main window for object detection annotation mode
 * 
 * Provides UI for:
 * - Loading images
 * - Drawing bounding boxes
 * - Labeling objects
 * - Navigating through images
 * - Saving annotations in YOLO format
 */
class ObjectDetectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ObjectDetectionWindow(QWidget *parent = nullptr);
    ~ObjectDetectionWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File operations
    void openImage();
    void openFolder();
    
    // Label management
    void addNewLabel();
    void onLabelSelected(int index);
    void onLabelListItemClicked(QListWidgetItem *item);
    
    // Bounding box operations
    void onBoundingBoxCreated(const QRect &rect);
    void onBoundingBoxSelected(int index);
    void onRequestLabelForBox();
    void deleteSelectedBox();
    
    // Navigation
    void nextImage();
    void previousImage();
    void skipImage();
    
    // Save operations
    void saveCurrentAnnotations();
    void saveAndNext();
    
    // UI updates
    void updateImageDisplay();
    void updateProgress();
    void updateNavigationButtons();
    void updateBoxList();

private:
    // Helper methods
    void setupUI();
    void loadImagesFromFolder(const QString &folderPath);
    bool isImageFile(const QString &filePath);
    void clearCurrentSession();
    void loadAnnotationsForCurrentImage();
    bool promptForLabel(QString &label);
    
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    // Image display area
    ImageCanvas *imageCanvas;
    QLabel *imageInfoLabel;
    QLabel *progressLabel;
    QProgressBar *progressBar;
    
    // Label management controls
    QGroupBox *labelGroup;
    QComboBox *labelComboBox;
    QLineEdit *newLabelInput;
    QPushButton *addLabelButton;
    QListWidget *labelsListWidget;
    
    // Bounding box list
    QGroupBox *boxListGroup;
    QListWidget *boxListWidget;
    QPushButton *deleteBoxButton;
    
    // Action buttons
    QPushButton *openImageButton;
    QPushButton *openFolderButton;
    QPushButton *saveButton;
    QPushButton *saveAndNextButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *skipButton;
    
    // Data members
    QStringList imageFiles;           // List of all image file paths
    QStringList processedImages;      // List of processed image paths
    int currentImageIndex;            // Current image index
    QString currentImagePath;         // Current image file path
    QString sourceFolder;             // Source folder for batch processing
    
    // Annotation management
    AnnotationManager annotationManager;
    QRect pendingBoundingBox;         // Temporary storage for box awaiting label
    
    // Constants
    static const QStringList IMAGE_EXTENSIONS;
};

#endif // OBJECTDETECTIONWINDOW_H

