#include "ImageProcessor.h"
#include <QtGlobal>

namespace ImageProcessor {

static inline uchar clampToByte(int v) {
    return static_cast<uchar>(qBound(0, v, 255));
}

QImage adjustBrightness(const QImage& src, int delta) {
    if (src.isNull() || delta == 0) return src;

    QImage out = src.convertToFormat(QImage::Format_ARGB32);
    const int height = out.height();
    const int width = out.width();

    for (int y = 0; y < height; ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(out.scanLine(y));
        for (int x = 0; x < width; ++x) {
            const QRgb px = line[x];
            const int r = clampToByte(qRed(px)   + delta);
            const int g = clampToByte(qGreen(px) + delta);
            const int b = clampToByte(qBlue(px)  + delta);
            line[x] = qRgba(r, g, b, qAlpha(px));
        }
    }
    return out;
}

}