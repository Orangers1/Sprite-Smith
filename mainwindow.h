/**
 * @file mainwindow.h
 * @brief Header file for the MainWindow class, managing user interactions and rendering.
 * @date 2025-03-31
 * Reviewed by: Isaac Huntsman
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "graphicsview.h"
#include "model.h"
#include "spriteframewidget.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main application window managing user interactions and rendering.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow.
     * @param parent Optional parent widget.
     * @param model Pointer to the model managing sprite data.
     */
    MainWindow(QWidget *parent = nullptr, Model *model = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui; ///< Pointer to the UI instance.
    GraphicsView *canvasView; ///< Graphics view for sprite editing.
    QVBoxLayout *frameLayout; ///< Layout for organizing sprite frames.
    QGraphicsPixmapItem *previousAnimationFrame; ///< Holds the previous frame in animation preview.
    QGraphicsScene *animationGraphicsScene; ///< Scene for displaying animation frames.
    QGraphicsView animationView; ///< View for rendering the animation.
    QImage *onionFrame; ///< Pointer to the onion skin frame overlay.
    int frameCount = 0; ///< Total number of frames in the animation.
    int currentResolution = 32; ///< Current resolution of the sprite grid.
    int currentFrame = 0; ///< Index of the currently active frame.
    std::vector<SpriteFrameWidget *> listOfFrames; ///< Stores all frames in the animation.

    void handleSaveChanges(); ///<handles saving changes

    void renderOnionSkin(); ///<renders the onion skin (previous frame)

public slots:

    void createNewFrame(); ///< handles creating new frame

    void clearImage(); ///<handles clearing image

    void enableEraser(); ///< turn on eraser

    void enablePencil(); ///< turn on pencil

    void enableFill(); ///< turn on fill

    void updateColorFromSliders(); ///< update color

    /**
     * @brief Handles resolution slider changes.
     * @param value The new resolution value.
     */
    void resolutionSliderChanged(int value);

    /**
     * @brief Updates the main image with the given frame data.
     * @param image Pointer to the image to update.
     * @param frameNum The frame number being updated.
     */
    void updateMainImage(QImage *image, int frameNum);

    /**
     * @brief Displays an animation frame.
     * @param image The image frame to display.
     */
    void displayAnimationFrame(QImage *image);

    /**
     * @brief Performs a flood fill operation on the image.
     * @param x X-coordinate of the starting point.
     * @param y Y-coordinate of the starting point.
     * @param image Reference to the image being modified.
     * @param fillColor The color to fill with.
     */
    void floodFillReceive(int x, int y, QImage& image, QColor& fillColor);

    /**
     * @brief Sets the color of a specific pixel.
     * @param x X-coordinate of the pixel.
     * @param y Y-coordinate of the pixel.
     * @param color The color to set.
     */
    void setPixel(int x, int y, QColor& color);

    void toggleOnionSkin();

    /**
     * @brief Receives and processes a new frame for onion skinning.
     * @param image Pointer to the new QImage frame.
     */
    void receiveFrame(QImage *image);

    void handleSave(); ///< handles save

    void handleLoad(); ///< handles loading

    /**
     * @brief Loads a set of frames from JSON data.
     * @param frames A vector of QImage pointers representing the frames.
     */
    void handleLoadFromJSON(std::vector<QImage *> frames);

    void pencilEnabled(); ///<exists to show the user which tool is selected

    void eraserEnabled(); ///<exists to show the user which tool is selected

    void bucketFillEnabled(); ///<exists to show the user which tool is selected

signals:
    /**
     * @brief Emitted when a new frame is created.
     * @param qi Pointer to the new QImage frame.
     */
    void newFrameCreated(QImage *qi);

    /**
     * @brief Signal for performing a flood fill operation.
     * @param x X-coordinate of the starting point.
     * @param y Y-coordinate of the starting point.
     * @param image Reference to the image being modified.
     * @param fillColor The color to fill with.
     */
    void floodFill(int x, int y, QImage& image, QColor& fillColor);

    /**
     * @brief Requests an onion skin frame.
     * @param frame The frame number to retrieve.
     */
    void getOnionSkinFrame(int frame);

    /**
     * @brief Saves changes to the current frame.
     * @param image The image being saved.
     * @param frameNum The frame number being updated.
     */
    void saveChanges(QImage *image, int frameNum);

    /**
     * @brief Signal for saving a file.
     * @param filename Reference to the filename string.
     */
    void save(QString &filename);

    /**
     * @brief Signal for loading a file.
     * @param filename Reference to the filename string.
     */
    void load(QString &filename);
};

#endif // MAINWINDOW_H
