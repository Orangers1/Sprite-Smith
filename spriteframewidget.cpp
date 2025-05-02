/**
 * @file spriteframewidget.cpp
 * @brief Implements a QWidget that displays a single frame in a sprite editor.
 * @date 2025-03-31
 * Reviewed by: Woojin Lee
 */

#include "spriteframewidget.h"
#include <QPainter>

SpriteFrameWidget::SpriteFrameWidget(int index, QImage *image)
    : frameNumber(index)
{
    // Make a deep copy of the image if it's not null
    if (image && !image->isNull())
    {
        img = image->copy(); // Create a deep copy of the image
    }

    // Set a fixed size for the widget
    setFixedSize(100, 100);
    setMinimumSize(100, 100);
}

void SpriteFrameWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Send signal to view (mainwindow cpp)
        emit frameSelected(&this->img, frameNumber);
    }

    QWidget::mousePressEvent(event);
}

void SpriteFrameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Draw a border around the widget
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, width() - 1, height() - 1);

    // If image exists, draw it scaled to fit the widget
    if (!img.isNull())
    {
        painter.drawImage(rect(), img);
    }
    else
    {
        // If no image, fill with a light gray color
        painter.fillRect(1, 1, width() - 2, height() - 2, Qt::lightGray);

        // Draw frame number
        painter.drawText(rect(), Qt::AlignCenter, QString::number(frameNumber));
    }
}

void SpriteFrameWidget::updateImage(QImage *image)
{
    img = *image;
    update();
}
