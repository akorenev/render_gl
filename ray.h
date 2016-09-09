#ifndef RAY_H
#define RAY_H

#include "item_t.h"
#include "boundingbox.h"

class Ray
{
public:
    enum Type
    {
        BBOX,
        POINT,
        TRIANGLE
    };
    Ray(const QVector3D &origin,
        const QVector3D &direction);
    virtual ~Ray();
    virtual Item_t::Ptr intersected(const PointDV & v) const = 0;
    virtual Type getType() const = 0;
    const QVector3D &  origin() const;
    const QVector3D &  direction() const;
protected:
    QVector3D m_origin;
    QVector3D m_direction;
};

class RayBBox : public Ray
{
public:
    RayBBox(const QVector3D &origin,
            const QVector3D &direction);
    Item_t::Ptr intersected(const PointDV & v) const;
    Type getType() const;
};

class RayPoint : public Ray
{
public:
    RayPoint(const QVector3D &origin,
             const QVector3D &direction);
    Item_t::Ptr intersected(const PointDV & v) const;
    Type getType() const;
};

class RayTriangle : public Ray
{
public:
    Item_t::Ptr intersected(const PointDV & v) const;
    Type getType() const;
};

#endif // RAY_H
