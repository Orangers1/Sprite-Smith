/**
 * @file graphicsview.cpp
 * @brief Implementation the GraphicsView class, handling user interactions and image rendering.
 * @date 2025-03-31
 * Reviewed by: David Perry
 */

#include "graphicsview.h"
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPixmap>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent),

    m_scene(new QGraphicsScene(this)),
    m_pixmapItem(nullptr),
    m_drawColor(Qt::black),
    isEraser(false),
    isPencil(true),
    isFill(false)
{
    // Start the drawing scene, set the Resize Anchor for scaling
    setScene(m_scene);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}

QImage *GraphicsView::getMainImage()
{
    return &m_image;
}

void GraphicsView::openImage(QImage *image)
{
    m_image = *image;
    renderMainImage();
}

void GraphicsView::renderMainImage()
{
    // Removes the previous pixel map, ensures that the pixel map loads correctly
    if (m_pixmapItem) {
        m_scene->removeItem(m_pixmapItem);
        delete m_pixmapItem;
        m_pixmapItem = nullptr;
    }

    // Start the GraphicsPixMapItem object
    m_pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_image));
    m_scene->addItem(m_pixmapItem);
    m_scene->setSceneRect(0, 0, m_image.width(), m_image.height());

    // Resets the transform scalar, then scales the image
    resetTransform();
    fitInView(m_scene->sceneRect(), Qt::IgnoreAspectRatio);
}

void GraphicsView::createImage(int width, int height)
{
    m_image = QImage(width, height, QImage::Format_ARGB32);

    // Sets the background color to transparent
    m_image.fill(Qt::transparent);

    renderMainImage();
}

void GraphicsView::clearImage()
{
    m_image.fill(Qt::transparent);
    renderMainImage();
}

void GraphicsView::enableEraser()
{
    isEraser = true;
    isPencil = false;
    isFill = false;

    m_drawColor = Qt::transparent;

    emit eraserEnabled();
}

void GraphicsView::enablePencil()
{
    isPencil = true;
    isEraser = false;
    isFill = false;

    m_drawColor = m_drawColor_copy;

    emit pencilEnabled();
}

void GraphicsView::enableFill()
{
    isFill = true;
    isPencil = false;
    isEraser = false;

    emit bucketFillEnabled();
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    // Convert mouse position to coordinates
    QPointF scenePos = mapToScene(event->pos());
    int x = static_cast<int>(scenePos.x());
    int y = static_cast<int>(scenePos.y());

    // Make sure we are within bounds, and then draw the pixel with the drawColor
    if (x >= 0 && x < m_image.width() &&
        y >= 0 && y < m_image.height())
    {
        if (isPencil || isEraser)
        {
            m_image.setPixelColor(x, y, m_drawColor);
            m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
        }
        else if (isFill)
        {
            emit floodFill(x, y, m_image, m_drawColor);
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    // Repeatedly calls mousePressEvent if the user is holding the left click
    if (event->buttons() & Qt::LeftButton) {

        mousePressEvent(event);
    } else {

        QGraphicsView::mouseMoveEvent(event);
    }
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    if (scene()) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

void GraphicsView::setDrawColor(const QColor &color)
{
    m_drawColor = color;
    m_drawColor_copy = color;
}

void GraphicsView::setPixelColor(int x, int y, QColor& color)
{
    m_image.setPixelColor(x, y, color);
    m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
}

void GraphicsView::renderOnionSkin(QImage *overlayImage)
{
    if (!overlayImage) {
        m_scene->removeItem(overlayLayer);
        delete overlayLayer;
        overlayLayer = nullptr;
    } else {
        // m_image is base layer, we put the onion skin on top of it.
        overlayLayer = new QGraphicsPixmapItem(QPixmap::fromImage(*overlayImage));
        overlayLayer->setZValue(1); // Higher z-value means it appears on top

        // make partially transparent
        overlayLayer->setOpacity(0.5);
        m_scene->addItem(overlayLayer);
    }
}
