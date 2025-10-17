#include "ObjectDetectionWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QCloseEvent>
#include <QSplitter>
#include <QScrollArea>

const QStringList ObjectDetectionWindow::IMAGE_EXTENSIONS = {"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.JPG", "*.JPEG", "*.PNG", "*.BMP"};

ObjectDetectionWindow::ObjectDetectionWindow(QWidget *parent)
    : QMainWindow(parent),
      currentImageIndex(-1)
{
    setupUI();
    setWindowTitle("Object Detection Annotation Tool");
    resize(1400, 900);
    
    // Initialize annotation manager
    annotationManager.setOutputDirectory("annotated_images");
}

ObjectDetectionWindow::~ObjectDetectionWindow()
{
}

void ObjectDetectionWindow::setupUI()
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    QHBoxLayout *mainHLayout = new QHBoxLayout(centralWidget);
    
    // Left panel - Image canvas and controls
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    // Title label
    QLabel *titleLabel = new QLabel("Object Detection Dataset Preparation Tool", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(titleLabel);
    
    // File operation buttons
    QHBoxLayout *fileButtonsLayout = new QHBoxLayout();
    openImageButton = new QPushButton("Open Single Image", this);
    openFolderButton = new QPushButton("Open Folder", this);
    fileButtonsLayout->addWidget(openImageButton);
    fileButtonsLayout->addWidget(openFolderButton);
    leftLayout->addLayout(fileButtonsLayout);
    
    // Progress information
    progressLabel = new QLabel("No images loaded", this);
    progressLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(progressLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    leftLayout->addWidget(progressBar);
    
    // Image canvas
    QGroupBox *imageGroup = new QGroupBox("Image Canvas - Draw bounding boxes by clicking and dragging", this);
    QVBoxLayout *imageLayout = new QVBoxLayout(imageGroup);
    
    imageCanvas = new ImageCanvas(this);
    imageLayout->addWidget(imageCanvas);
    
    imageInfoLabel = new QLabel("", this);
    imageInfoLabel->setAlignment(Qt::AlignCenter);
    imageLayout->addWidget(imageInfoLabel);
    
    leftLayout->addWidget(imageGroup, 1);
    
    // Save and navigation buttons
    QHBoxLayout *saveButtonsLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save Annotations", this);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton:enabled { background-color: #2196F3; color: white; font-weight: bold; }");
    
    saveAndNextButton = new QPushButton("Save && Next", this);
    saveAndNextButton->setEnabled(false);
    saveAndNextButton->setStyleSheet("QPushButton:enabled { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    saveButtonsLayout->addWidget(saveButton);
    saveButtonsLayout->addWidget(saveAndNextButton);
    leftLayout->addLayout(saveButtonsLayout);
    
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
    leftLayout->addLayout(navigationLayout);
    
    // Right panel - Labels and bounding boxes
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightPanel->setMaximumWidth(350);
    
    // Label management
    labelGroup = new QGroupBox("Object Labels", this);
    QVBoxLayout *labelLayout = new QVBoxLayout(labelGroup);
    
    QLabel *labelInstructionLabel = new QLabel("Create labels for objects you want to detect:", this);
    labelInstructionLabel->setWordWrap(true);
    labelLayout->addWidget(labelInstructionLabel);
    
    QHBoxLayout *newLabelLayout = new QHBoxLayout();
    newLabelInput = new QLineEdit(this);
    newLabelInput->setPlaceholderText("Enter label name (e.g., 'person', 'car')");
    addLabelButton = new QPushButton("Add Label", this);
    newLabelLayout->addWidget(newLabelInput);
    newLabelLayout->addWidget(addLabelButton);
    labelLayout->addLayout(newLabelLayout);
    
    QLabel *availableLabelsLabel = new QLabel("Available Labels:", this);
    labelLayout->addWidget(availableLabelsLabel);
    
    labelsListWidget = new QListWidget(this);
    labelsListWidget->setMaximumHeight(150);
    labelLayout->addWidget(labelsListWidget);
    
    rightLayout->addWidget(labelGroup);
    
    // Bounding boxes list
    boxListGroup = new QGroupBox("Bounding Boxes", this);
    QVBoxLayout *boxListLayout = new QVBoxLayout(boxListGroup);
    
    QLabel *boxListLabel = new QLabel("Boxes in current image:", this);
    boxListLayout->addWidget(boxListLabel);
    
    boxListWidget = new QListWidget(this);
    boxListLayout->addWidget(boxListWidget);
    
    deleteBoxButton = new QPushButton("Delete Selected Box", this);
    deleteBoxButton->setEnabled(false);
    boxListLayout->addWidget(deleteBoxButton);
    
    QLabel *instructionLabel = new QLabel(
        "<b>Instructions:</b><br>"
        "• Click and drag to draw bounding box<br>"
        "• Click box to select it<br>"
        "• Press Delete to remove selected box<br>"
        "• Drag corners to resize box", this);
    instructionLabel->setWordWrap(true);
    instructionLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 10px; border-radius: 5px; }");
    boxListLayout->addWidget(instructionLabel);
    
    rightLayout->addWidget(boxListGroup);
    rightLayout->addStretch();
    
    // Add panels to main layout
    mainHLayout->addWidget(leftPanel, 3);
    mainHLayout->addWidget(rightPanel, 1);
    
    // Set central widget
    setCentralWidget(centralWidget);
    
    // Connect signals and slots
    connect(openImageButton, &QPushButton::clicked, this, &ObjectDetectionWindow::openImage);
    connect(openFolderButton, &QPushButton::clicked, this, &ObjectDetectionWindow::openFolder);
    connect(addLabelButton, &QPushButton::clicked, this, &ObjectDetectionWindow::addNewLabel);
    connect(newLabelInput, &QLineEdit::returnPressed, this, &ObjectDetectionWindow::addNewLabel);
    connect(labelsListWidget, &QListWidget::itemClicked, this, &ObjectDetectionWindow::onLabelListItemClicked);
    
    connect(imageCanvas, &ImageCanvas::boundingBoxCreated, this, &ObjectDetectionWindow::onBoundingBoxCreated);
    connect(imageCanvas, &ImageCanvas::boundingBoxSelected, this, &ObjectDetectionWindow::onBoundingBoxSelected);
    connect(imageCanvas, &ImageCanvas::requestLabelForBox, this, &ObjectDetectionWindow::onRequestLabelForBox);
    
    connect(boxListWidget, &QListWidget::currentRowChanged, this, &ObjectDetectionWindow::onBoundingBoxSelected);
    connect(deleteBoxButton, &QPushButton::clicked, this, &ObjectDetectionWindow::deleteSelectedBox);
    
    connect(saveButton, &QPushButton::clicked, this, &ObjectDetectionWindow::saveCurrentAnnotations);
    connect(saveAndNextButton, &QPushButton::clicked, this, &ObjectDetectionWindow::saveAndNext);
    connect(nextButton, &QPushButton::clicked, this, &ObjectDetectionWindow::nextImage);
    connect(previousButton, &QPushButton::clicked, this, &ObjectDetectionWindow::previousImage);
    connect(skipButton, &QPushButton::clicked, this, &ObjectDetectionWindow::skipImage);
}

