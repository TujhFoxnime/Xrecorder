#ifndef DRAWCURSORCHECKBOX_H
#define DRAWCURSORCHECKBOX_H


#include <QImage>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>


QImage getCursorImage(Display *display, int &hotspotX, int &hotspotY);


/*class drawCursorCheckbox
{
public:
    drawCursorCheckbox();
};*/

#endif // DRAWCURSORCHECKBOX_H
