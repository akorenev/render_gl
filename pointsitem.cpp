#include "pointsitem.h"

pointsitem::pointsitem()
{
    addVertex(Vertex(0.0f, 0.5f, 0.0f,
                     1.0, 0.0, 0.0, 1.0));
    addVertex(Vertex(-0.5f, -0.5f, 0.0f,
                     1.0, 0.0, 0.0, 1.0));
    addVertex(Vertex(0.5f, -0.5f, 0.0f,
                     0.0, 1.0, 0.0, 1.0));
}

pointsitem::~pointsitem()
{
    m_vertexes.clear();
    m_indices.clear();
}

void pointsitem::addVertex(const Vertex &vertex)
{
    m_vertexes.push_back(vertex);
    m_indices.push_back(m_vertexes.size() - 1);
}

Item::Type pointsitem::getType() const
{
    return POINTS;
}
