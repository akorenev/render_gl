#ifndef RENDER_H
#define RENDER_H

#include <QMatrix4x4>
#include <QColor>
#include "ipainter.h"
#include "ifunctions.h"


namespace draw
{

class render : public QObject
{
    Q_OBJECT
public:
    render();
    ~render();
    void draw();
    void init();
    void resize(int w, int h);
    void updateZoom(int delta);
    void updatePosition(int x, int y, int _x, int _y);
    const PointD & getPos() const;
    void addModel(Model * model);
    void removeModel(Model * model);
    void setBackground(const QColor &color);
    void setRotateX(const double & x);
    void setRotateY(const double & y);
    void setRotateZ(const double & z);
    const double & getRotateX();
    const double & getRotateY();
    const double & getRotateZ();
    void releaseMouse();
private:
    void calculateBoundingBox();
signals:
    void update();
private:
    IFunctions m_IFunctions;
    int m_width;
    int m_height;
    double m_zoom;
    double m_zoomStep;
    double m_ratio;
    BoundingBox m_boundingBox;
    QColor m_background;
    PointD m_currentPos;
    double m_rotate_y;
    double m_rotate_x;
    double m_rotate_z;
    Item_t::Ptr m_item_t;
    IPainterL m_painterL;
};

}

#endif // RENDER_H
