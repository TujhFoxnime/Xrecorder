#include "areaselector.h"

//#undef Status

#include <QGuiApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDir>
#include <QDateTime>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>



AreaSelector::AreaSelector(QWidget *parent) : QDialog(parent)
{
    //
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //
    QScreen *screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());

    //
    screenCapture = screen->grabWindow(0);

    showFullScreen();
    setCursor(Qt::CrossCursor);
}



void AreaSelector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.drawPixmap(rect(), screenCapture);
    painter.fillRect(rect(), QColor(0, 0, 0, 150));

    //
    if (selecting) {
        QRect selectionRect(startPos, endPos);
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(selectionRect);

        //
        painter.drawPixmap(selectionRect, screenCapture, selectionRect);
    }
}



void AreaSelector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
        endPos = event->pos();
        selecting = true;
        update();
    }
}



void AreaSelector::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting) {
        endPos = event->pos();
        update();
    }
}




void AreaSelector::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && selecting) {
        endPos = event->pos();
        selecting = false;

        //
        QRect selectionRect(startPos, endPos);
        selectionRect = selectionRect.normalized();
        //selectedArea = screenCapture.copy(selectionRect);
        selectedArea = getCursorOnArea(selectionRect, currentCheckState);

        close();
    }
}

void AreaSelector::setCheckState(int state)
{
    currentCheckState = state;
}




void AreaSelector::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}




QPixmap AreaSelector::getCursorOnArea(const QRect& rect, int arg) // получение курсора по чекбоксу
{
    QScreen *screen = QGuiApplication::primaryScreen();

    if (!screenCapture.rect().contains(rect)) {
        qWarning() << "if (!screenCapture.rect().contains(rect))";
        return QPixmap();
    }
    //delay();

    selectedArea = screenCapture.copy(rect);
    qDebug() << "originalPixmap";


    if (arg == 2) {
        QPixmap cursorPixmap = screen->grabWindow(0, QCursor::pos().x(), QCursor::pos().y(), 100, 100);
        QPainter painter(&selectedArea);
        painter.drawPixmap(QCursor::pos().x() - rect.x(), QCursor::pos().y() - rect.y(), cursorPixmap);

        //qDebug() << "arg if";

        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            qWarning() << "Cannot open X display";
            return selectedArea;
        }

        int hotspotX = 0, hotspotY = 0;

        QImage cursorImg = getCursorImage(display, hotspotX, hotspotY);

        Window root = DefaultRootWindow(display);
        int rootX, rootY;
        unsigned int mask;
        Window retRoot, retChild;
        XQueryPointer(display, root, &retRoot, &retChild,
                          &rootX, &rootY, &rootX, &rootY, &mask);

        if (!cursorImg.isNull()) {
            QPainter painter(&selectedArea);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

            int cursorX = rootX - hotspotX - rect.x();
            int cursorY = rootY - hotspotY - rect.y();

            if (QRect(0, 0, rect.width(), rect.height()).contains(QPoint(cursorX, cursorY))) {
                painter.drawImage(cursorX, cursorY, cursorImg);
            } else {
                qWarning() << "Курсор не попал в снимок экрана!";
            }

            painter.end();
        } else {
            qWarning() << "Cursor image is null";
        }

        XCloseDisplay(display);
        qDebug() << "display closed";
    }


    qDebug() << "shootscreen is working";

    return selectedArea;
}


QPixmap AreaSelector::getSelectedArea(/*QPixmap screen, int arg*/)
{
    return selectedArea;
}


void AreaSelector::saveScreenshot(QPixmap screen/*, int arg*/)
{
    const QString format = ".png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    QDateTime curdate = QDateTime::currentDateTime();
    QString nameDate = curdate.toString("yyyy.MM.dd - hh:mm:ss");
    initialPath += tr("/") + nameDate + format;

    // settings menu (выбор папки сохранения default/select)

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
    if (!screen.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"),
                             tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}
