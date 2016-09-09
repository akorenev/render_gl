#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

namespace draw
{
    class render;
}

class Widget : public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
private:
    draw::render * m_render;
};

#endif // WIDGET_H
