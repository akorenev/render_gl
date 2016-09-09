#ifndef RENDER_H
#define RENDER_H

#include "ifunctions.h"
#include "model.h"
#include "pointitem.h"
#include "polygonitem.h"
#include "imageitem.h"
#include "cubeitem.h"
#include <QMatrix4x4>
#include <QColor>
#include "octotree.h"
#include "plymodel.h"
#include "ipainter.h"


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
    void drawPolygons(const std::vector<PolygonItem*> & polygonItemV);
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
    unsigned int m_programRect;
    unsigned int m_programCube;

    int attrib_vertex;
    int attrib_fragment;
    int attrib_border;
    Item_t::Ptr m_item_t;
    IPainterL m_painterL;
};

}

#endif // RENDER_H