void ObjectDetectionWindow::openImage()
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

void ObjectDetectionWindow::openFolder()
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

void ObjectDetectionWindow::loadImagesFromFolder(const QString &folderPath)
{
    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(IMAGE_EXTENSIONS, QDir::Files | QDir::NoDotAndDotDot);
    
    for (const QFileInfo &fileInfo : fileList) {
        if (isImageFile(fileInfo.absoluteFilePath())) {
            imageFiles.append(fileInfo.absoluteFilePath());
        }
    }
}

bool ObjectDetectionWindow::isImageFile(const QString &filePath)
{
    QString extension = QFileInfo(filePath).suffix().toLower();
    return (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "bmp");
}

void ObjectDetectionWindow::addNewLabel()
{
    QString newLabel = newLabelInput->text().trimmed();

    if (newLabel.isEmpty()) {
        QMessageBox::warning(this, "Invalid Label", "Please enter a label name.");
        return;
    }

    // Check if label already exists
    if (annotationManager.hasLabel(newLabel)) {
        QMessageBox::information(this, "Label Exists", "This label already exists.");
        newLabelInput->clear();
        return;
    }

    // Add new label
    annotationManager.addLabel(newLabel);
    labelsListWidget->addItem(newLabel);
    newLabelInput->clear();

    QMessageBox::information(this, "Label Added",
        QString("Label '%1' has been added successfully.").arg(newLabel));
}

