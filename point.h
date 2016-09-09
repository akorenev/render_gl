#ifndef POINT_H
#define POINT_H
#include <vector>
#include <algorithm>

template<typename T>
struct Point
{
    Point(){ clear();}
    Point(const T & x, const T & y, const T & z = 0)
    {
        point[0] = x;
        point[1] = y;
        point[2] = z;
    }

    const T & x() const { return point[0]; }
    const T & y() const { return point[1]; }
    const T & z() const { return point[2]; }

    T & operator[](int index)
    {
        return point[index];
    }

    bool operator == (const Point<T> & other) const
    {
        if(&other == &(*this))
            return true;
        return ((other.x() == x()) &
                (other.y() == y()) &
                (other.z() == z()));
    }

    bool operator !=(const Point<T> & other)
    {
        return !(*this == other);
    }

    Point<T> & operator =(const Point<T> & other)
    {
        if(&other == &(*this))
            return *this;
        point[0] = other.x();
        point[1] = other.y();
        point[2] = other.z();
        return *this;
    }

    void clear()
    {
        std::fill(point, point + sizeof(point) / sizeof(T), 0);
    }
private:
    T point[3];
};

typedef Point<double> PointD;
typedef Point<float> PointF;
typedef std::vector<PointD> PointDV;

#endif // POINT_H
