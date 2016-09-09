#ifndef COLOR_H
#define COLOR_H

template<typename T>
struct Color
{
public:
    Color()
    {
        clear();
    }
    Color(const T & r, const T & g, const T & b, const T & a = 0)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }
    T & operator[](int index)
    {
        return color[index];
    }
    void clear()
    {
        std::fill(color, color + sizeof(color) / sizeof(T), 0);
    }

    bool operator == (const Color<T> & other) const
    {
        if(&other == &(*this))
            return true;
        return ((other.r() == r()) &
                (other.g() == g()) &
                (other.b() == b()) &
                (other.a() == a()));
    }

    bool operator !=(const Point<T> & other)
    {
        return !(*this == other);
    }
    const T & r() const { return color[0]; }
    const T & g() const { return color[1]; }
    const T & b() const { return color[2]; }
    const T & a() const { return color[3]; }
private:
    T color[4];
};

typedef Color<double> ColorD;
typedef Color<float> ColorF;

#endif // COLOR_H
