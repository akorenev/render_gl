#include "ray.h"

Ray::Ray(const QVector3D &origin, const QVector3D &direction)
    : m_origin(origin)
    , m_direction(direction)
{
    m_direction.normalize();
}

Ray::~Ray(){}

const QVector3D &Ray::origin() const
{
    return m_origin;
}

const QVector3D &Ray::direction() const
{
    return m_direction;
}

RayBBox::RayBBox(const QVector3D &origin, const QVector3D &direction)
    : Ray(origin, direction){}

Item_t::Ptr RayBBox::intersected(const PointDV &v) const
{
    int size = v.size();
    if(size == 0 || size > 2)
        return 0;
    PointD min = v[0];
    PointD max = v[1];
    BoundingBox bbox(min, max);

    for(int i = 0; i < 3; ++i)
    {
        if(m_direction[i] == 0)
        {
            bool r = m_origin[i] < min[i] || m_origin[i] > max[i];
            if(r)
            {
                return 0;// discard;
            }
            else
            {
                continue; // next Axes;
            }
        }
        else
        {
            double tmin = (min[i] - m_origin[i]) / m_direction[i];

            if(tmin > 0)
            {
                QVector3D vmin = m_origin + m_direction * tmin;
                vmin[i] = min[i];

                BoundingBox bmin(PointD(vmin.x(), vmin.y(), vmin.z()),
                                 PointD(vmin.x(), vmin.y(), vmin.z()));

                if(BoundingBox::intersected(bbox, bmin))
                    return Item_t::Ptr(new Item_t(0, vmin, tmin));
            }

            double tmax = (max[i] - m_origin[i]) / m_direction[i];

            if(tmax > 0)
            {
                QVector3D vmax = m_origin + m_direction * tmax;

                vmax[i] = max[i];

                BoundingBox bmax(PointD(vmax.x(), vmax.y(), vmax.z()),
                                 PointD(vmax.x(), vmax.y(), vmax.z()));
                if(BoundingBox::intersected(bbox, bmax))
                    return Item_t::Ptr(new Item_t(0, vmax, tmax));
            }
        }
    }
    return 0;
}

Ray::Type RayBBox::getType() const
{
    return BBOX;
}

RayPoint::RayPoint(const QVector3D &origin,
                   const QVector3D &direction)
    : Ray(origin, direction){}

Item_t::Ptr RayPoint::intersected(const PointDV &v) const
{
    int size = v.size();
    if(size == 0 || size > 1)
        return 0;
    const PointD & p = v[0];
    QVector3D vector(p.x(), p.y(), p.z());
    for(int i = 0; i < 3; ++i)
        vector[i] -= m_origin[i];
    vector.normalize();
    double angle = cos(asin(1) / 90 / 5);
    double scalar = 0.0;
    for(int i = 0; i < 3; ++i)
        scalar += vector[i] * m_direction[i];
    return scalar > angle ? Item_t::Ptr(new Item_t(0, vector, angle)) : 0;
}

Ray::Type RayPoint::getType() const
{
    return POINT;
}

Item_t::Ptr RayTriangle::intersected(const PointDV &v) const
{
    return 0;
}

Ray::Type RayTriangle::getType() const
{
    return TRIANGLE;
}
