#include "ImageCanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPen>
#include <QFont>

ImageCanvas::ImageCanvas(QWidget *parent)
    : QWidget(parent),
      m_state(Idle),
      m_scale(1.0),
      m_selectedBoxIndex(-1),
      m_resizingCorner(BoundingBox::None)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setMinimumSize(400, 300);
}

ImageCanvas::~ImageCanvas()
{
}

void ImageCanvas::setImage(const QPixmap &pixmap)
{
    m_originalPixmap = pixmap;
    updateScaledImage();
    clearBoundingBoxes();
    update();
}

void ImageCanvas::clearImage()
{
    m_originalPixmap = QPixmap();
    m_scaledPixmap = QPixmap();
    clearBoundingBoxes();
    update();
}

void ImageCanvas::addBoundingBox(const BoundingBox &box)
{
    m_boundingBoxes.append(box);
    update();
}

void ImageCanvas::removeBoundingBox(int index)
{
    if (index >= 0 && index < m_boundingBoxes.size()) {
        m_boundingBoxes.removeAt(index);
        if (m_selectedBoxIndex == index) {
            m_selectedBoxIndex = -1;
        } else if (m_selectedBoxIndex > index) {
            m_selectedBoxIndex--;
        }
        update();
    }
}

void ImageCanvas::clearBoundingBoxes()
{
    m_boundingBoxes.clear();
    m_selectedBoxIndex = -1;
    update();
}

void ImageCanvas::setBoundingBoxes(const QList<BoundingBox> &boxes)
{
    m_boundingBoxes = boxes;
    m_selectedBoxIndex = -1;
    update();
}

void ImageCanvas::setSelectedBoxIndex(int index)
{
    m_selectedBoxIndex = index;
    
    // Update selection state in bounding boxes
    for (int i = 0; i < m_boundingBoxes.size(); ++i) {
        m_boundingBoxes[i].setSelected(i == index);
    }
    
    update();
}

void ImageCanvas::clearSelection()
{
    setSelectedBoxIndex(-1);
}

void ImageCanvas::updateScaledImage()
{
    if (m_originalPixmap.isNull()) {
        return;
    }
    
    // Calculate scale to fit image in widget while maintaining aspect ratio
    int availableWidth = width() - 20;
    int availableHeight = height() - 20;
    
    double scaleX = static_cast<double>(availableWidth) / m_originalPixmap.width();
    double scaleY = static_cast<double>(availableHeight) / m_originalPixmap.height();
    m_scale = qMin(scaleX, scaleY);
    
    int scaledWidth = static_cast<int>(m_originalPixmap.width() * m_scale);
    int scaledHeight = static_cast<int>(m_originalPixmap.height() * m_scale);
    
    m_scaledPixmap = m_originalPixmap.scaled(scaledWidth, scaledHeight,
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation);
    
    // Center the image
    int x = (width() - scaledWidth) / 2;
    int y = (height() - scaledHeight) / 2;
    m_imageRect = QRect(x, y, scaledWidth, scaledHeight);
}

QPoint ImageCanvas::screenToImage(const QPoint &screenPoint) const
{
    if (m_scale == 0) return QPoint();
    
    int x = static_cast<int>((screenPoint.x() - m_imageRect.x()) / m_scale);
    int y = static_cast<int>((screenPoint.y() - m_imageRect.y()) / m_scale);
    
    return QPoint(x, y);
}

QPoint ImageCanvas::imageToScreen(const QPoint &imagePoint) const
{
    int x = static_cast<int>(imagePoint.x() * m_scale) + m_imageRect.x();
    int y = static_cast<int>(imagePoint.y() * m_scale) + m_imageRect.y();
    
    return QPoint(x, y);
}

QRect ImageCanvas::imageToScreen(const QRect &imageRect) const
{
    QPoint topLeft = imageToScreen(imageRect.topLeft());
    QPoint bottomRight = imageToScreen(imageRect.bottomRight());
    
    return QRect(topLeft, bottomRight);
}

QRect ImageCanvas::screenToImage(const QRect &screenRect) const
{
    QPoint topLeft = screenToImage(screenRect.topLeft());
    QPoint bottomRight = screenToImage(screenRect.bottomRight());
    
    return QRect(topLeft, bottomRight);
}

int ImageCanvas::findBoxAtPoint(const QPoint &point) const
{
    QPoint imagePoint = screenToImage(point);
    
    // Search in reverse order to prioritize top-most boxes
    for (int i = m_boundingBoxes.size() - 1; i >= 0; --i) {
        if (m_boundingBoxes[i].contains(imagePoint)) {
            return i;
        }
    }
    
    return -1;
}

QColor ImageCanvas::getColorForIndex(int index) const
{
    // Cycle through a set of distinct colors
    static const QColor colors[] = {
        QColor(0, 255, 0),      // Green
        QColor(255, 0, 0),      // Red
        QColor(0, 0, 255),      // Blue
        QColor(255, 255, 0),    // Yellow
        QColor(255, 0, 255),    // Magenta
        QColor(0, 255, 255),    // Cyan
        QColor(255, 128, 0),    // Orange
        QColor(128, 0, 255),    // Purple
    };
    
    return colors[index % 8];
}

void ImageCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    if (m_scaledPixmap.isNull()) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "No image loaded");
        return;
    }
    
    // Draw the image
    painter.drawPixmap(m_imageRect, m_scaledPixmap);
    
    // Draw existing bounding boxes
    drawBoundingBoxes(painter);
    
    // Draw current drawing rectangle
    if (m_state == Drawing && !m_currentRect.isNull()) {
        QRect screenRect = imageToScreen(m_currentRect);
        painter.setPen(QPen(Qt::green, 2, Qt::DashLine));
        painter.drawRect(screenRect);
    }
}

