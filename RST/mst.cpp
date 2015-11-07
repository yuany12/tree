#include "mst.h"
#include "algorithm/data_format.h"
#include "algorithm/overlap.h"
#include "algorithm/LMST.h"
#include "algorithm/SMST.h"
#include "algorithm/ZMST.h"
#include "algorithm/PointsGenerator.h"

#include <QPoint>
#include <QtDebug>

#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>



MST::MST(char type) : TYPE(type)
{
}

void MST::setType(char type)
{
    TYPE = type;
}

int MST::getPointSize()
{
    return pointSize;
}

int MST::getLineSize()
{
    return lineSize;
}

std::vector<QPoint>& MST::getDrawFrom()
{
    return drawFrom;
}

std::vector<QPoint>& MST::getDrawTo()
{
    return drawTo;
}

std::vector<QPoint>& MST::getPoints()
{
    return points;
}

void MST::work()
{
    if ( TYPE=='Z' )
    {
        ZMST zmst;
        zmst.setPointsFromFile("input.tmp");
        zmst.exec();
        pointSize = zmst.points().size();
        lineSize = zmst.lines().size();
        points.clear();
        drawFrom.clear();
        drawTo.clear();
        for (std::vector<Point>::const_iterator it = zmst.points().begin();
             it != zmst.points().end(); ++ it)
        {
          QPoint p(it->x(), it->y());
          points.push_back(p);
        }
        for (std::vector<Line_Z>::const_iterator it = zmst.lines().begin();
             it != zmst.lines().end(); ++ it)
        {
          QPoint mid1(it->mid_point().x(),it->mid_point().y());
          if (mid1.y()==points[it->start()].y())
          {
              QPoint mid2(mid1.x(), points[it->end()].y());
              drawFrom.push_back(points[it->start()]);
              drawTo.push_back(mid1);
              drawFrom.push_back(mid1);
              drawTo.push_back(mid2);
              drawFrom.push_back(mid2);
              drawTo.push_back(points[it->end()]);

          }
          else
          {
              QPoint mid2(points[it->end()].x(), mid1.y());
              drawFrom.push_back(points[it->start()]);
              drawTo.push_back(mid1);
              drawFrom.push_back(mid1);
              drawTo.push_back(mid2);
              drawFrom.push_back(mid2);
              drawTo.push_back(points[it->end()]);
          }
        }
    }
    else if ( TYPE=='L' )
    {
        PointsGenerator pg;
        LMST lmst("input.tmp");
        pointSize = lmst.points().size();
        points.clear();
        drawFrom.clear();
        drawTo.clear();
        for (int i = 0; i < pointSize; i++)
        {
            QPoint p(lmst.points()[i].x(), lmst.points()[i].y());
            points.push_back(p);
        }
        lineSize = 0;
        if (pointSize>=2)
        {
            lineSize = lmst.result().size();
            for (int i = 0; i < lmst.result().size(); i++)
            {
                if (lmst.result()[i].direction())
                {
                    QPoint midPoint(points[lmst.result()[i].end()].x(), points[lmst.result()[i].start()].y());
                    drawFrom.push_back(points[lmst.result()[i].start()]);
                    drawTo.push_back(midPoint);
                    drawFrom.push_back(midPoint);
                    drawTo.push_back(points[lmst.result()[i].end()]);
                }
                else
                {
                    QPoint midPoint(points[lmst.result()[i].start()].x(), points[lmst.result()[i].end()].y());
                    drawFrom.push_back(points[lmst.result()[i].start()]);
                    drawTo.push_back(midPoint);
                    drawFrom.push_back(midPoint);
                    drawTo.push_back(points[lmst.result()[i].end()]);
                }
            }
        }
    }
}

char MST::getType()
{
    return TYPE;
}
