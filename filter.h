#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include "render.h"

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(draw::render * render,
                    QObject *parent = 0);
protected:
    bool eventFilter(QObject * obj, QEvent * e);
private:
    draw::render * m_render;
    bool m_pressLeftButton;
    int m_x;
    int m_y;
    PointD m_pos;
    double m_totalScaleFactor;
    double m_zoomStep;
};

#endif // FILTER_H
