#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include "item.h"

class PolygonItem : public Item
{
public:
    PolygonItem();
    ~PolygonItem();
    Type getType() const;
    void setPoints(const PointDV & pointDV);
    const PointDV & getPoints();
    const BoundingBox& getBoundingBox() const;
    void setFill(const float * fill);
    const float * getFill() const;
private:
    PointDV m_pointDV;
    float m_fill[4];
};

#endif // POLYGONITEM_H
