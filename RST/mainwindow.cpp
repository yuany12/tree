#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "donewdialog.h"
#include "data.h"
#include "paintarea.h"
#include "mst.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QString>
#include <QScrollArea>
#include <QPoint>
#include <QCloseEvent>

#include <fstream>
#include <vector>

#include <QtDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    saved(false),
    modified(false),
    curInputFile(tr("untitled.in")),
    curPicFile(tr("untitle.png"))
{
    ui->setupUi(this);
    setWindowTitle(tr("yy-RST"));
    QIcon icon(":/new/prefix1/icons/wi.png");
    setWindowIcon(icon);
    resize(800,600);
    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area);
    scrollArea->widget()->setMinimumSize(20000,15000);
    setCentralWidget(scrollArea);

    connect(area,SIGNAL(addPoint(QPoint)),
            this,SLOT(addPoint(QPoint)));
    connect(area,SIGNAL(erasePoint(QPoint)),
            this,SLOT(erasePoint(QPoint)));
    connect(area,SIGNAL(movePoint(QPoint,QPoint)),
            this,SLOT(movePoint(QPoint,QPoint)));
    connect(area,SIGNAL(movingPoint(QPoint,QPoint)),
            this,SLOT(movingPoint(QPoint,QPoint)));
    connect(area,SIGNAL(onPoint(QPoint)),
            this,SLOT(onPoint(QPoint)));
    data = new Data(80,60,0);
    connect(data,SIGNAL(dataResultChanged(MST*)),
            area,SLOT(imageChanged(MST*)));
    connect(data,SIGNAL(clear()),
            area,SLOT(clear()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::maybeSaved()
{
    if(data->isModified())
    {
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this,tr("Save the Input"),tr("The input file has been changed. "
                                                                "Do you want to save it?"),
        QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(box == QMessageBox::Yes)
        {
            return doInputFileSave();
        }
        else if(box == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::doNew()
{
    if (maybeSaved())
    {
        doNewDialog ndl;
        if (ndl.exec()==QDialog::Accepted)
        {
            int max_x = ndl.getWidth();
            int max_y = ndl.getHeight();
            area->setImageSize(max_x*5,max_y*5);
            if (ndl.getBackgroundColor().isValid())
                area->setBackColor(ndl.getBackgroundColor());
            else area->setBackColor(qRgb(255,255,255));
            if (ndl.getPointColor().isValid())
                area->setPointColor(ndl.getPointColor());
            else area->setPointColor(qRgb(0,0,0));
            if (ndl.getPointColor().isValid())
                area->setLineColor(ndl.getLineColor());
            else area->setLineColor(qRgb(0,0,0));
            scrollArea->widget()->resize(max_x*5,max_y*5);
            saved = false;
            if (data!=NULL)
                delete data;
            data = new Data(max_x, max_y, ndl.getPointNumber(), ndl.getAlgorithm());
            connect(data,SIGNAL(dataResultChanged(MST*)),
                    area,SLOT(imageChanged(MST*)));
            connect(data,SIGNAL(clear()),
                    area,SLOT(clear()));
            area->imageChanged(data->getMst());
        }
    }
}

void MainWindow::doOpen()
{
    if (maybeSaved())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open input file"),
                                                        QDir::currentPath(),"Input Files(*.in)");
        doNewDialog ndl;
        connect(this,SIGNAL(OpenDialog()),
                &ndl,SLOT(WhenOpen()));
        emit OpenDialog();
        if (!fileName.isEmpty() && ndl.exec()==QDialog::Accepted)
        {
            saved = true;
            curInputFile = fileName;
            if (ndl.getBackgroundColor().isValid())
                area->setBackColor(ndl.getBackgroundColor());
            else area->setBackColor(qRgb(255,255,255));
            if (ndl.getPointColor().isValid())
                area->setPointColor(ndl.getPointColor());
            else area->setPointColor(qRgb(0,0,0));
            if (ndl.getPointColor().isValid())
                area->setLineColor(ndl.getLineColor());
            else area->setLineColor(qRgb(0,0,0));
            if (data!=NULL)
                delete data;
            QByteArray ba = fileName.toLocal8Bit();
            data = new Data(ba.data(), ndl.getAlgorithm());
            connect(data,SIGNAL(dataResultChanged(MST*)),
                    area,SLOT(imageChanged(MST*)));
            connect(data,SIGNAL(clear()),
                    area,SLOT(clear()));
            area->setImageSize(data->getMaxX()*5, data->getMaxY()*5);
            scrollArea->widget()->resize(data->getMaxX()*5, data->getMaxY()*5);
            area->imageChanged(data->getMst());
        }
    }
}

bool MainWindow::saveInputFile(QString fileName)
{
    if (fileName.isEmpty())
        return false;
    ui->statusBar->showMessage("saving", 800);
    QByteArray ba = fileName.toLocal8Bit();
    std::ofstream fout(ba.data());
    fout << data->getMaxX() << ' ' << data->getMaxY() << std::endl;
    for (int i=0;i<data->getPointsNumber();i++)
    {
        fout << data->getMst()->getPoints()[i].x() << " " << data->getMst()->getPoints()[i].y() << std::endl;
    }
    return true;
}

bool MainWindow::doInputFileSave()
{
    if (saved)
    {
        return saveInputFile(curInputFile);
    }
    else return doInputFileSaveAs();
}

bool MainWindow::doInputFileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save the input file as"),curInputFile);
    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        saved = true;
        return saveInputFile(fileName);
    }
}

bool MainWindow::doExport()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Export the result as "),curPicFile);
    if(fileName.isEmpty())
    {
        return false;
    }
    if(area->saveImage(fileName,"png"))
    {
        ui->statusBar->showMessage("exporting", 800);
        return true;
    }
    else return false;
}

