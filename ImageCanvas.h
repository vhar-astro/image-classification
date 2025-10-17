#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include "BoundingBox.h"
#include <QWidget>
#include <QPixmap>
#include <QList>
#include <QPoint>

/**
 * @brief Custom widget for displaying images and drawing bounding boxes
 * 
 * Handles mouse interactions for:
 * - Drawing new bounding boxes (click and drag)
 * - Selecting existing bounding boxes (click)
 * - Resizing bounding boxes (drag corners)
 * - Deleting bounding boxes (delete key)
 */
class ImageCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCanvas(QWidget *parent = nullptr);
    ~ImageCanvas();
    
    // Image management
    void setImage(const QPixmap &pixmap);
    QPixmap image() const { return m_originalPixmap; }
    void clearImage();
    
    // Bounding box management
    void addBoundingBox(const BoundingBox &box);
    void removeBoundingBox(int index);
    void clearBoundingBoxes();
    QList<BoundingBox> boundingBoxes() const { return m_boundingBoxes; }
    void setBoundingBoxes(const QList<BoundingBox> &boxes);
    
    // Selection
    int selectedBoxIndex() const { return m_selectedBoxIndex; }
    void setSelectedBoxIndex(int index);
    void clearSelection();
    
    // Get image dimensions (original, not scaled)
    int imageWidth() const { return m_originalPixmap.width(); }
    int imageHeight() const { return m_originalPixmap.height(); }

signals:
    void boundingBoxCreated(const QRect &rect);
    void boundingBoxSelected(int index);
    void boundingBoxModified(int index);
    void requestLabelForBox();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    // Drawing state
    enum DrawingState {
        Idle,
        Drawing,
        Resizing,
        Moving
    };
    
    DrawingState m_state;
    QPoint m_startPoint;
    QPoint m_currentPoint;
    QRect m_currentRect;
    
    // Image data
    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
    QRect m_imageRect;  // Rectangle where the scaled image is drawn
    double m_scale;     // Scale factor from original to displayed image
    
    // Bounding boxes
    QList<BoundingBox> m_boundingBoxes;
    int m_selectedBoxIndex;
    BoundingBox::Corner m_resizingCorner;
    
    // Helper methods
    void updateScaledImage();
    QPoint screenToImage(const QPoint &screenPoint) const;
    QPoint imageToScreen(const QPoint &imagePoint) const;
    QRect imageToScreen(const QRect &imageRect) const;
    QRect screenToImage(const QRect &screenRect) const;
    int findBoxAtPoint(const QPoint &point) const;
    void drawBoundingBoxes(QPainter &painter);
    QColor getColorForIndex(int index) const;
};

#endif // IMAGECANVAS_H

