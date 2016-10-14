#ifndef POINTSITEM_H
#define POINTSITEM_H

#include <vector>
#include "item.h"
#include "vertex.h"

class pointsitem : public Item
{
public:
    pointsitem();
    ~pointsitem();

    Type getType() const;

    std::vector<Vertex> m_vertexes;
    std::vector<unsigned short> m_indices;
};

#endif // POINTSITEM_H
