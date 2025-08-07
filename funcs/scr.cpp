#include "mainwindow.h"
#include "scr.h"

#include <QGroupBox>
#include <QStandardPaths>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QGuiApplication>
#include <QWindow>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPainter>

#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>


//#include <>




void delay() {
    QTime dieTime = QTime::currentTime().addSecs(3);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


Screenshot::Screenshot() : screenshotLabel(new QLabel(this))
{
    screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = screen()->geometry();
    screenshotLabel->setMinimumSize(screenGeometry.width()
                                    / 8, screenGeometry.height() / 8);

    QGroupBox *optionsGroupBox = new QGroupBox(tr("Options"), this);
    delaySpinBox = new QSpinBox(optionsGroupBox);
    delaySpinBox->setSuffix(tr("s"));
    delaySpinBox->setMaximum(60);

    /*connect(delaySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &Screenshot::updateCheckBox);*/

    hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"), optionsGroupBox);

    QGridLayout *optionsGroupBoxLayout = new QGridLayout(optionsGroupBox);
    optionsGroupBoxLayout->addWidget(new QLabel(tr("Screenshot Delay:"), this), 0, 0);
    optionsGroupBoxLayout->addWidget(delaySpinBox, 0 ,1);
    optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);

    //mainLayout->addWidget(optionsGroupBox);

    //QHBoxLayout *buttonsLayout = new QHBoxLayout;
    newScreenshotButton = new QPushButton(tr("New Screenshot"), this);
    connect(newScreenshotButton, &QPushButton::clicked, this, &Screenshot::newScreenshot);
    //buttonsLayout->addWidget(newScreenshotButton);

    QPushButton *saveScreenshotButton = new QPushButton(tr("Save Screenshot"), this);
    connect(saveScreenshotButton, &QPushButton::clicked, this, &Screenshot::saveScreenshot);
    //buttonsLayout->addWidget(saveScreenshotButton);

}


void Screenshot::saveScreenshot()
{
    //const QString namePicture = "yyyy.MM.dd_hh:mm:ss";
    //QString initialPath = QDir::homePath();

    //originalPixmap.save(namePicture);



    const QString format = ".png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    QDateTime curdate = QDateTime::currentDateTime();
    QString nameDate = curdate.toString("yyyy.MM.dd - hh:mm:ss");
    initialPath += tr("/") + nameDate + format;

    // settings menu


    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);

    QStringList mimeTypes;
    const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
    for (const QByteArray &bf : baMimeTypes)
        mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/png");
    fileDialog.setDefaultSuffix("png");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fileName = /*nameDate;*/fileDialog.selectedFiles().first();
    if (!originalPixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"),
                             tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}


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



QPixmap Screenshot::shootScreen(const QRect& rect, int arg)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    /*if (const QWindow *window = windowHandle())
        screen = window->screen();*/
    if (!screen)
        return QPixmap();
    /*if (delaySpinBox->value() != 0)
        QApplication::beep();*/

    delay();

    originalPixmap = screen->grabWindow(0, rect.x(),
                                        rect.y(), rect.width(),
                                        rect.height());
    qDebug() << "originalPixmap";


    if (arg == 2) {
        /*QPixmap cursorPixmap = screen->grabWindow(0, QCursor::pos().x(), QCursor::pos().y(), 100, 100);
        QPainter painter(&originalPixmap);
        painter.drawPixmap(QCursor::pos().x() - rect.x(), QCursor::pos().y() - rect.y(), cursorPixmap);*/

        qDebug() << "arg if";

        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            qWarning() << "Cannot open X display";
            return originalPixmap;
        }

        int hotspotX = 0;
        int hotspotY = 0;

        QImage cursorImg = getCursorImage(display, hotspotX, hotspotY);

        Window root = DefaultRootWindow(display);
        int rootX, rootY, winX, winY;
        unsigned int mask;
        Window retRoot, retChild;
        XQueryPointer(display, root, &retRoot, &retChild,
                          &rootX, &rootY, &winX, &winY, &mask);

        if (!cursorImg.isNull()) {
            QPainter painter(&originalPixmap);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

            int cursorX = rootX - hotspotX - rect.x();
            int cursorY = rootY - hotspotY - rect.y();

            painter.drawImage(cursorX, cursorY, cursorImg);

            painter.end();
        } else {
            qWarning() << "Cursor image is null";
        }

        XCloseDisplay(display);
        qDebug() << "display closed";;
    }

    updateScreenshotLabel();

    newScreenshotButton->setDisabled(false);
    if (hideThisWindowCheckBox->isChecked())
        show();
    qDebug() << "shootscreen is working";
    Screenshot::saveScreenshot();
    return originalPixmap;
}


void Screenshot::updateCheckBox()
{
    if (delaySpinBox->value() == 0)
    {
        hideThisWindowCheckBox->setDisabled(true);
        hideThisWindowCheckBox->setChecked(false);
    } else {
        hideThisWindowCheckBox->setDisabled(false);
    }
}

void Screenshot::updateScreenshotLabel()
{
    screenshotLabel->setPixmap(originalPixmap
                               .scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation
                                       ));
}


void Screenshot::newScreenshot()
{
    if (hideThisWindowCheckBox->isChecked())
        hide();
    newScreenshotButton->setDisabled(true);

    qDebug() << "newscr1";
    QTimer::singleShot(delaySpinBox->value() * 1000, this, [this]() {
        QRect fullScreenRect = QGuiApplication::primaryScreen()->geometry();
        shootScreen(fullScreenRect, hideThisWindowCheckBox->isChecked() ? 2 : 0);
    });
    qDebug() << "newscr2";
}

