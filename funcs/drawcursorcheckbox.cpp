#include "drawcursorcheckbox.h"

#include <QDebug>

/*drawCursorCheckbox::drawCursorCheckbox()
{

}*/



QImage getCursorImage(Display *display, int &hotspotX, int &hotspotY) {
    qDebug() << "get img cursor1";
    XFixesCursorImage *cursor = XFixesGetCursorImage(display);
    if (!cursor)
        return QImage();

    hotspotX = cursor->xhot;
    hotspotY = cursor->yhot;

    int width = cursor->width;
    int height  =cursor->height;

    const unsigned long *pixels = cursor->pixels;

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);

    for (int y = 0; y < height; ++y) {
        QRgb *scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < width; ++x) {
            unsigned long argb = pixels[y * width + x];

            unsigned char a = (argb >> 24) & 0xFF;
            unsigned char r = (argb >> 16) & 0xFF;
            unsigned char g = (argb >> 8) & 0xFF;
            unsigned char b = argb & 0xFF;

            scanLine[x] = qRgba(r, g, b, a);
        }
    }
    QImage cursorImg = image.copy();
    XFree(cursor);
    qDebug() << "get img cursor2";
    return cursorImg;
}
