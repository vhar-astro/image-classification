#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QProgressBar>
#include <QStringList>
#include <QFileInfoList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QPixmap>
#include <QResizeEvent>
#include <QSet>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // File operations
    void openImage();
    void openFolder();
    
    // Classification operations
    void classifyImage();
    void addNewCategory();
    void onCategorySelected(int index);
    
    // Navigation
    void nextImage();
    void previousImage();
    void skipImage();
    
    // UI updates
    void updateImageDisplay();
    void updateProgress();
    void updateNavigationButtons();

private:
    // Helper methods
    void setupUI();
    void loadImagesFromFolder(const QString &folderPath);
    bool moveImageToCategory(const QString &imagePath, const QString &category);
    void createCategoryFolder(const QString &category);
    bool isImageFile(const QString &filePath);
    void clearCurrentSession();
    void scaleImageToFit();
    
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    // Image display area
    QScrollArea *scrollArea;
    QLabel *imageLabel;
    QLabel *imageInfoLabel;
    QLabel *progressLabel;
    QProgressBar *progressBar;
    
    // Classification controls
    QGroupBox *classificationGroup;
    QComboBox *categoryComboBox;
    QLineEdit *newCategoryInput;
    QPushButton *addCategoryButton;
    QListWidget *categoriesListWidget;
    
    // Action buttons
    QPushButton *openImageButton;
    QPushButton *openFolderButton;
    QPushButton *classifyButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *skipButton;
    
    // Data members
    QStringList imageFiles;           // List of all image file paths
    QStringList processedImages;      // List of processed image paths
    QSet<QString> categories;         // Set of available categories
    int currentImageIndex;            // Current image index
    QString currentImagePath;         // Current image file path
    QString sourceFolder;             // Source folder for batch processing
    QString outputFolder;             // Output folder for classified images
    QPixmap currentPixmap;            // Current image pixmap
    
    // Constants
    static const QStringList IMAGE_EXTENSIONS;
};

#endif // MAINWINDOW_H

