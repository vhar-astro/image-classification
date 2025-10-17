#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QString>
#include <QRect>
#include <QColor>

/**
 * @brief Represents a bounding box annotation for object detection
 * 
 * Stores the rectangle coordinates, label, and provides methods for
 * conversion between screen coordinates and normalized YOLO format.
 */
class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const QRect &rect, const QString &label, int classId = 0);
    
    // Getters
    QRect rect() const { return m_rect; }
    QString label() const { return m_label; }
    int classId() const { return m_classId; }
    QColor color() const { return m_color; }
    bool isSelected() const { return m_selected; }
    
    // Setters
    void setRect(const QRect &rect) { m_rect = rect; }
    void setLabel(const QString &label) { m_label = label; }
    void setClassId(int id) { m_classId = id; }
    void setColor(const QColor &color) { m_color = color; }
    void setSelected(bool selected) { m_selected = selected; }
    
    // Coordinate conversion methods
    // Convert from image coordinates to normalized YOLO format (0-1)
    void toYoloFormat(int imageWidth, int imageHeight, 
                      double &xCenter, double &yCenter, 
                      double &width, double &height) const;
    
    // Convert from normalized YOLO format to image coordinates
    static BoundingBox fromYoloFormat(double xCenter, double yCenter, 
                                      double width, double height,
                                      int imageWidth, int imageHeight,
                                      const QString &label, int classId);
    
    // Check if a point is inside the bounding box
    bool contains(const QPoint &point) const;
    
    // Check if a point is near a corner (for resizing)
    enum Corner { None, TopLeft, TopRight, BottomLeft, BottomRight };
    Corner nearCorner(const QPoint &point, int threshold = 10) const;
    
private:
    QRect m_rect;           // Bounding box rectangle in image coordinates
    QString m_label;        // Object label/class name
    int m_classId;          // Class ID for YOLO format
    QColor m_color;         // Color for visualization
    bool m_selected;        // Whether this bbox is currently selected
};

#endif // BOUNDINGBOX_H

