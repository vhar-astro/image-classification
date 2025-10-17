#include "AnnotationManager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

AnnotationManager::AnnotationManager()
    : m_outputDirectory("annotated_images"), m_nextClassId(0)
{
}

void AnnotationManager::setOutputDirectory(const QString &directory)
{
    m_outputDirectory = directory;
    
    // Create directory structure
    QDir dir;
    dir.mkpath(m_outputDirectory + "/images");
    dir.mkpath(m_outputDirectory + "/labels");
}

void AnnotationManager::addLabel(const QString &label)
{
    if (!m_labelToId.contains(label)) {
        m_labelToId[label] = m_nextClassId;
        m_idToLabel[m_nextClassId] = label;
        m_nextClassId++;
    }
}

void AnnotationManager::removeLabel(const QString &label)
{
    if (m_labelToId.contains(label)) {
        int classId = m_labelToId[label];
        m_labelToId.remove(label);
        m_idToLabel.remove(classId);
    }
}

QStringList AnnotationManager::labels() const
{
    return m_labelToId.keys();
}

int AnnotationManager::getClassId(const QString &label) const
{
    return m_labelToId.value(label, -1);
}

QString AnnotationManager::getLabel(int classId) const
{
    return m_idToLabel.value(classId, "");
}

bool AnnotationManager::hasLabel(const QString &label) const
{
    return m_labelToId.contains(label);
}

bool AnnotationManager::saveClassesFile()
{
    QString classesFilePath = m_outputDirectory + "/classes.txt";
    QFile file(classesFilePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open classes file for writing:" << classesFilePath;
        return false;
    }
    
    QTextStream out(&file);
    
    // Write labels in order of class ID
    for (int i = 0; i < m_nextClassId; ++i) {
        if (m_idToLabel.contains(i)) {
            out << m_idToLabel[i] << "\n";
        }
    }
    
    file.close();
    return true;
}

bool AnnotationManager::loadClassesFile()
{
    QString classesFilePath = m_outputDirectory + "/classes.txt";
    QFile file(classesFilePath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open classes file for reading:" << classesFilePath;
        return false;
    }
    
    // Clear existing labels
    m_labelToId.clear();
    m_idToLabel.clear();
    m_nextClassId = 0;
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString label = in.readLine().trimmed();
        if (!label.isEmpty()) {
            addLabel(label);
        }
    }
    
    file.close();
    return true;
}

bool AnnotationManager::saveAnnotations(const QString &imagePath,
                                        const QList<BoundingBox> &boxes,
                                        int imageWidth, int imageHeight)
{
    QString annotationPath = getAnnotationFilePath(imagePath);
    QFile file(annotationPath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open annotation file for writing:" << annotationPath;
        return false;
    }
    
    QTextStream out(&file);
    
    // Write each bounding box in YOLO format
    for (const BoundingBox &box : boxes) {
        double xCenter, yCenter, width, height;
        box.toYoloFormat(imageWidth, imageHeight, xCenter, yCenter, width, height);
        
        // Format: <class_id> <x_center> <y_center> <width> <height>
        out << box.classId() << " "
            << QString::number(xCenter, 'f', 6) << " "
            << QString::number(yCenter, 'f', 6) << " "
            << QString::number(width, 'f', 6) << " "
            << QString::number(height, 'f', 6) << "\n";
    }
    
    file.close();
    
    // Also save the classes file
    saveClassesFile();
    
    return true;
}

bool AnnotationManager::loadAnnotations(const QString &imagePath,
                                        QList<BoundingBox> &boxes,
                                        int imageWidth, int imageHeight)
{
    QString annotationPath = getAnnotationFilePath(imagePath);
    QFile file(annotationPath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // No annotations exist yet - this is not an error
        return true;
    }
    
    boxes.clear();
    QTextStream in(&file);
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() != 5) {
            qWarning() << "Invalid annotation line:" << line;
            continue;
        }
        
        bool ok;
        int classId = parts[0].toInt(&ok);
        if (!ok) continue;
        
        double xCenter = parts[1].toDouble(&ok);
        if (!ok) continue;
        
        double yCenter = parts[2].toDouble(&ok);
        if (!ok) continue;
        
        double width = parts[3].toDouble(&ok);
        if (!ok) continue;
        
        double height = parts[4].toDouble(&ok);
        if (!ok) continue;
        
        QString label = getLabel(classId);
        BoundingBox box = BoundingBox::fromYoloFormat(
            xCenter, yCenter, width, height,
            imageWidth, imageHeight, label, classId);
        
        boxes.append(box);
    }
    
    file.close();
    return true;
}

bool AnnotationManager::hasAnnotations(const QString &imagePath) const
{
    QString annotationPath = getAnnotationFilePath(imagePath);
    return QFile::exists(annotationPath);
}

QString AnnotationManager::getAnnotationFilePath(const QString &imagePath) const
{
    QString fileName = getAnnotationFileName(imagePath);
    return m_outputDirectory + "/labels/" + fileName;
}

bool AnnotationManager::copyImageToOutput(const QString &imagePath)
{
    QString fileName = getImageFileName(imagePath);
    QString destPath = m_outputDirectory + "/images/" + fileName;
    
    // Don't copy if already exists
    if (QFile::exists(destPath)) {
        return true;
    }
    
    return QFile::copy(imagePath, destPath);
}

QString AnnotationManager::getImageFileName(const QString &imagePath) const
{
    return QFileInfo(imagePath).fileName();
}

QString AnnotationManager::getAnnotationFileName(const QString &imagePath) const
{
    QFileInfo fileInfo(imagePath);
    return fileInfo.baseName() + ".txt";
}

