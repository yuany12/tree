#ifndef PAINTAREA_H
#define PAINTAREA_H


#include <QWidget>

class MST;

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0,
                       int Width = 400,
                       int Height = 300,
                       QColor backColor = qRgb(255,255,255),
                       QColor pointColor = qRgb(0,0,0),
                       QColor lineColor = qRgb(0,0,0)
                       );
    void setImageSize(int width=400, int height=300);
    QSize getImageSize();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool saveImage(const QString &fileName, const char *fileFormat); //保存图片
    bool openImage(const QString &fileName); //打开图片
    void doPrint();                         //打印图片
    void setBackColor(QColor);              //设置背景色
    void setPointColor(QColor);             //设置点色
    void setLineColor(QColor);              //设置线色
    void pointMoving(MST *mst);             //为动态表示点挪动需要
private:
    QImage image;       //存放可画区域的图像
    QColor backColor;
    QColor pointColor;
    QColor lineColor;
    QPoint from,to;
    QPoint movingfrom,movingto;
    void paintPoint(QImage&, QPoint, bool moving=false);
    void paintLine(QImage&, QPoint, QPoint);
    qreal scale;                        //缩放比例
    int width,height;
    bool askPaintLine;                  //控制是否画出line
    QColor getGray(QColor color);       //获取灰度色
signals:
    void addPoint(QPoint);
    void erasePoint(QPoint);
    void movePoint(QPoint,QPoint);
    void movingPoint(QPoint,QPoint);
    void onPoint(QPoint);                   //鼠标在该点上
public slots:
    void imageChanged(MST*);
    void zoomIn(); //放大
    void zoomOut(); //缩小
    void zoom1();  //还原大小
    void clear(); //清屏
};

#endif // PAINTAREA_H
