#include "MainWindow.h"
#include <QFileInfo>
#include <QDateTime>
#include <QStandardPaths>

const QStringList MainWindow::IMAGE_EXTENSIONS = {"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.JPG", "*.JPEG", "*.PNG", "*.BMP"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      currentImageIndex(-1),
      outputFolder("classified_images")
{
    setupUI();
    setWindowTitle("Image Classification Tool");
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Title label
    QLabel *titleLabel = new QLabel("Machine Learning Image Classification Tool", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // File operation buttons
    QHBoxLayout *fileButtonsLayout = new QHBoxLayout();
    openImageButton = new QPushButton("Open Single Image", this);
    openFolderButton = new QPushButton("Open Folder", this);
    fileButtonsLayout->addWidget(openImageButton);
    fileButtonsLayout->addWidget(openFolderButton);
    mainLayout->addLayout(fileButtonsLayout);
    
    // Progress information
    progressLabel = new QLabel("No images loaded", this);
    progressLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(progressLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    mainLayout->addWidget(progressBar);
    
    // Image display area
    QGroupBox *imageGroup = new QGroupBox("Current Image", this);
    QVBoxLayout *imageLayout = new QVBoxLayout(imageGroup);
    
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(400);
    
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(false);
    imageLabel->setText("No image loaded");
    imageLabel->setStyleSheet("QLabel { background-color: #f0f0f0; border: 2px dashed #999; }");
    
    scrollArea->setWidget(imageLabel);
    imageLayout->addWidget(scrollArea);
    
    imageInfoLabel = new QLabel("", this);
    imageInfoLabel->setAlignment(Qt::AlignCenter);
    imageLayout->addWidget(imageInfoLabel);
    
    mainLayout->addWidget(imageGroup);
    
    // Classification controls
    classificationGroup = new QGroupBox("Classification", this);
    QVBoxLayout *classificationLayout = new QVBoxLayout(classificationGroup);
    
    QLabel *categoryLabel = new QLabel("Select or create a category:", this);
    classificationLayout->addWidget(categoryLabel);
    
    categoryComboBox = new QComboBox(this);
    categoryComboBox->setEditable(false);
    categoryComboBox->addItem("-- Select Category --");
    classificationLayout->addWidget(categoryComboBox);
    
    QHBoxLayout *newCategoryLayout = new QHBoxLayout();
    QLabel *newCatLabel = new QLabel("New Category:", this);
    newCategoryInput = new QLineEdit(this);
    newCategoryInput->setPlaceholderText("Enter new category name");
    addCategoryButton = new QPushButton("Add Category", this);
    newCategoryLayout->addWidget(newCatLabel);
    newCategoryLayout->addWidget(newCategoryInput);
    newCategoryLayout->addWidget(addCategoryButton);
    classificationLayout->addLayout(newCategoryLayout);
    
    classifyButton = new QPushButton("Classify and Move Image", this);
    classifyButton->setEnabled(false);
    classifyButton->setStyleSheet("QPushButton:enabled { background-color: #4CAF50; color: white; font-weight: bold; padding: 10px; }");
    classificationLayout->addWidget(classifyButton);
    
    mainLayout->addWidget(classificationGroup);
    
    // Navigation buttons
    QHBoxLayout *navigationLayout = new QHBoxLayout();
    previousButton = new QPushButton("← Previous", this);
    skipButton = new QPushButton("Skip", this);
    nextButton = new QPushButton("Next →", this);
    
    previousButton->setEnabled(false);
    skipButton->setEnabled(false);
    nextButton->setEnabled(false);
    
    navigationLayout->addWidget(previousButton);
    navigationLayout->addWidget(skipButton);
    navigationLayout->addWidget(nextButton);
    mainLayout->addLayout(navigationLayout);
    
    // Set central widget
    setCentralWidget(centralWidget);
    
    // Connect signals and slots
    connect(openImageButton, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(openFolderButton, &QPushButton::clicked, this, &MainWindow::openFolder);
    connect(classifyButton, &QPushButton::clicked, this, &MainWindow::classifyImage);
    connect(addCategoryButton, &QPushButton::clicked, this, &MainWindow::addNewCategory);
    connect(categoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onCategorySelected);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextImage);
    connect(previousButton, &QPushButton::clicked, this, &MainWindow::previousImage);
    connect(skipButton, &QPushButton::clicked, this, &MainWindow::skipImage);
    connect(newCategoryInput, &QLineEdit::returnPressed, this, &MainWindow::addNewCategory);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Image File",
        QDir::homePath(),
        "Image Files (*.jpg *.jpeg *.png *.bmp);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        clearCurrentSession();
        imageFiles.append(fileName);
        currentImageIndex = 0;
        currentImagePath = fileName;
        updateImageDisplay();
        updateProgress();
        updateNavigationButtons();
    }
}

void MainWindow::openFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
        "Open Folder Containing Images",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    if (!folderPath.isEmpty()) {
        clearCurrentSession();
        sourceFolder = folderPath;
        loadImagesFromFolder(folderPath);
        
        if (imageFiles.isEmpty()) {
            QMessageBox::warning(this, "No Images Found",
                "No supported image files (JPG, PNG, BMP) were found in the selected folder.");
            return;
        }
        
        currentImageIndex = 0;
        currentImagePath = imageFiles[0];
        updateImageDisplay();
        updateProgress();
        updateNavigationButtons();
    }
}

void MainWindow::loadImagesFromFolder(const QString &folderPath)
{
    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(IMAGE_EXTENSIONS, QDir::Files | QDir::NoDotAndDotDot);
    
    for (const QFileInfo &fileInfo : fileList) {
        if (isImageFile(fileInfo.absoluteFilePath())) {
            imageFiles.append(fileInfo.absoluteFilePath());
        }
    }
}

bool MainWindow::isImageFile(const QString &filePath)
{
    QString extension = QFileInfo(filePath).suffix().toLower();
    return (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "bmp");
}

void MainWindow::updateImageDisplay()
{
    if (currentImageIndex < 0 || currentImageIndex >= imageFiles.size()) {
        imageLabel->setText("No image loaded");
        imageInfoLabel->setText("");
        return;
    }
    
    currentImagePath = imageFiles[currentImageIndex];
    currentPixmap = QPixmap(currentImagePath);
    
    if (currentPixmap.isNull()) {
        imageLabel->setText("Failed to load image");
        imageInfoLabel->setText("");
        QMessageBox::warning(this, "Error", "Failed to load image: " + currentImagePath);
        return;
    }
    
    scaleImageToFit();
    
    // Update image info
    QFileInfo fileInfo(currentImagePath);
    QString info = QString("File: %1 | Size: %2x%3 | %4 KB")
        .arg(fileInfo.fileName())
        .arg(currentPixmap.width())
        .arg(currentPixmap.height())
        .arg(fileInfo.size() / 1024);
    imageInfoLabel->setText(info);
    
    skipButton->setEnabled(true);
}

void MainWindow::scaleImageToFit()
{
    if (currentPixmap.isNull()) {
        return;
    }
    
    // Scale image to fit the scroll area while maintaining aspect ratio
    int maxWidth = scrollArea->viewport()->width() - 20;
    int maxHeight = scrollArea->viewport()->height() - 20;
    
    QPixmap scaledPixmap = currentPixmap.scaled(maxWidth, maxHeight, 
        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->adjustSize();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    scaleImageToFit();
}

void MainWindow::updateProgress()
{
    if (imageFiles.isEmpty()) {
        progressLabel->setText("No images loaded");
        progressBar->setValue(0);
        return;
    }
    
    int total = imageFiles.size();
    int processed = processedImages.size();
    int current = currentImageIndex + 1;
    
    progressLabel->setText(QString("Image %1 of %2 | Processed: %3")
        .arg(current)
        .arg(total)
        .arg(processed));
    
    int progressPercent = (processed * 100) / total;
    progressBar->setValue(progressPercent);
}

void MainWindow::updateNavigationButtons()
{
    previousButton->setEnabled(currentImageIndex > 0);
    nextButton->setEnabled(currentImageIndex < imageFiles.size() - 1);
}

void MainWindow::addNewCategory()
{
    QString newCategory = newCategoryInput->text().trimmed();
    
    if (newCategory.isEmpty()) {
        QMessageBox::warning(this, "Invalid Category", "Please enter a category name.");
        return;
    }
    
    // Check if category already exists
    if (categories.contains(newCategory)) {
        QMessageBox::information(this, "Category Exists", "This category already exists.");
        categoryComboBox->setCurrentText(newCategory);
        newCategoryInput->clear();
        return;
    }
    
    // Add new category
    categories.insert(newCategory);
    categoryComboBox->addItem(newCategory);
    categoryComboBox->setCurrentText(newCategory);
    newCategoryInput->clear();
    
    // Create the category folder
    createCategoryFolder(newCategory);
}

void MainWindow::onCategorySelected(int index)
{
    // Enable classify button only if a valid category is selected and an image is loaded
    bool validSelection = (index > 0) && (currentImageIndex >= 0);
    classifyButton->setEnabled(validSelection);
}

void MainWindow::classifyImage()
{
    if (currentImageIndex < 0 || currentImageIndex >= imageFiles.size()) {
        return;
    }

    QString selectedCategory = categoryComboBox->currentText();
    if (selectedCategory == "-- Select Category --" || selectedCategory.isEmpty()) {
        QMessageBox::warning(this, "No Category Selected", "Please select a category first.");
        return;
    }

    // Move the image to the category folder
    if (moveImageToCategory(currentImagePath, selectedCategory)) {
        processedImages.append(currentImagePath);

        QMessageBox::information(this, "Success",
            QString("Image classified as '%1' and moved successfully!").arg(selectedCategory));

        // Move to next image
        if (currentImageIndex < imageFiles.size() - 1) {
            nextImage();
        } else {
            // All images processed
            QMessageBox::information(this, "Complete",
                "All images have been processed!");
            clearCurrentSession();
        }
    }
}

bool MainWindow::moveImageToCategory(const QString &imagePath, const QString &category)
{
    QFileInfo fileInfo(imagePath);
    QString categoryPath = outputFolder + "/" + category;

    // Create category folder if it doesn't exist
    createCategoryFolder(category);

    // Destination path
    QString destPath = categoryPath + "/" + fileInfo.fileName();

    // Check if file already exists at destination
    if (QFile::exists(destPath)) {
        // Add timestamp to make filename unique
        QString baseName = fileInfo.baseName();
        QString extension = fileInfo.suffix();
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        destPath = categoryPath + "/" + baseName + "_" + timestamp + "." + extension;
    }

    // Copy the file (safer than move in case of errors)
    if (QFile::copy(imagePath, destPath)) {
        // Optionally delete the original file
        // QFile::remove(imagePath);
        return true;
    } else {
        QMessageBox::critical(this, "Error",
            QString("Failed to move image to category folder.\nSource: %1\nDestination: %2")
            .arg(imagePath).arg(destPath));
        return false;
    }
}

void MainWindow::createCategoryFolder(const QString &category)
{
    QString categoryPath = outputFolder + "/" + category;
    QDir dir;

    if (!dir.exists(categoryPath)) {
        if (!dir.mkpath(categoryPath)) {
            QMessageBox::warning(this, "Error",
                QString("Failed to create category folder: %1").arg(categoryPath));
        }
    }
}

void MainWindow::nextImage()
{
    if (currentImageIndex < imageFiles.size() - 1) {
        currentImageIndex++;
        updateImageDisplay();
        updateProgress();
        updateNavigationButtons();
    }
}

void MainWindow::previousImage()
{
    if (currentImageIndex > 0) {
        currentImageIndex--;
        updateImageDisplay();
        updateProgress();
        updateNavigationButtons();
    }
}

void MainWindow::skipImage()
{
    // Just move to next image without classifying
    nextImage();
}

void MainWindow::clearCurrentSession()
{
    imageFiles.clear();
    processedImages.clear();
    currentImageIndex = -1;
    currentImagePath.clear();
    imageLabel->clear();
    imageLabel->setText("No image loaded");
    imageInfoLabel->setText("");
    progressLabel->setText("No images loaded");
    progressBar->setValue(0);
    updateNavigationButtons();
}