void ObjectDetectionWindow::onLabelSelected(int index)
{
    Q_UNUSED(index);
}

void ObjectDetectionWindow::onLabelListItemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
}

void ObjectDetectionWindow::onBoundingBoxCreated(const QRect &rect)
{
    pendingBoundingBox = rect;
}

void ObjectDetectionWindow::onRequestLabelForBox()
{
    if (pendingBoundingBox.isNull()) {
        return;
    }

    QString label;
    if (promptForLabel(label)) {
        // Create bounding box with label
        int classId = annotationManager.getClassId(label);
        BoundingBox box(pendingBoundingBox, label, classId);
        imageCanvas->addBoundingBox(box);
        updateBoxList();

        // Enable save buttons
        saveButton->setEnabled(true);
        saveAndNextButton->setEnabled(true);
    }

    pendingBoundingBox = QRect();
}

void ObjectDetectionWindow::onBoundingBoxSelected(int index)
{
    imageCanvas->setSelectedBoxIndex(index);
    boxListWidget->setCurrentRow(index);
    deleteBoxButton->setEnabled(index >= 0);
}

void ObjectDetectionWindow::deleteSelectedBox()
{
    int index = imageCanvas->selectedBoxIndex();
    if (index >= 0) {
        imageCanvas->removeBoundingBox(index);
        updateBoxList();
        deleteBoxButton->setEnabled(false);
    }
}

bool ObjectDetectionWindow::promptForLabel(QString &label)
{
    QStringList labels = annotationManager.labels();

    if (labels.isEmpty()) {
        QMessageBox::warning(this, "No Labels",
            "Please create at least one label before drawing bounding boxes.");
        return false;
    }

    bool ok;
    label = QInputDialog::getItem(this, "Select Label",
        "Choose a label for this bounding box:", labels, 0, false, &ok);

    return ok && !label.isEmpty();
}

void ObjectDetectionWindow::saveCurrentAnnotations()
{
    if (currentImagePath.isEmpty()) {
        return;
    }

    QList<BoundingBox> boxes = imageCanvas->boundingBoxes();

    if (boxes.isEmpty()) {
        QMessageBox::information(this, "No Annotations",
            "There are no bounding boxes to save for this image.");
        return;
    }

    // Save annotations
    bool success = annotationManager.saveAnnotations(
        currentImagePath, boxes,
        imageCanvas->imageWidth(), imageCanvas->imageHeight());

    if (success) {
        // Copy image to output directory
        annotationManager.copyImageToOutput(currentImagePath);

        // Mark as processed
        if (!processedImages.contains(currentImagePath)) {
            processedImages.append(currentImagePath);
        }

        updateProgress();
        QMessageBox::information(this, "Success", "Annotations saved successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to save annotations.");
    }
}

void ObjectDetectionWindow::saveAndNext()
{
    saveCurrentAnnotations();
    nextImage();
}

void ObjectDetectionWindow::nextImage()
{
    if (currentImageIndex < imageFiles.size() - 1) {
        currentImageIndex++;
        updateImageDisplay();
        updateProgress();
        updateNavigationButtons();
    }
}

