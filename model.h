/**
 * @file model.h
 * @brief Header file for the Model class, managing sprite frames, animation, and fill bucket algorithm.
 * @date 2025-03-31
 * Reviewed by: Jared Pratt
 */

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>
#include <vector>

/**
 * @class Model
 * @brief Manages sprite frames, animation playback, and image operations.
 */
class Model : public QObject
{
    Q_OBJECT

private:
    std::vector<QImage *> frames; ///< Stores animation frames.
    float frameRate = 1; ///< Frame rate of the animation.

    /**
     * @brief Checks if a neighbor pixel is valid for flood fill.
     * @param x X-coordinate of the pixel.
     * @param y Y-coordinate of the pixel.
     * @param image Reference to the image being processed.
     * @param startColor The starting color of the fill region.
     * @return True if the neighbor is valid, false otherwise.
     */
    bool isValidNeighbor(int x, int y, QImage& image, QColor& startColor);

    /**
     * @brief Checks if a pixel coordinate is within the image bounds.
     * @param x X-coordinate of the pixel.
     * @param y Y-coordinate of the pixel.
     * @param image Reference to the image.
     * @return True if the coordinates are within bounds, false otherwise.
     */
    bool isInBounds(int x, int y, QImage& image);

public slots:

    void startAnimation();

    /**
     * @brief Adds a new frame to the animation.
     * @param image Pointer to the QImage frame to add.
     */
    void addFrame(QImage *image);

    /**
     * @brief Updates the animation frame rate.
     * @param value The new frame rate value.
     */
    void updateFrameRate(int value);

    /**
     * @brief Performs a flood fill operation on the image.
     * @param x X-coordinate of the starting point.
     * @param y Y-coordinate of the starting point.
     * @param image Reference to the image being modified.
     * @param fillColor The color to fill with.
     */
    void floodFill(int x, int y, QImage& image, QColor& fillColor);

    /**
     * @brief Retrieves a specific frame from the animation.
     * @param frame The index of the frame to retrieve.
     */
    void getFrame(int frame);

    /**
     * @brief Updates an existing frame with a new image.
     * @param image Pointer to the new QImage.
     * @param frameNum The frame number to update.
     */
    void updateFrame(QImage *image, int frameNum);

    /**
     * @brief Saves the animation to a file.
     * @param filename Reference to the filename string.
     */
    void save(QString &filename);

    /**
     * @brief Loads an animation from a file.
     * @param filename Reference to the filename string.
     */
    void load(QString &filename);

signals:
    /**
     * @brief Signal to set the current animation frame.
     * @param image Pointer to the QImage frame.
     */
    void setAnimationFrame(QImage *image);

    /**
     * @brief Signal to set a pixel's color during flood fill.
     * @param x X-coordinate of the pixel.
     * @param y Y-coordinate of the pixel.
     * @param fillColor The color to set.
     */
    void setPixelFull(int x, int y, QColor& fillColor);

    /**
     * @brief Signal to return a specific frame.
     * @param image Pointer to the QImage frame.
     */
    void returnFrame(QImage *image);

    /**
     * @brief Signal to load animation frames from JSON data.
     * @param frames A vector of QImage pointers representing the frames.
     */
    void loadFromJSON(std::vector<QImage *> frames);
};

#endif // MODEL_H
