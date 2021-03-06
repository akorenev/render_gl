#include "filter.h"
#include <QEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTouchEvent>
#include <QLineF>

Filter::Filter(draw::render *render, QObject *parent)
    : QObject(parent)
    , m_render(render)
    , m_pressLeftButton(false)
    , m_totalScaleFactor(1.0)
    , m_zoomStep(1.15)
{}

bool Filter::eventFilter(QObject *obj, QEvent *e)
{
    Q_UNUSED(obj);
    switch (e->type())
    {
    case QEvent::Wheel:
    {
        QWheelEvent * event = static_cast<QWheelEvent*>(e);
        double zoom = m_render->getZoom();
        if(event->delta() > 0)
            zoom *= m_zoomStep;
        else
            zoom /= m_zoomStep;
        m_render->setZoom(zoom);
    }break;
    case QEvent::MouseMove:
    {
        if(m_pressLeftButton)
        {
            QMouseEvent * event = static_cast<QMouseEvent*>(e);
            double x = event->x();
            double y = event->y();
            m_render->updatePosition(x, y, m_x, m_y);
            m_x = x;
            m_y = y;
        }
    }break;
    case QEvent::MouseButtonPress:
    {
        QMouseEvent * event = static_cast<QMouseEvent*>(e);
        if(event->button() == Qt::LeftButton)
        {
            m_pressLeftButton = true;
            m_x = event->pos().x();
            m_y = event->pos().y();
            m_pos = m_render->getPos();
        }
    }break;
    case QEvent::MouseButtonRelease:
    {
        if(m_pressLeftButton)
        {
            m_pressLeftButton = false;
            m_x = 0;
            m_y = 0;
            m_render->releaseMouse();
        }
    } break;
    case QEvent::KeyPress:
    {
        QKeyEvent * event = static_cast<QKeyEvent*>(e);
        switch (event->key())
        {
        case Qt::Key_Right:
            m_render->setRotateY(m_render->getRotateY() + 1.0);
            break;
        case Qt::Key_Left:
            m_render->setRotateY(m_render->getRotateY() - 1.0);
            break;
        case Qt::Key_Up:
            m_render->setRotateX(m_render->getRotateX() - 1.0);
            break;
        case Qt::Key_Down:
            m_render->setRotateX(m_render->getRotateX() + 1.0);
            break;
        case Qt::Key_F1:
            m_render->setRotateZ(m_render->getRotateZ() + 1.0);
            break;
        case Qt::Key_F2:
            m_render->setRotateZ(m_render->getRotateZ() - 1.0);
            break;
        default:
            break;
        }

    } break;
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
    {
        QTouchEvent * event = static_cast<QTouchEvent*>(e);
        QList<QTouchEvent::TouchPoint> points = event->touchPoints();
        if(points.count() == 2)
        {
            const QTouchEvent::TouchPoint &p1 = points.first();
            const QTouchEvent::TouchPoint &p2 = points.last();
            qreal currentScaleFactor =
                    QLineF(p1.pos(), p2.pos()).length()
                    / QLineF(p1.startPos(), p2.startPos()).length();
            if (event->touchPointStates() & Qt::TouchPointReleased) {
                // if one of the fingers is released, remember the current scale
                // factor so that adding another finger later will continue zooming
                // by adding new scale factor to the existing remembered value.
                m_totalScaleFactor *= currentScaleFactor;
                currentScaleFactor = 1.0;
            }
            double scale = m_totalScaleFactor * currentScaleFactor;
            m_render->setZoom(scale);
        }
    } break;
    default:
        break;
    }
    return QObject::eventFilter(obj, e);
}
