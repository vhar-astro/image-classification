#ifndef ANNOTATIONMANAGER_H
#define ANNOTATIONMANAGER_H

#include "BoundingBox.h"
#include <QString>
#include <QList>
#include <QMap>

/**
 * @brief Manages saving and loading of object detection annotations
 * 
 * Handles YOLO format annotations:
 * - One .txt file per image with bounding box annotations
 * - classes.txt file with label names
 * - Format: <class_id> <x_center> <y_center> <width> <height> (normalized 0-1)
 */
class AnnotationManager
{
public:
    AnnotationManager();
    
    // Set the output directory for annotations
    void setOutputDirectory(const QString &directory);
    QString outputDirectory() const { return m_outputDirectory; }
    
    // Label/class management
    void addLabel(const QString &label);
    void removeLabel(const QString &label);
    QStringList labels() const;
    int getClassId(const QString &label) const;
    QString getLabel(int classId) const;
    bool hasLabel(const QString &label) const;
    
    // Save/load classes.txt file
    bool saveClassesFile();
    bool loadClassesFile();
    
    // Save/load annotations for a specific image
    bool saveAnnotations(const QString &imagePath, 
                        const QList<BoundingBox> &boxes,
                        int imageWidth, int imageHeight);
    
    bool loadAnnotations(const QString &imagePath,
                        QList<BoundingBox> &boxes,
                        int imageWidth, int imageHeight);
    
    // Check if annotations exist for an image
    bool hasAnnotations(const QString &imagePath) const;
    
    // Get annotation file path for an image
    QString getAnnotationFilePath(const QString &imagePath) const;
    
    // Copy image to output directory
    bool copyImageToOutput(const QString &imagePath);
    
private:
    QString m_outputDirectory;
    QMap<QString, int> m_labelToId;  // Map label name to class ID
    QMap<int, QString> m_idToLabel;  // Map class ID to label name
    int m_nextClassId;
    
    // Helper methods
    QString getImageFileName(const QString &imagePath) const;
    QString getAnnotationFileName(const QString &imagePath) const;
};

#endif // ANNOTATIONMANAGER_H

