#include "paintarea.h"
#include "data.h"
#include "data.h"
#include "mst.h"
#include <QCursor>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QImage>
#include <QSize>
#include <QPen>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QPrinter>
#include <QPrintDialog>

#include <QtDebug>


PaintArea::PaintArea(QWidget *parent,int width,int height,
                     QColor back,
                     QColor point,
                     QColor line):
    QWidget(parent),
    backColor(back),pointColor(point),lineColor(line),
    scale(1),width(width),height(height),askPaintLine(true)
{
    setMouseTracking(true);
    image = QImage(width, height, QImage::Format_RGB32);
    image.fill(backColor.rgb());
}

void PaintArea::setImageSize(int width, int height)
{
    QImage newImage(width, height, QImage::Format_RGB32);
    image = newImage;
    update();
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(scale,scale);
    painter.drawImage(0,0,image);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) //当鼠标左键按下
    {
        QPoint sf(event->pos()/scale);
        from.setX(sf.x()/5);
        from.setY(sf.y()/5);
        movingfrom.setX(sf.x()/5);
        movingfrom.setY(sf.y()/5);
        //        qDebug() << "get pos from" << from;
    }

}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    QPoint tmp(event->pos()/scale);

    movingto.setX(tmp.x()/5);
    movingto.setY(tmp.y()/5);
    if(event->buttons()&Qt::LeftButton)
    {
        askPaintLine = false;
        emit movingPoint(from, movingto);
        askPaintLine = true;
        movingfrom = movingto;
    }
    else emit onPoint(movingto);
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) //如果鼠标左键释放
    {
        QPoint st(event->pos()/scale);
        to.setX(st.x()/5);
        to.setY(st.y()/5);
        if ( from == to )
            emit addPoint(from);
        else emit movePoint(from, to);
    }
    else if (event->button() == Qt::RightButton)
    {
        QPoint point((event->pos()/scale).x()/5, (event->pos()/scale).y()/5);
        emit erasePoint(point);
    }
}

void PaintArea::imageChanged(MST *mst)
{
    image.fill(backColor.rgb());
    if (askPaintLine)
    {
        int lineSize = mst->getDrawFrom().size();
        for (int i=0; i<lineSize; i++)
        {
            paintLine(image, mst->getDrawFrom()[i], mst->getDrawTo()[i]);
        }
    }
    int pointSize = mst->getPointSize();
    for (int i=0; i<pointSize; i++)
    {
        paintPoint(image, mst->getPoints()[i]);
    }
    update();
}

QColor PaintArea::getGray(QColor color)
{
    return QColor(color.green(),color.blue(),color.red(),128);
}

void PaintArea::paintPoint(QImage &theImage, QPoint point, bool moving)
{
    QPainter pt(&theImage);
    QPen pen;
    if (moving)
        pen.setColor(getGray(pointColor));
    else
        pen.setColor(pointColor);
    pen.setWidth(5);
    pt.setPen(pen);
    QPoint dp(point.x()*5+2, point.y()*5+2);
    pt.drawPoint(dp);
    update();
}

void PaintArea::paintLine(QImage &theImage, QPoint from, QPoint to)
{
    if (from!=to)
    {
        QPainter pt(&theImage);
        pt.setPen(lineColor);
        QPoint df(from.x()*5+2, from.y()*5+2),
                dt(to.x()*5+2, to.y()*5+2);
        pt.drawLine(df, dt);
        update();
    }
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage theImage = image;
    if (theImage.save(fileName, fileFormat)) //实现了文件存储
    {
        return true;
    }
    else
    {
        return false;
    }
}


void PaintArea::doPrint()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}

void PaintArea::setBackColor(QColor color)
{
    backColor = color;
}

void PaintArea::setPointColor(QColor color)
{
    pointColor = color;
}

void PaintArea::setLineColor(QColor color)
{
    lineColor = color;
}

void PaintArea::zoomIn()
{
    scale *= 1.2;
    update();
}

void PaintArea::zoomOut()
{
    scale /=1.2;
    update();
}

void PaintArea::zoom1()
{
    scale = 1;
    update();
}

void PaintArea::clear()
{
    image.fill(backColor); //用现在的画布背景色进行填充
    update();
}

QSize PaintArea::getImageSize()
{
    QSize size(width, height);
    return size*scale;
}

void PaintArea::pointMoving(MST *mst)
{
    imageChanged(mst);
    paintPoint(image,movingto,true);
}
