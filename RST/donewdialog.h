#ifndef DONEWDIALOG_H
#define DONEWDIALOG_H

#include <QDialog>

class QColor;

namespace Ui {
class doNewDialog;
}

class doNewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit doNewDialog(QWidget *parent = 0);
    ~doNewDialog();
    int getWidth();
    int getHeight();
    int getPointNumber();
    char getAlgorithm();
    QColor getBackgroundColor();
    QColor getPointColor();
    QColor getLineColor();

private slots:
    void on_radioButton_blank_clicked(bool checked);
    void on_radioButton_random_clicked(bool checked);
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_radioButton_ZMST_clicked();
    void on_radioButton_LMST_clicked();

public slots:
    void WhenOpen();
private:
    QColor backgroundColor, pointColor, lineColor;
    char alg;
    Ui::doNewDialog *ui;
};

#endif // DONEWDIALOG_H
