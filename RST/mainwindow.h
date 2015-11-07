#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QString;
class QScrollArea;
class QPoint;
class Data;
class PaintArea;
class QCloseEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void doNew();
    void doOpen();
    bool doInputFileSave();
    bool doInputFileSaveAs();
    bool doExport();
private slots:
    bool addPoint(QPoint);
    bool erasePoint(QPoint);
    bool movePoint(QPoint, QPoint);
    bool movingPoint(QPoint,QPoint);
    void onPoint(QPoint);
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSaveInput_triggered();
    void on_actionSaveInputAs_triggered();
    void on_actionExport_triggered();
    void on_actionExit_triggered();
    void on_actionPrint_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionInitial_Zoom_triggered();
    void on_actionZRMT_triggered();
    void on_actionLRMT_triggered();
    void on_actionBackground_triggered();
    void on_actionPoints_triggered();
    void on_actionLines_triggered();
    void on_actionClear_C_triggered();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    bool isSaved;
    Data *data;
    PaintArea *area;
    QScrollArea *scrollArea;
    bool saved, modified;
    QString curInputFile, curPicFile;
    bool maybeSaved();
    bool saveInputFile(QString fileName);
protected:
    void closeEvent(QCloseEvent *);
signals:
    void OpenDialog();
};

#endif // MAINWINDOW_H