bool MainWindow::erasePoint(QPoint point)
{
    QString s1,s2;
    s1.setNum(point.x());
    s2.setNum(point.y());

    if (data->erase(point))
    {
        ui->statusBar->showMessage("erasing the point ("
                                   + s1 + "," + s2 +
                                   ")",800);
        return true;
    }
    else return false;
}

bool MainWindow::addPoint(QPoint point)
{
    QString s1,s2;
    s1.setNum(point.x());
    s2.setNum(point.y());
    if (data->add(point))
    {
        ui->statusBar->showMessage("adding the point ("
                                   + s1 + "," + s2 +
                                   ")",800);
        return true;
    }
    else return false;
}

bool MainWindow::movePoint(QPoint from, QPoint to)
{
    QString s1,s2,s3,s4;
    s1.setNum(from.x());
    s2.setNum(from.y());
    s3.setNum(to.x());
    s4.setNum(to.y());

    if (data->move(from, to))
    {
        ui->statusBar->showMessage("moving the point from ("
                                   + s1 + "," + s2 + ") to (" +
                                   s3 + "," + s4 +
                                   ")",800);
        return true;
    }
    else return false;
}

bool MainWindow::movingPoint(QPoint from, QPoint to)
{
    QString s1,s2,s3,s4;
    s1.setNum(from.x());
    s2.setNum(from.y());
    s3.setNum(to.x());
    s4.setNum(to.y());
    if (data->inPointSet(from)&&!data->inPointSet(to))
    {

        ui->statusBar->showMessage("moving the point from ("
                                   + s1 + "," + s2 + ") to (" +
                                   s3 + "," + s4 +
                                   ")",800);
        area->pointMoving(data->getMst());
        return true;
    }
    else
    {
        ui->statusBar->showMessage("(" + s3 + "," + s4 +")",800);
        return false;
    }
}

void MainWindow::onPoint(QPoint point)
{
    QString s1,s2;
    s1.setNum(point.x());
    s2.setNum(point.y());
    ui->statusBar->showMessage("(" + s1 + "," + s2 + ")" ,800);
}

void MainWindow::on_actionNew_triggered()
{
    doNew();
}

void MainWindow::on_actionOpen_triggered()
{
    doOpen();
}

void MainWindow::on_actionSaveInput_triggered()
{
    doInputFileSave();
}

void MainWindow::on_actionSaveInputAs_triggered()
{
    doInputFileSaveAs();
}

void MainWindow::on_actionExport_triggered()
{
    doExport();
}

void MainWindow::closeEvent(QCloseEvent *event) //关闭事件
{
    if(maybeSaved())
        qApp->quit();
    else event->ignore();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionPrint_triggered()
{
    area->doPrint();
}

void MainWindow::on_actionZoom_In_triggered()
{
    area->zoomIn();
    scrollArea->widget()->resize(area->getImageSize());
}

void MainWindow::on_actionZoom_Out_triggered()
{
    area->zoomOut();
    scrollArea->widget()->resize(area->getImageSize());
}


void MainWindow::on_actionInitial_Zoom_triggered()
{
    area->zoom1();
    scrollArea->widget()->resize(area->getImageSize());
}

void MainWindow::on_actionZRMT_triggered()
{
    if (data->getType()!='Z')
        data->setMstType('Z');
    modified = true;
}

void MainWindow::on_actionLRMT_triggered()
{
    if (data->getType()!='L')
        data->setMstType('L');
    modified = true;
}

void MainWindow::on_actionBackground_triggered()
{
    QColor newColor = QColorDialog::getColor(); //从颜色对话框获得颜色
    if (newColor.isValid())
    {
        area->setBackColor(newColor);
        area->imageChanged(data->getMst());
    }
}


void MainWindow::on_actionPoints_triggered()
{
    QColor newColor = QColorDialog::getColor(); //从颜色对话框获得颜色
    if (newColor.isValid())
    {
        area->setPointColor(newColor);
        area->imageChanged(data->getMst());
    }
}


void MainWindow::on_actionLines_triggered()
{
    QColor newColor = QColorDialog::getColor(); //从颜色对话框获得颜色
    if (newColor.isValid())
    {
        area->setLineColor(newColor);
        area->imageChanged(data->getMst());
    }
}

void MainWindow::on_actionClear_C_triggered()
{
    int x=data->getMaxX(),y=data->getMaxY();
    char t=data->getType();
    delete data;
    data = new Data(x,y,0,t);
    connect(data,SIGNAL(dataResultChanged(MST*)),
            area,SLOT(imageChanged(MST*)));
    connect(data,SIGNAL(clear()),
            area,SLOT(clear()));
    area->clear();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::StandardButton box;
    box = QMessageBox::information(this,tr("About yy-RST"),tr("Copyright (c) Yuan YUAN.""\nAll rights reserved."));
}
