#include "render.h"
#include <iostream>

#include "plypainter.h"
#include "painter.h"

namespace draw
{

render::render()
    : m_width(0)
    , m_height(0)
    , m_zoom(1.0)
    , m_background(QColor(200, 200, 200))
    , m_rotate_y(0.0)
    , m_rotate_x(0.0)
    , m_rotate_z(0.0)
{}

render::~render()
{
    IPainterL::iterator it = m_painterL.begin();
    while(it != m_painterL.end())
    {
        delete *it;
        it = m_painterL.erase(it);
    }
}

void render::draw()
{
    calculateBoundingBox();
    m_IFunctions.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_IFunctions.glClearColor(m_background.redF(),
                              m_background.greenF(),
                              m_background.blueF(),
                              m_background.alphaF());
//    m_IFunctions.glMatrixMode(GL_PROJECTION);

    QMatrix4x4 mProjection;
    mProjection.setToIdentity();
    mProjection.perspective(45, m_ratio, 1, 1000);
//    mProjection.ortho(-2.0, 2.0, -2.0, 2.0, -1, 1);

    QMatrix4x4 mModelViewMatrix;
    mModelViewMatrix.setToIdentity();
    mModelViewMatrix.translate(0, 0, -2);
    mModelViewMatrix.scale(m_zoom);
    mModelViewMatrix.rotate(m_rotate_x, 1.0, 0.0, 0.0);
    mModelViewMatrix.rotate(m_rotate_y, 0.0, 1.0, 0.0);
    mModelViewMatrix.rotate(m_rotate_z, 0.0, 0.0, 1.0);

    mProjection *= mModelViewMatrix;
    m_painterInfo->pushMatrix(mProjection);

//    const ShaderInfoV & shaderInfoV = m_shader->getShaderInfo();
//    int matrixId = shaderInfoV[0]->getKeyAttribute("matrix");
//    if(matrixId != -1)
//        m_IFunctions.glUniformMatrix4fv(matrixId, 1, 0, mProjection.data());

//    m_IFunctions.glLoadMatrixf(mProjection.data());
//    m_IFunctions.glMatrixMode(GL_MODELVIEW);
//    m_IFunctions.glLoadIdentity();
//    m_IFunctions.glTranslated(m_currentPos.x(), m_currentPos.y(), m_boundingBox.min.z());
//    m_IFunctions.glScaled(m_zoom, m_zoom, m_zoom);
//    m_IFunctions.glRotated(m_rotate_x, 1.0, 0.0, 0.0);
//    m_IFunctions.glRotated(m_rotate_y, 0.0, 1.0, 0.0);
//    m_IFunctions.glRotated(m_rotate_z, 0.0, 0.0, 1.0);



    for(auto & v : m_painterL)
        v->draw(m_painterInfo);

    m_IFunctions.glUseProgram(0);
    m_IFunctions.glFlush();
    while(!m_painterInfo->emptyMatrix())
        m_painterInfo->popMatrix();
}

void render::init()
{
    m_IFunctions.initializeOpenGLFunctions();
    m_IFunctions.glClearColor(m_background.redF(),
                              m_background.greenF(),
                              m_background.blueF(),
                              m_background.alphaF());

    m_IFunctions.glEnable(GL_DEPTH_TEST);
    m_painterInfo = IPainterInfo::Ptr(new IPainterInfo());
}

void render::resize(int w, int h)
{
    m_width = w;
    m_height = h;
    m_ratio = w * 1.0 / h;
    if(m_ratio < 1)
        m_ratio = 1 / m_ratio;
    m_IFunctions.glViewport(0, 0, m_width, m_height);
//    m_IFunctions.glMatrixMode(GL_PROJECTION);
//    QMatrix4x4 m;
//    m.setToIdentity();
//    m.perspective(45, m_ratio, 1, 10000);
    //    m_IFunctions.glLoadMatrixf(m.data());
}

void render::setZoom(const double & zoom)
{
    m_zoom = zoom;
    emit update();
}

const double &render::getZoom() const
{
    return m_zoom;
}

void render::updatePosition(int x, int y, int _x, int _y)
{
    if(m_painterL.empty())
        return;
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(m_currentPos.x(), m_currentPos.y(), m_boundingBox.min.z());
    m.scale(m_zoom, m_zoom, m_zoom);
    m.rotate(m_rotate_x, 1.0, 0.0, 0.0);
    m.rotate(m_rotate_y, 0.0, 1.0, 0.0);
    m.rotate(m_rotate_z, 0.0, 0.0, 1.0);

    QMatrix4x4 m1;
    m1.setToIdentity();
    m1.perspective(45, m_ratio, 1, 10000);

    QRect viewport(0, 0, m_width, m_height);
    if(!m_item_t)
    {
        QVector3D vv;
        vv[0] = x;
        vv[1] = m_height - _y;
        vv[2] = -1;
        QVector3D v0 = vv.unproject(m, m1, viewport);
        vv[0] = x;
        vv[1] = m_height - _y;
        vv[2] = 1;
        QVector3D v1 = vv.unproject(m, m1, viewport);
        v1 = v1 - v0;
        v1.normalize();

        Item_tL item_tL;
        RayPoint ray(v0, v1);

        for(auto & v : m_painterL)
        {
            Model * model = v->getModel();
            if(Item_t::Ptr item_t = model->searchFirstItem(ray))
                item_tL.push_back(item_t);

        }
        if(item_tL.empty())
            return;
        item_tL.sort(SortItem_t);
        m_item_t = *item_tL.begin();

//        pointsitem * pointItem = (pointsitem*)m_item_t->item;
//        unsigned char color[4] = {0, 255, 0, 255};
        //pointItem->setFill(color);
//        std::cout << "selected item " << item_tL.size() << std::endl;
    }

    const QVector3D & v = m_item_t->v;

    QVector3D v0;
    v0[0] = v[0];
    v0[1] = v[1];
    v0[2] = v[2];
    QVector3D vv(v0.project(m, m1, viewport));

    QVector3D v1;
    v1[0] = x;
    v1[1] = m_height - y;
    v1[2] = vv[2];
    QVector3D vv1(v1.unproject(m, m1, viewport));

    QVector3D v2;
    v2[0] = _x;
    v2[1] = m_height - _y;
    v2[2] = vv[2];
    QVector3D vv2(v2.unproject(m, m1, viewport));

    m_currentPos[0] += m_zoom *(vv1[0] - vv2[0]);
    m_currentPos[1] += m_zoom *(vv1[1] - vv2[1]);
    m_currentPos[2] += m_zoom *(vv1[2] - vv2[2]);
    emit update();
}

const PointD &render::getPos() const
{
    return m_currentPos;
}

struct KeyMatchPainter
{
    KeyMatchPainter(Model * model)
        : m_model(model){}