void ObjectDetectionWindow::previousImage()
{
    if (currentImageIndex > 0) {
        currentImageIndex--;
        updateImageDisplay();
        updateProgress();
        updateNavigationButtons();
    }
}

void ObjectDetectionWindow::skipImage()
{
    nextImage();
}

void ObjectDetectionWindow::updateImageDisplay()
{
    if (currentImageIndex < 0 || currentImageIndex >= imageFiles.size()) {
        imageCanvas->clearImage();
        imageInfoLabel->setText("");
        return;
    }

    currentImagePath = imageFiles[currentImageIndex];
    QPixmap pixmap(currentImagePath);

    if (pixmap.isNull()) {
        imageCanvas->clearImage();
        imageInfoLabel->setText("");
        QMessageBox::warning(this, "Error", "Failed to load image: " + currentImagePath);
        return;
    }

    imageCanvas->setImage(pixmap);

    // Load existing annotations if any
    loadAnnotationsForCurrentImage();

    // Update image info
    QFileInfo fileInfo(currentImagePath);
    QString info = QString("File: %1 | Size: %2x%3 | %4 KB")
        .arg(fileInfo.fileName())
        .arg(pixmap.width())
        .arg(pixmap.height())
        .arg(fileInfo.size() / 1024);
    imageInfoLabel->setText(info);

    skipButton->setEnabled(true);
    saveButton->setEnabled(true);
    saveAndNextButton->setEnabled(true);

    updateBoxList();
}

void ObjectDetectionWindow::loadAnnotationsForCurrentImage()
{
    QList<BoundingBox> boxes;
    annotationManager.loadAnnotations(currentImagePath, boxes,
        imageCanvas->imageWidth(), imageCanvas->imageHeight());

    imageCanvas->setBoundingBoxes(boxes);
}

void ObjectDetectionWindow::updateProgress()
{
    if (imageFiles.isEmpty()) {
        progressLabel->setText("No images loaded");
        progressBar->setValue(0);
        return;
    }

    int total = imageFiles.size();
    int processed = processedImages.size();
    int current = currentImageIndex + 1;

    progressLabel->setText(QString("Image %1 of %2 | Annotated: %3")
        .arg(current)
        .arg(total)
        .arg(processed));

    int progressPercent = (processed * 100) / total;
    progressBar->setValue(progressPercent);
}

void ObjectDetectionWindow::updateNavigationButtons()
{
    previousButton->setEnabled(currentImageIndex > 0);
    nextButton->setEnabled(currentImageIndex < imageFiles.size() - 1);
}

void ObjectDetectionWindow::updateBoxList()
{
    boxListWidget->clear();

    QList<BoundingBox> boxes = imageCanvas->boundingBoxes();
    for (int i = 0; i < boxes.size(); ++i) {
        const BoundingBox &box = boxes[i];
        QString itemText = QString("%1. %2 [%3, %4, %5x%6]")
            .arg(i + 1)
            .arg(box.label())
            .arg(box.rect().x())
            .arg(box.rect().y())
            .arg(box.rect().width())
            .arg(box.rect().height());
        boxListWidget->addItem(itemText);
    }
}

void ObjectDetectionWindow::clearCurrentSession()
{
    imageFiles.clear();
    processedImages.clear();
    currentImageIndex = -1;
    currentImagePath.clear();
    imageCanvas->clearImage();
    imageInfoLabel->setText("");
    progressLabel->setText("No images loaded");
    progressBar->setValue(0);
    updateNavigationButtons();
    boxListWidget->clear();
}

void ObjectDetectionWindow::closeEvent(QCloseEvent *event)
{
    // Check if there are unsaved changes
    if (!imageCanvas->boundingBoxes().isEmpty() && !currentImagePath.isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
            "Unsaved Changes",
            "Do you want to save the current annotations before closing?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            saveCurrentAnnotations();
            event->accept();
        } else if (reply == QMessageBox::No) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

