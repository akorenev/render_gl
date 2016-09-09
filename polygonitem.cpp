#include "polygonitem.h"

PolygonItem::PolygonItem()
{
    std::fill(m_fill, m_fill + 4, 0);
}

PolygonItem::~PolygonItem(){}

Item::Type PolygonItem::getType() const
{
    return POLYGON;
}

void PolygonItem::setPoints(const PointDV &pointDV)
{
    if(pointDV.empty())
        return;
    m_boundingBox.min = pointDV[0];
    m_boundingBox.max = pointDV[0];
    for(auto & v : pointDV)
    {
        BoundingBox::calculate(m_boundingBox,v);
        m_pointDV.push_back(v);
    }
}

const PointDV &PolygonItem::getPoints()
{
    return m_pointDV;
}

const BoundingBox &PolygonItem::getBoundingBox() const
{
    return  m_boundingBox;
}

void PolygonItem::setFill(const float * fill)
{
    std::copy(fill, fill + 4, m_fill);
}

const float * PolygonItem::getFill() const
{
    return &m_fill[0];
}

