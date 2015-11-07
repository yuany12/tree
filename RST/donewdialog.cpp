#include "donewdialog.h"
#include "ui_donewdialog.h"
#include <QColor>
#include <QColorDialog>

namespace Ui {
class doNewDialog;
}

doNewDialog::doNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::doNewDialog),
    backgroundColor(qRgb(255,255,255)),
    pointColor(qRgb(0,0,0)),
    lineColor(qRgb(0,0,0)),
    alg('L')
{
    ui->setupUi(this);
    QPalette pal1 = ui->textBrowser->palette();
    pal1.setColor(QPalette::Base, backgroundColor);
    ui->textBrowser->setPalette(pal1);

    QPalette pal2 = ui->textBrowser_2->palette();
    pal2.setColor(QPalette::Base, pointColor);
    ui->textBrowser_2->setPalette(pal2);

    QPalette pal3 = ui->textBrowser_3->palette();
    pal3.setColor(QPalette::Base, lineColor);
    ui->textBrowser_3->setPalette(pal3);
}

doNewDialog::~doNewDialog()
{
    delete ui;
}

int doNewDialog::getWidth()
{
    return ui->spinBox_2->text().toInt();
}

int doNewDialog::getHeight()
{
    return ui->spinBox_1->text().toInt();
}

int doNewDialog::getPointNumber()
{
    return ui->spinBox_3->text().toInt();
}

void doNewDialog::on_radioButton_blank_clicked(bool checked)
{
    ui->spinBox_3->setValue(0);
}

void doNewDialog::on_radioButton_random_clicked(bool checked)
{
    ui->spinBox_3->setValue(50);
}

void doNewDialog::on_toolButton_clicked()
{
    QColor newColor = QColorDialog::getColor(); //从颜色对话框获得颜色
    if (newColor.isValid()) //如果得到的是可用的颜色
    {
        backgroundColor = newColor;
        QPalette palette = ui->textBrowser->palette(); //显示这个颜色
        palette.setColor(QPalette::Base,backgroundColor);
        ui->textBrowser->setPalette(palette);
        update();
    }
}

void doNewDialog::on_toolButton_2_clicked()
{
    QColor newColor = QColorDialog::getColor(); //从颜色对话框获得颜色
    if (newColor.isValid()) //如果得到的是可用的颜色
    {
        pointColor = newColor;
        QPalette palette = ui->textBrowser_2->palette(); //显示这个颜色
        palette.setColor(QPalette::Base,pointColor);
        ui->textBrowser_2->setPalette(palette);
        update();
    }
}

void doNewDialog::on_toolButton_3_clicked()
{
    QColor newColor = QColorDialog::getColor(); //从颜色对话框获得颜色
    if (newColor.isValid()) //如果得到的是可用的颜色
    {
        lineColor = newColor;
        QPalette palette = ui->textBrowser_3->palette(); //显示这个颜色
        palette.setColor(QPalette::Base,lineColor);
        ui->textBrowser_3->setPalette(palette);
        update();
    }
}

char doNewDialog::getAlgorithm()
{
    return alg;
}

void doNewDialog::on_radioButton_ZMST_clicked()
{
    alg = 'Z';
}

void doNewDialog::on_radioButton_LMST_clicked()
{
    alg = 'L';
}

QColor doNewDialog::getBackgroundColor()
{
    return backgroundColor;
}

QColor doNewDialog::getPointColor()
{
    return pointColor;
}

QColor doNewDialog::getLineColor()
{
    return lineColor;
}

void doNewDialog::WhenOpen()
{
    ui->groupBox_type->setDisabled(true);
    ui->groupBox_options->setDisabled(true);
}

