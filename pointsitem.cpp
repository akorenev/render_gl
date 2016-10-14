#include "pointsitem.h"

pointsitem::pointsitem()
{
//    addVertex(Vertex(PointF(0.0f, 0.5f, 0.0f),
//                     ColorF(1.0, 0.0, 0.0, 1.0)));
//    addVertex(Vertex(PointF(-0.5f, -0.5f, 0.0f),
//                     ColorF(1.0, 0.0, 0.0, 1.0)));
//    addVertex(Vertex(PointF(0.5f, -0.5f, 0.0f),
//                     ColorF(0.0, 1.0, 0.0, 1.0)));
}

pointsitem::~pointsitem()
{
    m_vertexes.clear();
    m_indices.clear();
}

Item::Type pointsitem::getType() const
{
    return POINTS;
}
