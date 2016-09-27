#ifndef RENDER_H
#define RENDER_H

#include <QMatrix4x4>
#include <QColor>
#include "ipainter.h"
#include "ifunctions.h"
#include "shader.h"

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
    void setZoom(const double & zoom);
    const double & getZoom() const;

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
    GLuint loadShader(const char * value, GLenum type);
signals:
    void update();
private:
    IFunctions m_IFunctions;
    int m_width;
    int m_height;
    double m_zoom;
    double m_ratio;
    BoundingBox m_boundingBox;
    QColor m_background;
    PointD m_currentPos;
    double m_rotate_y;
    double m_rotate_x;
    double m_rotate_z;
    Item_t::Ptr m_item_t;
    IPainterL m_painterL;
    IPainterInfo::Ptr m_painterInfo;
    Shader::Ptr m_shader;
};

}

#endif // RENDER_H
