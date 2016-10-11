#ifndef VERTEX_H
#define VERTEX_H

#include "color.h"
#include "point.h"

struct Vertex
{
    Vertex(){}
    Vertex(const PointF & _point,
           const ColorF & _color)
        : point(_point)
        , color(_color) {}
    PointF point;
    ColorF color;
};

#endif // VERTEX_H
