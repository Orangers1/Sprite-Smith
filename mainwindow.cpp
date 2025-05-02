/**
 * @file mainwindow.cpp
 * @brief Implementation the MainWindow class, managing user interactions and rendering.
 * @date 2025-03-31
 * Reviewed by: Jared Pratt
 */

#include "mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QImage>
#include "model.h"
#include "spriteframewidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Model *model)
    : QMainWindow(parent), ui(new Ui::MainWindow), canvasView(new GraphicsView(this)), previousAnimationFrame(nullptr)
{
    ui->setupUi(this);

    // Instantiate the siders and labels
    ui->redSlider->setRange(0, 255);
    ui->greenSlider->setRange(0, 255);
    ui->blueSlider->setRange(0, 255);
    ui->alphaSlider->setRange(0, 255);
    ui->redSlider->setValue(0);
    ui->greenSlider->setValue(0);
    ui->blueSlider->setValue(0);
    ui->alphaSlider->setValue(255);
    ui->currentToolName->setText("Pencil");
    ui->resolutionSlider->setMinimum(8);
    ui->resolutionSlider->setMaximum(64);
    ui->resolutionSlider->setValue(32);

    // Connections for updating the json info with these signal values
    connect(ui->redSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);
    connect(ui->greenSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);
    connect(ui->blueSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);
    connect(ui->alphaSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);

    // Connections for saving changes
    connect(ui->saveChangesButton, &QPushButton::clicked, this, &MainWindow::handleSaveChanges);
    connect(this, &MainWindow::saveChanges, model, &Model::updateFrame);

    // Connections for saving and saving changes
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handleSave);
    connect(this, &MainWindow::save, model, &Model::save);

    // Connections for loading
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::handleLoad);
    connect(this, &MainWindow::load, model, &Model::load);
    connect(model, &Model::loadFromJSON, this, &MainWindow::handleLoadFromJSON);

    // Connections for frames and animation
    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::createNewFrame);
    connect(ui->startAnimationButton, &QPushButton::clicked, model, &Model::startAnimation);
    connect(ui->frameRateSlider, &QSlider::valueChanged, model, &Model::updateFrameRate);
    connect(this, &MainWindow::newFrameCreated, model, &Model::addFrame);
    connect(model, &Model::returnFrame, this, &MainWindow::receiveFrame);
    connect(model, &Model::setAnimationFrame, this, &MainWindow::displayAnimationFrame);

    // Connections for onion skinning
    connect(ui->onionSkinToggle, &QPushButton::clicked, this, &MainWindow::toggleOnionSkin);
    connect(this, &MainWindow::getOnionSkinFrame, model, &Model::getFrame);

    // Connections for fill bucket
    connect(ui->fillBucketButton, &QPushButton::clicked, this, &MainWindow::enableFill);
    connect(this, &MainWindow::floodFill, model, &Model::floodFill);
    connect(model, &Model::setPixelFull, this, &MainWindow::setPixel);
    connect(canvasView, &GraphicsView::floodFill, this, &MainWindow::floodFillReceive);
    connect(canvasView, &GraphicsView::bucketFillEnabled, this, &MainWindow::bucketFillEnabled);

    // Connections for pencil, eraser, and clear
    connect(ui->clearImageButton, &QPushButton::clicked, this, &MainWindow::clearImage);
    connect(ui->eraserButton, &QPushButton::clicked, this, &MainWindow::enableEraser);
    connect(ui->pencilButton, &QPushButton::clicked, this, &MainWindow::enablePencil);
    connect(canvasView, &GraphicsView::pencilEnabled, this, &MainWindow::pencilEnabled);
    connect(canvasView, &GraphicsView::eraserEnabled, this, &MainWindow::eraserEnabled);

    // Connection for resolution slider
    connect(ui->resolutionSlider, &QSlider::valueChanged, this, &MainWindow::resolutionSliderChanged);

    // Load the widget space for the drawing area
    QVBoxLayout *layout = new QVBoxLayout(ui->imageWorkspace);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(canvasView);

    updateColorFromSliders();

    // Create the image using height and width values
    canvasView->createImage(currentResolution, currentResolution);

    // Initialize animation scene and view
    animationGraphicsScene = new QGraphicsScene(this);
    animationView.setScene(animationGraphicsScene);

    // Set up animation preview area
    QVBoxLayout *animationLayout = new QVBoxLayout(ui->animationPreviewFrame);
    animationLayout->setContentsMargins(0, 0, 0, 0);
    animationLayout->setSpacing(0);
    animationLayout->addWidget(&animationView);

    frameLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
}

MainWindow::~MainWindow()
{
    if (previousAnimationFrame)
    {
        delete previousAnimationFrame;
    }
    delete animationGraphicsScene;
    delete ui;
    delete frameLayout;
}

void MainWindow::createNewFrame()
{
    frameCount++;
    currentFrame = frameCount; // realign current frame
    QImage *qi = canvasView->getMainImage();
    QImage *deepCopy = new QImage(qi->copy());

    // Emit a signal to the model that contains the QImage
    emit newFrameCreated(deepCopy);

    // Create the sprite frame widget first, which will make a copy of the current image
    SpriteFrameWidget *sfw = new SpriteFrameWidget(frameCount, deepCopy);

    connect(sfw, &SpriteFrameWidget::frameSelected, this, &MainWindow::updateMainImage);

    listOfFrames.insert(listOfFrames.begin(), sfw);
    frameLayout->insertWidget(0, sfw);

    // Reset after creation of the sprite frame widget with a copy of the current image
    canvasView->createImage(currentResolution, currentResolution);
}

