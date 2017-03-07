#include "widget.h"
#include "render.h"
#include "filter.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_render(new draw::render)
{}

Widget::~Widget()
{
    delete m_render;
}

void Widget::addModel(Model * model)
{
    m_render->addModel(model);
}

void Widget::removeModel(Model * model)
{
    m_render->removeModel(model);
}

void Widget::initializeGL()
{
    m_render->init();
    connect(m_render, SIGNAL(update()), this, SLOT(update()));
    installEventFilter(new Filter(m_render));
}

void Widget::paintGL()
{
    m_render->draw();
}

void Widget::resizeGL(int w, int h)
{
    m_render->resize(w, h);
}
