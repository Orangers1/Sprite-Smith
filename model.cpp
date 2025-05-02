/**
 * @file model.cpp
 * @brief Implementation of the Model class, managing sprite frames, animation, and fill bucket algorithm.
 * @date 2025-03-31
 * Reviewed by: Jared Pratt
 */

#include "model.h"
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QWidget>
#include <queue>
#include <vector>

void Model::startAnimation()
{
    // Emit the signal in a QTimer within a loop
    for (int i = 0; i < (int) frames.size(); i++) {
        QTimer::singleShot((1 / frameRate) * 1000 * i, this, [=]() {
            emit setAnimationFrame(frames[i]);
        });
    }
}

void Model::addFrame(QImage *image)
{
    frames.push_back(image);
}

void Model::updateFrameRate(int value)
{
    frameRate = value;
}

void Model::floodFill(int x, int y, QImage& image, QColor& fillColor)
{
    std::vector<int> startPixel = {x, y};
    std::queue<std::vector<int>> pixelQueue;
    QColor startColor = image.pixelColor(x, y);

    if (startColor == fillColor) {
        return;
    }

    emit setPixelFull(x, y, fillColor);

    // Flood fill algorithm using BFS
    pixelQueue.push(startPixel);
    while (!pixelQueue.empty())
    {
        std::vector<int> currentPixel = pixelQueue.front();
        pixelQueue.pop();

        int currentX = currentPixel[0];
        int currentY = currentPixel[1];

        // Right neighbor
        if (isValidNeighbor(currentX+1, currentY, image, startColor)){
            pixelQueue.push(std::vector<int> {currentX + 1, currentY});
            emit setPixelFull(currentX + 1, currentY, fillColor);
        }
        // Left neighbor
        if (isValidNeighbor(currentX-1, currentY, image, startColor)){
            pixelQueue.push(std::vector<int> {currentX - 1, currentY});
            emit setPixelFull(currentX - 1, currentY, fillColor);
        }
        // Bottom neighbor
        if (isValidNeighbor(currentX, currentY+1, image, startColor)){
            pixelQueue.push(std::vector<int> {currentX, currentY + 1});
            emit setPixelFull(currentX, currentY + 1, fillColor);
        }
        // Top neighbor
        if (isValidNeighbor(currentX, currentY-1, image, startColor)){
            pixelQueue.push(std::vector<int> {currentX, currentY - 1});
            emit setPixelFull(currentX, currentY - 1, fillColor);
        }
    }
}

bool Model::isValidNeighbor(int x, int y, QImage& image, QColor& startColor){
    return isInBounds(x, y, image) && (image.pixelColor(x, y) == startColor);
}

bool Model::isInBounds(int x, int y, QImage& image){
    return (x >= 0 && x < image.width() &&
            y >= 0 && y < image.height());
}

void Model::getFrame(int frame)
{
    emit returnFrame(frames[frame]);
}

void Model::updateFrame(QImage *image, int frameNum)
{
    // Update models representation of the frame
    if (frameNum > 0 && frameNum <= (int) frames.size()) {
        frames[frameNum - 1] = image;
    }
}

void Model::save(QString &filename)
{
    QJsonObject root;

    // Loop through QImages vector
    QJsonArray framesArray;
    for (size_t i = 0; i < frames.size(); i++) {
        QJsonObject frameObj;
        frameObj["index"] = static_cast<int>(i);
        frameObj["resolution"] = frames[i]->width();
        frameObj["bytesPerLine"] = frames[i]->bytesPerLine();

        QByteArray pixelData(reinterpret_cast<const char *>(
                                 frames[i]->bits()), // Frames[i]->bits() are the raw bits
                             frames[i]->bytesPerLine()
                                 * frames[i]->height()); // Dimensions
        frameObj["pixelData"] = QString(pixelData.toBase64());

        framesArray.append(frameObj); // Append base 64 version of bytearray
    }
    root["frames"] = framesArray;

    // Write to file
    QJsonDocument doc(root);
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void Model::load(QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    // Wipe the vector
    for (auto frame : frames) {
        delete frame;
    }
    frames.clear();

    // Parse the incoming json
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    QJsonArray framesArray = root["frames"].toArray();

    // Reconstruct data from the json back into usable byte arrays and Qimages
    for (int i = 0; i < framesArray.size(); i++) {
        auto frameObj = framesArray[i];
        QJsonObject frame = frameObj.toObject();

        // Get frame dimensions and create new QImage
        int resolution = frame["resolution"].toInt();
        QImage *image = new QImage(resolution, resolution, QImage::Format_ARGB32);

        // Get the bytes per line
        int bytesPerLine = frame["bytesPerLine"].toInt();

        // Decode the base64 pixel data
        QByteArray pixelData = QByteArray::fromBase64(frame["pixelData"].toString().toUtf8());

        // Copy the pixel data back into the image
        memcpy(image->bits(), pixelData.constData(), bytesPerLine * resolution);

        frames.push_back(image); // refill the model-side vector
    }

    // Send to mainwindow
    emit loadFromJSON(frames);
}
