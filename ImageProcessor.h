#pragma once
#include <QImage>

namespace ImageProcessor {

// Adjust brightness by adding delta in range [-100,100] to RGB channels (alpha preserved)
QImage adjustBrightness(const QImage& src, int delta);

}