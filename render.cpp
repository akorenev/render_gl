#include "render.h"
#include <QDebug>

#include <GL/glu.h>
#include <GL/gl.h>

#include "plypainter.h"
#include "painter.h"

namespace draw
{

render::render()
    : m_width(0)
    , m_height(0)
    , m_zoom(1.0)
    , m_zoomStep(1.15)
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
    mProjection.perspective(45, m_ratio, 1, 10000);

    QMatrix4x4 mModelViewMatrix;
    mModelViewMatrix.setToIdentity();
    mModelViewMatrix.translate(m_currentPos.x(), m_currentPos.y(), m_boundingBox.min.z());
    mModelViewMatrix.scale(m_zoom);
    mModelViewMatrix.rotate(m_rotate_x, 1.0, 0.0, 0.0);
    mModelViewMatrix.rotate(m_rotate_y, 0.0, 1.0, 0.0);
    mModelViewMatrix.rotate(m_rotate_z, 0.0, 0.0, 1.0);

    mProjection *= mModelViewMatrix;
    m_painterInfo->pushMatrix(mProjection);
    m_IFunctions.glUseProgram(m_shader->getProgramId());
    const ShaderInfoV & shaderInfoV = m_shader->getShaderInfo();
    int matrixId = shaderInfoV[0]->getKeyAttribute("matrix");
    if(matrixId != -1)
        m_IFunctions.glUniformMatrix4fv(matrixId, 1, 0, mProjection.data());

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

    ShaderInfoV shaderInfoV;
    ShaderInfo::Attributes attributes;
    attributes["matrix"] = 0;

    const char *  value =
            "varying vec4 vColor;\n"
            "uniform mat4 matrix;\n"
            "void main(){\n"
            "gl_Position = matrix * gl_Vertex;\n"
            "vColor = gl_Color;\n"
            "}\n";
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::VERTEX, attributes)));

    value = "varying vec4 vColor;\n"
            "void main(){\n"
            "gl_FragColor = vColor;\n"
            "}\n";
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::FRAGMENT)));


    m_shader = Shader::Ptr(new Shader(&m_IFunctions, shaderInfoV));

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

void render::updateZoom(int delta)
{
    if(delta > 0)
        m_zoom *= m_zoomStep;
    else
        m_zoom /= m_zoomStep;

    draw();
    emit update();
}

void render::updatePosition(int x, int y, int _x, int _y)
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(m_currentPos.x(), m_currentPos.y(), m_boundingBox.min.z());
    m.scale(m_zoom, m_zoom, m_zoom);
    m.rotate(m_rotate_x, 1.0, 0.0, 0.0);
    m.rotate(m_rotate_y, 0.0, 1.0, 0.0);
    m.rotate(m_rotate_z, 0.0, 0.0, 1.0);

    QGenericMatrix<4, 4, double> m_;
    for(int i = 0; i < 16 ; ++i)
        m_.data()[i] = m.data()[i];

    QMatrix4x4 m1;
    m1.setToIdentity();
    m1.perspective(45, m_ratio, 1, 10000);

    QGenericMatrix<4, 4, double> m1_;
    for(int i = 0; i < 16 ; ++i)
        m1_.data()[i] = m1.data()[i];

    if(m_painterL.empty())
        return;

    const GLint viewport[4] = {0, 0, m_width, m_height};

    if(!m_item_t)
    {
        double x4, y4, z4;
        gluUnProject(_x, m_height - _y, -1, m_.data(), m1_.data(), viewport, &x4, &y4, &z4);
        QVector3D origin(x4, y4, z4);
        double x3, y3, z3;
        gluUnProject(_x, m_height - _y, 1, m_.data(), m1_.data(), viewport, &x3, &y3, &z3);
        x3 -=x4;
        y3 -=y4;
        z3 -=z4;
        QVector3D direction(x3, y3, z3);
        direction.normalize();

        Item_tL item_tL;
        RayPoint ray(origin, direction);
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

        PointItem * pointItem = (PointItem*)m_item_t->item;
        unsigned char color[4] = {0, 255, 0, 255};
        pointItem->setFill(color);
        qDebug() << item_tL.size();
    }


    //Item * item = item_t->item;
    //qDebug() << item->getType();

    //double xx = item->getBoundingBox().min.x() / 2 + item->getBoundingBox().max.x() / 2;
    //double yy = item->getBoundingBox().min.y() / 2 + item->getBoundingBox().max.y() / 2;
    //double zz = item->getBoundingBox().min.z() / 2 + item->getBoundingBox().max.z() / 2;
    const QVector3D & v = m_item_t->v;

    double x1;
    double y1;
    double z1;
    gluProject(v[0], v[1], v[2], m_.data(), m1_.data(), viewport, &x1, &y1, &z1);

    double zz = z1;

    gluUnProject(x, m_height-y, zz, m_.data(), m1_.data(), viewport, &x1, &y1, &z1);

    double x2;
    double y2;
    double z2;
    gluUnProject(_x, m_height-_y, zz, m_.data(), m1_.data(), viewport, &x2, &y2, &z2);

    m_currentPos[0] += m_zoom *(x1 - x2);
    m_currentPos[1] += m_zoom *(y1 - y2);
    m_currentPos[2] += m_zoom *(z1 - z2);

    //qDebug() << x1 - x2 << "  " <<  z1 - z2;
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
}

void render::setBackground(const QColor &color)
{
    m_background = color;
    draw();
    emit update();
}

void render::setRotateX(const double &x)
{
    m_rotate_x = x;
    draw();
    emit update();
}

void render::setRotateY(const double &y)
{
    m_rotate_y = y;
    draw();
    emit update();
}

void render::setRotateZ(const double &z)
{
    m_rotate_z = z;
    draw();
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
    unsigned char color[4] = {255, 255, 255, 255};
    PointItem * pointItem = (PointItem*)m_item_t->item;
    pointItem->setFill(color);
    m_item_t = 0;

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
