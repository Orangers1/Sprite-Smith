/**
 * @file graphicsview.h
 * @brief Header file for the GraphicsView class, handling user interactions and image rendering.
 * @date 2025-03-31
 * Reviewed by: David Perry
 */

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QImage>
#include <QGraphicsPixmapItem>

/**
 * @class GraphicsView
 * @brief A custom QGraphicsView for handling sprite editing interactions.
 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for GraphicsView.
     * @param parent Optional parent widget.
     */
    explicit GraphicsView(QWidget *parent = nullptr);

    /**
     * @brief Creates a new image with specified dimensions.
     * @param width Width of the image.
     * @param height Height of the image.
     */
    void createImage(int width, int height);

    /**
     * @brief Loads an existing image into the editor.
     * @param image Pointer to the QImage to be loaded.
     */
    void openImage(QImage *image);

    /**
     * @brief Returns a pointer to the main image currently being edited.
     * @return Pointer to the main QImage.
     */
    QImage *getMainImage();

    /**
     * @brief Clears the current image by filling it with transparency.
     */
    void clearImage();

    /**
     * @brief Enables eraser tool mode for drawing with transparency.
     * Disables pencil and fill modes when activated.
     */
    void enableEraser();

    /**
     * @brief Enables pencil tool mode for drawing with the current color.
     * Disables eraser and fill modes when activated.
     */
    void enablePencil();

    /**
     * @brief Enables fill bucket tool mode for filling areas with the current color.
     * Disables pencil and eraser modes when activated.
     */
    void enableFill();

    /**
     * @brief Sets the current drawing color.
     * @param color New drawing color.
     */
    void setDrawColor(const QColor &color);

    /**
     * @brief Sets the color of a specific pixel.
     * @param x X-coordinate of the pixel.
     * @param y Y-coordinate of the pixel.
     * @param color New color for the pixel.
     */
    void setPixelColor(int x, int y, QColor &color);

    /**
     * @brief Renders an onion skin overlay image.
     * @param overlayImage Image to overlay for onion skinning.
     */
    void renderOnionSkin(QImage *overlayImage);

protected:
    /**
     * @brief Handles mouse press events for drawing actions.
     * @param event Pointer to the mouse event.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse move events for continuous drawing.
     * @param event Pointer to the mouse event.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Handles resizing events to maintain image scaling.
     * @param event Pointer to the resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *m_scene;  ///< Scene to hold the image for rendering.
    QImage m_image;  ///< Main image being edited.
    QGraphicsPixmapItem *m_pixmapItem; ///< Pixmap item representing the image.
    QColor m_drawColor; ///< Current drawing color.
    QColor m_drawColor_copy; ///< Backup copy of the drawing color.
    bool isEraser; ///< Flag to indicate eraser mode.
    bool isPencil; ///< Flag to indicate pencil mode.
    bool isFill; ///< Flag to indicate bucket fill mode.
    QGraphicsPixmapItem *overlayLayer; ///< Overlay layer for onion skinning.

    /**
     * @brief Renders the main image onto the scene.
     */
    void renderMainImage();

signals:
    /**
     * @brief Signal emitted to trigger flood fill.
     * @param x X-coordinate of the fill start point.
     * @param y Y-coordinate of the fill start point.
     * @param image Reference to the image being filled.
     * @param fillColor Color to be used for filling.
     */
    void floodFill(int x, int y, QImage &image, QColor &fillColor);

    /**
     * @brief Signal emitted when pencil tool is activated.
     */
    void pencilEnabled();

    /**
     * @brief Signal emitted when eraser tool is activated.
     */
    void eraserEnabled();

    /**
     * @brief Signal emitted when fill bucket tool is activated.
     */
    void bucketFillEnabled();
};

#endif // GRAPHICSVIEW_H