void ImageCanvas::drawBoundingBoxes(QPainter &painter)
{
    for (int i = 0; i < m_boundingBoxes.size(); ++i) {
        const BoundingBox &box = m_boundingBoxes[i];
        QRect screenRect = imageToScreen(box.rect());
        
        // Determine color and pen style
        QColor color = getColorForIndex(i);
        int penWidth = box.isSelected() ? 3 : 2;
        Qt::PenStyle penStyle = box.isSelected() ? Qt::SolidLine : Qt::SolidLine;
        
        // Draw rectangle
        painter.setPen(QPen(color, penWidth, penStyle));
        painter.drawRect(screenRect);
        
        // Draw label background
        QString labelText = box.label();
        if (!labelText.isEmpty()) {
            QFont font = painter.font();
            font.setPointSize(10);
            font.setBold(true);
            painter.setFont(font);
            
            QFontMetrics fm(font);
            QRect textRect = fm.boundingRect(labelText);
            textRect.adjust(-4, -2, 4, 2);
            textRect.moveTo(screenRect.left(), screenRect.top() - textRect.height());
            
            // Draw label background
            painter.fillRect(textRect, color);
            
            // Draw label text
            painter.setPen(Qt::black);
            painter.drawText(textRect, Qt::AlignCenter, labelText);
        }
        
        // Draw corner handles for selected box
        if (box.isSelected()) {
            painter.setBrush(color);
            int handleSize = 8;
            QPoint topLeft = screenRect.topLeft();
            QPoint topRight = screenRect.topRight();
            QPoint bottomLeft = screenRect.bottomLeft();
            QPoint bottomRight = screenRect.bottomRight();
            
            painter.drawEllipse(topLeft, handleSize/2, handleSize/2);
            painter.drawEllipse(topRight, handleSize/2, handleSize/2);
            painter.drawEllipse(bottomLeft, handleSize/2, handleSize/2);
            painter.drawEllipse(bottomRight, handleSize/2, handleSize/2);
        }
    }
}

void ImageCanvas::mousePressEvent(QMouseEvent *event)
{
    if (m_originalPixmap.isNull()) {
        return;
    }
    
    if (event->button() != Qt::LeftButton) {
        return;
    }
    
    QPoint clickPos = event->pos();
    
    // Check if clicking on image area
    if (!m_imageRect.contains(clickPos)) {
        return;
    }
    
    // Check if clicking on a corner of selected box (for resizing)
    if (m_selectedBoxIndex >= 0) {
        QPoint imagePoint = screenToImage(clickPos);
        BoundingBox::Corner corner = m_boundingBoxes[m_selectedBoxIndex].nearCorner(imagePoint, 10);
        
        if (corner != BoundingBox::None) {
            m_state = Resizing;
            m_resizingCorner = corner;
            m_startPoint = imagePoint;
            return;
        }
    }
    
    // Check if clicking on an existing box
    int boxIndex = findBoxAtPoint(clickPos);
    if (boxIndex >= 0) {
        setSelectedBoxIndex(boxIndex);
        emit boundingBoxSelected(boxIndex);
        m_state = Moving;
        m_startPoint = screenToImage(clickPos);
        return;
    }
    
    // Start drawing new box
    clearSelection();
    m_state = Drawing;
    m_startPoint = screenToImage(clickPos);
    m_currentPoint = m_startPoint;
    m_currentRect = QRect();
}

void ImageCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_originalPixmap.isNull()) {
        return;
    }
    
    QPoint currentPos = event->pos();
    
    if (!m_imageRect.contains(currentPos)) {
        return;
    }
    
    QPoint imagePoint = screenToImage(currentPos);
    
    if (m_state == Drawing) {
        // Update current rectangle
        m_currentPoint = imagePoint;
        
        int x = qMin(m_startPoint.x(), m_currentPoint.x());
        int y = qMin(m_startPoint.y(), m_currentPoint.y());
        int w = qAbs(m_currentPoint.x() - m_startPoint.x());
        int h = qAbs(m_currentPoint.y() - m_startPoint.y());
        
        m_currentRect = QRect(x, y, w, h);
        update();
    }
    else if (m_state == Resizing && m_selectedBoxIndex >= 0) {
        // Resize the selected box
        BoundingBox &box = m_boundingBoxes[m_selectedBoxIndex];
        QRect rect = box.rect();
        
        switch (m_resizingCorner) {
            case BoundingBox::TopLeft:
                rect.setTopLeft(imagePoint);
                break;
            case BoundingBox::TopRight:
                rect.setTopRight(imagePoint);
                break;
            case BoundingBox::BottomLeft:
                rect.setBottomLeft(imagePoint);
                break;
            case BoundingBox::BottomRight:
                rect.setBottomRight(imagePoint);
                break;
            default:
                break;
        }
        
        box.setRect(rect.normalized());
        emit boundingBoxModified(m_selectedBoxIndex);
        update();
    }
}

void ImageCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    
    if (m_state == Drawing) {
        // Finalize the bounding box
        if (m_currentRect.width() > 10 && m_currentRect.height() > 10) {
            emit boundingBoxCreated(m_currentRect);
            emit requestLabelForBox();
        }
        m_currentRect = QRect();
    }
    
    m_state = Idle;
    m_resizingCorner = BoundingBox::None;
}

void ImageCanvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (m_selectedBoxIndex >= 0) {
            removeBoundingBox(m_selectedBoxIndex);
        }
    }
    
    QWidget::keyPressEvent(event);
}

void ImageCanvas::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateScaledImage();
    update();
}