    bool operator()(ipainter * _ipainter)
    {
        return _ipainter->getModel() == m_model;
    }
private:
    Model * m_model;
};

void render::addModel(Model * model)
{
    if(!model)
        return;
    IPainterL::iterator it = std::find_if(m_painterL.begin(), m_painterL.end(),
                                          KeyMatchPainter(model));
    if(it == m_painterL.end())
    {
        ipainter * _ipainter = 0;
        switch (model->getType())
        {
        case Model::PLY_MODEL:
            _ipainter = new plypainter();
            break;
        default:
            _ipainter = new painter();
        }
        _ipainter->setModel(model);
        _ipainter->setIFunctions(&m_IFunctions);
        m_painterL.push_back(_ipainter);
    }
    emit update();
}

void render::removeModel(Model *model)
{
    if(!model)
        return;
    IPainterL::iterator it = std::find_if(m_painterL.begin(), m_painterL.end(),
                                          KeyMatchPainter(model));
    if(it != m_painterL.end())
    {
        delete *it;
        m_painterL.erase(it);
    }
    emit update();
}

void render::setBackground(const QColor &color)
{
    m_background = color;
    emit update();
}

void render::setRotateX(const double &x)
{
    m_rotate_x = x;
    emit update();
}

void render::setRotateY(const double &y)
{
    m_rotate_y = y;
    emit update();
}

void render::setRotateZ(const double &z)
{
    m_rotate_z = z;
    emit update();
}

const double &render::getRotateX()
{
    return m_rotate_x;
}

const double &render::getRotateY()
{
    return m_rotate_y;
}

const double &render::getRotateZ()
{
    return m_rotate_z;
}

void render::releaseMouse()
{
    if(!m_item_t)
        return;
//    unsigned char color[4] = {255, 255, 255, 255};
//    pointsitem * pointItem = (pointsitem*)m_item_t->item;
    //if(pointItem)
       //pointItem->setFill(color);
    m_item_t = 0;
    emit update();

}

void render::calculateBoundingBox()
{
    for(auto & v : m_painterL)
    {
        Model * model = v->getModel();
        BoundingBox::calculate(m_boundingBox, model->getBoundingBox());
    }
}

}
