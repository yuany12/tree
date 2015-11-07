#ifndef MST_H
#define MST_H

#include <vector>

class QPoint;

class MST
{
private:
    char TYPE;//'L' or 'Z'
    std::vector<QPoint> points;
    std::vector<QPoint> drawFrom;
    std::vector<QPoint> drawTo;
    int pointSize, lineSize;
public:
    MST(char type);
    void setType(char type);
    char getType();
    void work();
    int getPointSize();
    int getLineSize();
    std::vector<QPoint>& getPoints();
    std::vector<QPoint>& getDrawFrom();
    std::vector<QPoint>& getDrawTo();
};


#endif // MST_H
