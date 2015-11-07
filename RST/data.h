#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <vector>

class MST;
class QPoint;


class Data : public QObject
{
    Q_OBJECT

public:
    Data(char* inputFileName, char mst_type='L');    //输入生成
    Data(int x=80, int y=60, int pointsNumber=20, char mst_type='L'); //随机生成
    ~Data();
    int getMaxX();
    int getMaxY();
    int getPointsNumber();
    std::vector<QPoint>::iterator Data::find(QPoint point);
    bool inPointSet(QPoint point);
    bool erase(QPoint);
    bool add(QPoint);
    bool move(QPoint from, QPoint to);
    void cal();
    MST* getMst();
    void setMstType(char type);
    char getType();
    bool isModified();
private:
    int max_x, max_y; // x,y坐标的最大值
    std::vector<QPoint> pointSet;
    MST *mst;
    void rewriteInput();
    void saveInputData(char* outputFileName);
    bool modified;
signals:
    void dataResultChanged(MST*);
    void clear();
};

#endif // DATA_H
