#ifndef POINTSITEM_H
#define POINTSITEM_H

#include <vector>
#include "item.h"


struct Vertex
{
    Vertex(){}
    Vertex(float x, float y, float z,
           float r, float g, float b, float a)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;

        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;

    }
    float position[3];
    float color[4];
};


class pointsitem : public Item
{
public:
    pointsitem();
    ~pointsitem();

    void addVertex(const Vertex & vertex);
    Type getType() const;

    std::vector<Vertex> m_vertexes;
    std::vector<unsigned short> m_indices;
};

#endif // POINTSITEM_H
