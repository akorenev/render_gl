#include "widget.h"
#include "render.h"
#include "filter.h"
#include "model.h"
#include "pointitem.h"
#include "polygonitem.h"
#include "imageitem.h"
#include "cubeitem.h"
#include "octotree.h"
#include "plydriver.h"
#include "plymodel.h"
#include <QApplication>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_render(0){}

Widget::~Widget()
{
    delete m_render;
}

ImageItem * loadImageItem(const QString & path)
{
    QImage i(path);
    unsigned char * bits = 0;
    unsigned int w = 0;
    unsigned int h = 0;
    unsigned int d = 0;
    if(!i.isNull())
    {
        w = i.width();
        h = i.height();
        d = (i.depth() / 8);
        unsigned char * b = i.bits();
        int size = w * h * d;
        bits = new unsigned char[size];
        memset(bits,  0, size);
        for(unsigned int i = 0;  i < w; i++)
        {
            for(unsigned int j = 0; j < h; j++)
            {
                int index = (j * w + i) * d;
                switch (d)
                {
                case 1:
                    bits[index] = b[index];
                    break;
                case 4:
                {
                    bits[index + 0] = b[index + 2];
                    bits[index + 1] = b[index + 1];
                    bits[index + 2] = b[index + 0];
                    bits[index + 3] = b[index + 3];
                } break;
                default:
                    break;
                }
            }
        }
    }
    if(!bits)
        return 0;
    ImageItem * item = new ImageItem(bits, d, w, h);
    return item;
}


void Widget::initializeGL()
{
    if(m_render)
        return;
    m_render = new draw::render();
    m_render->init();
    connect(m_render, SIGNAL(update()), this, SLOT(update()));

    ItemUS itemUS;

    for(int i = 0; i < 2000; ++i)
    {
        PointItem * pointItem = new PointItem();
        PointD p;
        p[0] = -500 + (rand() % 1000);
        p[1] = -500 + (rand() % 1000);
        p[2] = -500 + (rand() % 1000);
        pointItem->setPoint(p);

        unsigned char fill[] = {255, 255, 255, 255};
        pointItem->setFill(fill);
        itemUS.insert(pointItem);
    }

    for(int i = 0; i < 1000; ++i)
    {
        PolygonItem * polygonItem = new PolygonItem();
        PointDV points;
        PointD p;
       // double z = (rand() % 500);
        p[0] = -500 + (rand() % 500);
        p[1] = -500 + (rand() % 500);
        points.push_back(p);
        PointD p1;
        p1[0] = p[0];
        p1[1] = p[1] + 10;
        points.push_back(p1);
        PointD p2;
        p2[0] = p[0] + 10;
        p2[1] = p[1] + 10;
        points.push_back(p2);
        PointD p3;
        p3[0] = p[0] + 10;
        p3[1] = p[1];
        points.push_back(p3);
        points.push_back(p);

        polygonItem->setPoints(points);
        float fill[4] = {1.0, 0.0, 0.0, 1.0};
        polygonItem->setFill(&fill[0]);
        itemUS.insert(polygonItem);
    }


//    ImageItem * item1 = loadImageItem(":/test1");
//    if(item1)
//    {
//        item1->setPos(PointD(0, 0));
//        itemUS.insert(item1);

//        ImageItem * v = new ImageItem(*item1);
//        v->setPos(PointD(v->w() + 50, 0));
//        itemUS.insert(v);
//    }

    double k = 50;
    CubeItem * item = new CubeItem();
    item->setSide(CubeItem::BACK, CubeItem::SIDE(PointD(-k, k, -k),
                                                 PointD(k, k, -k),
                                                 PointD(k, -k, -k),
                                                 PointD(-k, -k, -k),
                                                 ColorD(1.0, 0, 0)));
    item->getSide(CubeItem::BACK).setTexture(loadImageItem(":/test1"));

    item->setSide(CubeItem::RIGHT, CubeItem::SIDE(PointD(k, -k, -k),
                                                  PointD(k, k, -k),
                                                  PointD(k, k, k),
                                                  PointD(k, -k, k),
                                                  ColorD(0, 1.0, 0)));
    item->getSide(CubeItem::RIGHT).setTexture(loadImageItem(":/test1"));
    item->setSide(CubeItem::LEFT, CubeItem::SIDE(PointD(-k, -k, k),
                                                 PointD(-k, k, k),
                                                 PointD(-k, k, -k),
                                                 PointD(-k, -k, -k),
                                                 ColorD(0, 0, 1.0)));
    item->getSide(CubeItem::LEFT).setTexture(loadImageItem(":/test1"));

    item->setSide(CubeItem::FRONT, CubeItem::SIDE(PointD(k, -k, k),
                                                  PointD(k, k, k),
                                                  PointD(-k, k, k),
                                                  PointD(-k, -k, k),
                                                  ColorD(1.0, 1.0, 1.0)));
    item->getSide(CubeItem::FRONT).setTexture(loadImageItem(":/test1"));

    item->setSide(CubeItem::BOTTOM, CubeItem::SIDE(PointD(k, -k, -k),
                                                   PointD(k, -k, k),
                                                   PointD(-k, -k, k),
                                                   PointD(-k, -k, -k),
                                                   ColorD(1.0, 1.0, 0)));
    item->getSide(CubeItem::BOTTOM).setTexture(loadImageItem(":/test1"));

    item->setSide(CubeItem::TOP, CubeItem::SIDE(PointD(k, k, k),
                                                PointD(k, k, -k),
                                                PointD(-k, k, -k),
                                                PointD(-k, k, k),
                                                ColorD(1.0, 0, 1.0)));
    item->getSide(CubeItem::TOP).setTexture(loadImageItem(":/test1"));
//    itemUS.insert(item);

//    CubeItem * item_2 = new CubeItem(*item);
//    item_2->setPos(PointD(500, 0));
//    itemUS.insert(item_2);

    BoundingBox input(PointD(-1000, -1000, -1000), PointD(1000, 1000, 1000));
//    plydriver driver;
//    PlyModel * plyModel = new PlyModel(input);
//    driver.load(plyModel,
//                QString("%1%2%3")
//                .arg(qApp->applicationDirPath())
//                .arg("/")
//                .arg("dragon.ply").toStdString().c_str());
//    m_render->addModel(plyModel);
//    BoundingBox input;
//    for(auto & v : itemUS)
//        BoundingBox::calculate(input, v->getBoundingBox());
//    qDebug() << input.min.x() << input.min.y() << input.min.z()
//             << input.max.x() << input.max.y() << input.max.z();
    OctoModel * model = new OctoModel(4096, input);
    model->setItemL(itemUS);
    m_render->addModel(model);


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
