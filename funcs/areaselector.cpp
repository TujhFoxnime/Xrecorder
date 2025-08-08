#include "areaselector.h"

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
        selectedArea = screenCapture.copy(selectionRect);

        close();
    }
}




void AreaSelector::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}




QPixmap AreaSelector::getSelectedArea() const
{
    return selectedArea;
}


void AreaSelector::saveScreenshot(QPixmap screen)
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
    if (!screen.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"),
                             tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}
