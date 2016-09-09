#include "pointitem.h"

PointItem::PointItem()
{
    std::fill_n(m_fill, 4, 255);
}

Item::Type PointItem::getType() const
{
    return POINT;
}

const PointD &PointItem::getPoint() const
{
    return m_point;
}

void PointItem::setPoint(const PointD &point)
{
    m_point = point;
    double r = 5.0;
    m_boundingBox.min = PointD(m_point.x() - r, m_point.y() - r, m_point.z() - r);
    m_boundingBox.max = PointD(m_point.x() + r, m_point.y() + r, m_point.z() + r);
}

void PointItem::setFill(unsigned char fill[])
{
    std::copy(fill, fill + sizeof(fill) / sizeof(unsigned char), m_fill);
}

const unsigned char *PointItem::getFill() const
{
    return &m_fill[0];
}
