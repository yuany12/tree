#include "data.h"
#include "mst.h"

#include <QPoint>

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

#include <QtDebug>


Data::Data(char* inputFileName, char mst_type) :
    mst(new MST(mst_type)),modified(false)
{
    pointSet.clear();
    std::ifstream fin(inputFileName);
    fin >> max_x >> max_y;
    int x,y;
    while (fin >> x >> y)
    {
        QPoint point(x,y);
        pointSet.push_back(point);
    }
    fin.close();
    rewriteInput();
    cal();
}

Data::Data(int x, int y, int pointsNumber, char mst_type) :
    mst(new MST(mst_type)),modified(false)
{
    if (pointsNumber>0)
        modified = true;
    pointSet.clear();
    max_x = x;
    max_y = y;
    std::ofstream fout("input.tmp");
    srand((unsigned)time(NULL));
    while (pointsNumber--)
    {
        int qx = rand()%(x);
        int qy = rand()%(y);
        fout << qx << ' ' << qy << std::endl;
        QPoint point(qx,qy);
        pointSet.push_back(point);
    }
    fout.close();
    cal();
}

Data::~Data()
{
    delete mst;
}

void Data::rewriteInput()
{
    std::ofstream fout("input.tmp");
    for (std::vector<QPoint>::iterator it = pointSet.begin(); it != pointSet.end() ; it ++ )
    {
        fout << (*it).x() << ' ' << (*it).y() << std::endl;
    }
    fout.close();
}

int Data::getMaxX()
{
    return max_x;
}

int Data::getMaxY()
{
    return max_y;
}

int Data::getPointsNumber()
{
    return pointSet.size();
}

std::vector<QPoint>::iterator Data::find(QPoint point)
{
    for ( std::vector<QPoint>::iterator it=pointSet.begin(); it!=pointSet.end(); it++ )
        if (it->x()==point.x() && it->y()==point.y())
        {
            return it;
        }
    return pointSet.end();
}

bool Data::inPointSet(QPoint point)
{
    for ( std::vector<QPoint>::iterator it=pointSet.begin(); it!=pointSet.end(); it++ )
        if (it->x()==point.x() && it->y()==point.y())
        {
            return true;
        }
    return false;
}

bool Data::erase(QPoint point)
{
    std::vector<QPoint>::iterator it=find(point);
    if (it!=pointSet.end())
    {
        qDebug() << "erase" << point;
        pointSet.erase(it);
        rewriteInput();
        cal();
        return true;
    }
    else return false;
}

bool Data::add(QPoint point)
{
    std::vector<QPoint>::iterator it=find(point);
    if ( it==pointSet.end() &&
        point.x()>=0 && point.y()>=0 &&
        point.x()<max_x && point.y()<max_y )
    {
        modified =true;
        pointSet.push_back(point);
        rewriteInput();
        cal();
        return true;
    }
    else return false;
}

bool Data::move(QPoint from, QPoint to)
{
    std::vector<QPoint>::iterator it=find(from);
    if (it!=pointSet.end() && to.x()>=0 && to.y()>=0 &&
            to.x()<max_x && to.y()<max_y)
    {
        modified =true;
        pointSet.erase(it);
        pointSet.push_back(to);
        rewriteInput();
        cal();
        return true;
    }
    else return false;
}

void Data::cal()
{
    if (pointSet.size()>0)
    {
        mst->work();
        emit dataResultChanged(mst);
    }
    else
    {
        emit clear();
    }
}

void Data::setMstType(char type)
{
    mst->setType(type);
    cal();
}

void Data::saveInputData(char* outputFileName)
{
    modified = false;
    std::ofstream fout(outputFileName);
    fout << max_x << ' ' << max_y << std::endl;
    for (std::vector<QPoint>::iterator it=pointSet.begin(); it!=pointSet.end() ;it++ )
    {
        fout << it->x() << ' ' << it->y() << std::endl;
    }
    fout.close();
}

MST* Data::getMst()
{
    return mst;
}

char Data::getType()
{
    return mst->getType();
}

bool Data::isModified()
{
    return modified;
}
