/**
 * @file spriteframewidget.h
 * @brief Header file for the SpriteFrameWidget class, handling individual animation frames.
 * @date 2025-03-31
 * Reviewed by: Josh Eggett
 */

#ifndef SPRITEFRAMEWIDGET_H
#define SPRITEFRAMEWIDGET_H

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

/**
 * @class SpriteFrameWidget
 * @brief Represents a single frame in the sprite animation.
 */
class SpriteFrameWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for SpriteFrameWidget.
     * @param index The frame number of the frame in the animation.
     * @param image Pointer to the image representing this frame.
     */
    SpriteFrameWidget(int index, QImage *image);

    /**
     * @brief Updates the displayed image.
     * @param image Pointer to the new image.
     */
    void updateImage(QImage *image);

protected:
    /**
     * @brief Handles painting the frame.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Handles mouse press events.
     * @param event The mouse event.
     */
    void mousePressEvent(QMouseEvent *event) override;

private: 
    QImage img; ///< Stores the image data for this frame.
    int frameNumber = 0; ///< The index of this frame in the animation.

signals:
    /**
     * @brief Emitted when this frame is selected to open the image for editing.
     * @param image Pointer to the selected image data.
     * @param frameNum The frame number of the selected image.
     */
    void frameSelected(QImage *image, int frameNum);
};

#endif // SPRITEFRAMEWIDGET_H
