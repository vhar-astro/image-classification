#include "BoundingBox.h"
#include <QPoint>
#include <cmath>

BoundingBox::BoundingBox()
    : m_classId(0), m_color(Qt::green), m_selected(false)
{
}

BoundingBox::BoundingBox(const QRect &rect, const QString &label, int classId)
    : m_rect(rect), m_label(label), m_classId(classId), 
      m_color(Qt::green), m_selected(false)
{
}

void BoundingBox::toYoloFormat(int imageWidth, int imageHeight,
                                double &xCenter, double &yCenter,
                                double &width, double &height) const
{
    // Convert from top-left corner + width/height to center + normalized dimensions
    // YOLO format: <class_id> <x_center> <y_center> <width> <height>
    // All values are normalized to [0, 1] relative to image dimensions
    
    double rectCenterX = m_rect.x() + m_rect.width() / 2.0;
    double rectCenterY = m_rect.y() + m_rect.height() / 2.0;
    
    // Normalize to [0, 1]
    xCenter = rectCenterX / imageWidth;
    yCenter = rectCenterY / imageHeight;
    width = static_cast<double>(m_rect.width()) / imageWidth;
    height = static_cast<double>(m_rect.height()) / imageHeight;
}

BoundingBox BoundingBox::fromYoloFormat(double xCenter, double yCenter,
                                        double width, double height,
                                        int imageWidth, int imageHeight,
                                        const QString &label, int classId)
{
    // Convert from normalized YOLO format to image coordinates
    double rectWidth = width * imageWidth;
    double rectHeight = height * imageHeight;
    double rectX = (xCenter * imageWidth) - (rectWidth / 2.0);
    double rectY = (yCenter * imageHeight) - (rectHeight / 2.0);
    
    QRect rect(static_cast<int>(rectX), static_cast<int>(rectY),
               static_cast<int>(rectWidth), static_cast<int>(rectHeight));
    
    return BoundingBox(rect, label, classId);
}

bool BoundingBox::contains(const QPoint &point) const
{
    return m_rect.contains(point);
}

BoundingBox::Corner BoundingBox::nearCorner(const QPoint &point, int threshold) const
{
    // Check if point is near any corner of the bounding box
    QPoint topLeft = m_rect.topLeft();
    QPoint topRight = m_rect.topRight();
    QPoint bottomLeft = m_rect.bottomLeft();
    QPoint bottomRight = m_rect.bottomRight();
    
    auto distance = [](const QPoint &p1, const QPoint &p2) -> double {
        int dx = p1.x() - p2.x();
        int dy = p1.y() - p2.y();
        return std::sqrt(dx * dx + dy * dy);
    };
    
    if (distance(point, topLeft) <= threshold)
        return TopLeft;
    if (distance(point, topRight) <= threshold)
        return TopRight;
    if (distance(point, bottomLeft) <= threshold)
        return BottomLeft;
    if (distance(point, bottomRight) <= threshold)
        return BottomRight;
    
    return None;
}

