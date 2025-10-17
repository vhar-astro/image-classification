#include "ModeSelectionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>

ModeSelectionDialog::ModeSelectionDialog(QWidget *parent)
    : QDialog(parent), m_selectedMode(ImageClassification)
{
    setupUI();
    setWindowTitle("Select Mode");
    setModal(true);
    resize(500, 300);
}

ModeSelectionDialog::~ModeSelectionDialog()
{
}

void ModeSelectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Machine Learning Dataset Preparation Tool", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    mainLayout->addSpacing(20);
    
    // Instructions
    QLabel *instructionLabel = new QLabel("Please select the mode you want to use:", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);
    
    mainLayout->addSpacing(10);
    
    // Mode selection group
    QGroupBox *modeGroup = new QGroupBox("Mode Selection", this);
    QVBoxLayout *modeLayout = new QVBoxLayout(modeGroup);
    
    // Image Classification mode
    classificationRadio = new QRadioButton("Image Classification", this);
    classificationRadio->setChecked(true);
    QLabel *classificationDesc = new QLabel(
        "Assign a single category to each entire image.\n"
        "Use this for image classification datasets.", this);
    classificationDesc->setStyleSheet("QLabel { color: #666; margin-left: 25px; }");
    modeLayout->addWidget(classificationRadio);
    modeLayout->addWidget(classificationDesc);
    
    modeLayout->addSpacing(15);
    
    // Object Detection mode
    detectionRadio = new QRadioButton("Object Detection", this);
    QLabel *detectionDesc = new QLabel(
        "Draw bounding boxes around objects and label them.\n"
        "Use this for object detection datasets (YOLO format).", this);
    detectionDesc->setStyleSheet("QLabel { color: #666; margin-left: 25px; }");
    modeLayout->addWidget(detectionRadio);
    modeLayout->addWidget(detectionDesc);
    
    mainLayout->addWidget(modeGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    okButton->setMinimumWidth(100);
    
    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setMinimumWidth(100);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(okButton, &QPushButton::clicked, this, &ModeSelectionDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ModeSelectionDialog::onCancelClicked);
}

void ModeSelectionDialog::onOkClicked()
{
    if (classificationRadio->isChecked()) {
        m_selectedMode = ImageClassification;
    } else {
        m_selectedMode = ObjectDetection;
    }
    accept();
}

void ModeSelectionDialog::onCancelClicked()
{
    reject();
}

