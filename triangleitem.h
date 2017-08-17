#ifndef POINTSITEM_H
#define POINTSITEM_H

#include <vector>
#include "item.h"
#include "vertex.h"

class TriangleItem : public Item
{
public:
    Type getType() const;
    std::vector<Vertex> m_vertexes;
    std::vector<unsigned short> m_indices;
};

#endif // POINTSITEM_H