void MainWindow::clearImage()
{
    canvasView->clearImage();
}

void MainWindow::enablePencil()
{
    canvasView->enablePencil();
}

void MainWindow::pencilEnabled(){
    ui->currentToolName->setText("Pencil");
}

void MainWindow::enableEraser()
{
    canvasView->enableEraser();
}

void MainWindow::eraserEnabled(){
    ui->currentToolName->setText("Eraser");
}

void MainWindow::enableFill(){
    canvasView->enableFill();
}

void MainWindow::bucketFillEnabled(){
    ui->currentToolName->setText("Fill Bucket");
}

void MainWindow::floodFillReceive(int x, int y, QImage& image, QColor& fillColor){
    emit floodFill(x, y, image, fillColor);
}

void MainWindow::setPixel(int x, int y, QColor& color)
{
    canvasView->setPixelColor(x, y, color);
}

void MainWindow::updateColorFromSliders()
{
    // Get current values from the sliders
    int r = ui->redSlider->value();
    int g = ui->greenSlider->value();
    int b = ui->blueSlider->value();
    int a = ui->alphaSlider->value();

    // Update the labels
    ui->redValueLabel->setText(QString::number(r));
    ui->greenValueLabel->setText(QString::number(g));
    ui->blueValueLabel->setText(QString::number(b));
    ui->alphaValueLabel->setText(QString::number(a));

    // Create a new QColor )
    QColor newColor(r, g, b, a);

    // Update the color display circle, inspired from some code on Qc website
    int diameter = ui->colorDisplay->width();
    ui->colorDisplay->setStyleSheet(
        QString("background-color: %1; border-radius: %2px; border: 1px solid black;")
            .arg(newColor.name(QColor::HexArgb))
            .arg(diameter / 2));

    // Use setDrawColor with the new color
    canvasView->setDrawColor(newColor);
}

void MainWindow::resolutionSliderChanged(int value)
{
    currentResolution = value;
    canvasView->createImage(currentResolution, currentResolution);
}

void MainWindow::updateMainImage(QImage *image, int frameNum)
{
    currentFrame = frameNum;
    canvasView->openImage(image);

    if (ui->onionSkinToggle->checkState()) {
        canvasView->renderOnionSkin(nullptr);
        toggleOnionSkin();
    }
}

void MainWindow::displayAnimationFrame(QImage *image)
{
    // Clear previous pixelmap
    if (previousAnimationFrame)
    {
        animationGraphicsScene->removeItem(previousAnimationFrame);
        delete previousAnimationFrame;
        previousAnimationFrame = nullptr;
    }

    // Create pixmap
    previousAnimationFrame = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    animationGraphicsScene->addItem(previousAnimationFrame);
    animationGraphicsScene->setSceneRect(0, 0, (*image).width(), (*image).height());

    // Reset and scale
    animationView.resetTransform();
    animationView.fitInView(animationGraphicsScene->sceneRect(), Qt::IgnoreAspectRatio);
}

void MainWindow::toggleOnionSkin()
{
    if (currentFrame >= 2) {
        // bridge the gap between frameCount and indexing the vector.
        if (ui->onionSkinToggle->checkState()) {
            emit getOnionSkinFrame(currentFrame - 2); // vector
        } else {
            canvasView->renderOnionSkin(nullptr);
        }
    }
}

void MainWindow::receiveFrame(QImage *image)
{
    this->onionFrame = image;
    canvasView->renderOnionSkin(image);
}

void MainWindow::handleSaveChanges()
{
    // Signal the respective sprite frame to update
    if (frameCount > 0) {
        QImage *deepCopy = new QImage(canvasView->getMainImage()->copy());

        SpriteFrameWidget *sfw = listOfFrames[frameCount - currentFrame];
        sfw->updateImage(deepCopy);

        emit saveChanges(deepCopy, currentFrame);
    }
}

void MainWindow::handleSave()
{
    QString filename
        = QFileDialog::getSaveFileName(this, tr("Save Sprite Sheet"), "", tr("Sprite Sheet Files (*.ssp);;All Files (*)"));

    if (!filename.isEmpty()) {
        // Add .ssp extension if not specified
        if (!filename.endsWith(".ssp", Qt::CaseInsensitive)) {
            filename += ".ssp";
        }
        emit save(filename);
    }
}

void MainWindow::handleLoad()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Sprite Sheet"), "", tr("Sprite Sheet Files (*.ssp);;All Files (*)"));
    if (!filename.isEmpty()) {
        emit load(filename);
    }
}

void MainWindow::handleLoadFromJSON(std::vector<QImage *> frames)
{
    // Remove all existing sprite frame widgets from the QSCrollArea
    for (auto frame : listOfFrames) {
        frameLayout->removeWidget(frame);
        delete frame;
    }

    listOfFrames.clear();

    // Recreate all the sfw's
    for (size_t i = 0; i < frames.size(); i++) {
        SpriteFrameWidget *sfw = new SpriteFrameWidget(i + 1, frames[i]);
        connect(sfw, &SpriteFrameWidget::frameSelected, this, &MainWindow::updateMainImage);
        listOfFrames.insert(listOfFrames.begin(), sfw); // Aligns the frontend sprite frame widgets with the backend QImages
        frameLayout->insertWidget(0, sfw); // Populate the QScrollArea
    }

    // Update frontend variables
    frameCount = frames.size();
    currentFrame = 1;

    // Display first frame by default
    if (!frames.empty()) {
        canvasView->openImage(frames[0]);
    }

    this->onionFrame = nullptr;

    update();
}
