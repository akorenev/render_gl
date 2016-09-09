#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "point.h"

class BoundingBox
{
public:
    BoundingBox(){}
    BoundingBox(const PointD &_min, const PointD & _max)
        : min(_min)
        , max(_max){}
    static void calculate(BoundingBox & result, const BoundingBox & input)
    {
        result.min[0] = std::min(result.min.x(), input.min.x());
        result.min[1] = std::min(result.min.y(), input.min.y());
        result.min[2] = std::min(result.min.z(), input.min.z());

        result.max[0] = std::max(result.max.x(), input.max.x());
        result.max[1] = std::max(result.max.y(), input.max.y());
        result.max[2] = std::max(result.max.z(), input.max.z());
    }
    template<typename T>
    static void calculate(BoundingBox &result, const Point<T> & p)
    {
        result.min[0] = std::min(result.min.x(), p.x());
        result.min[1] = std::min(result.min.y(), p.y());
        result.min[2] = std::min(result.min.z(), p.z());

        result.max[0] = std::max(result.max.x(), p.x());
        result.max[1] = std::max(result.max.y(), p.y());
        result.max[2] = std::max(result.max.z(), p.z());
    }
    static PointD getMiddlePoint(const BoundingBox & bbox)
    {
        PointD middle;
        middle[0] = bbox.min.x() / 2 + bbox.max.x() / 2;
        middle[1] = bbox.min.y() / 2 + bbox.max.y() / 2;
        middle[2] = bbox.min.z() / 2 + bbox.max.z() / 2;
        return middle;
    }

    static bool intersected(const BoundingBox & first, const BoundingBox & second)
    {
        if(!intersect(first.min.x(), first.max.x(), second.min.x(), second.max.x()))
            return false;
        if(!intersect(first.min.y(), first.max.y(), second.min.y(), second.max.y()))
            return false;
        if(!intersect(first.min.z(), first.max.z(), second.min.z(), second.max.z()))
            return false;
        return true;
    }

    PointD min; // min x y z
    PointD max; // max x y z

private:
    static bool intersect(double min1, double max1, double min2, double max2)
    {
        if(min1 > max2)
            return false;
        if(max1 < min2)
            return false;
        return true;
    }
};

#endif // BOUNDINGBOX_H
