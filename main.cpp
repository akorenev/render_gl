#include <QApplication>
#include "widget.h"
#include "polygonitem.h"
#include "imageitem.h"
#include "cubeitem.h"
#include "octotree.h"
#include "plydriver.h"
#include "plymodel.h"
#include "qimagedriver.h"
#include <QApplication>
#include <QFile>
#include "pointsitem.h"


void init_0(Model *& model)
{
//    ItemUS itemUS;

//    for(int i = 0; i < 100; ++i)
//    {
//        PolygonItem * polygonItem = new PolygonItem();
//        PointDV points;
//        PointD p;
//       // double z = (rand() % 500);
//        p[0] = -500 + (rand() % 1000);
//        p[1] = -500 + (rand() % 1000);
//        points.push_back(p);
//        PointD p1;
//        p1[0] = p[0];
//        p1[1] = p[1] + 10;
//        points.push_back(p1);
//        PointD p2;
//        p2[0] = p[0] + 10;
//        p2[1] = p[1] + 10;
//        points.push_back(p2);
//        PointD p3;
//        p3[0] = p[0] + 10;
//        p3[1] = p[1];
//        points.push_back(p3);
//        points.push_back(p);

//        polygonItem->setPoints(points);
//        float fill[4] = {1.0, 0.0, 0.0, 1.0};
//        polygonItem->setFill(&fill[0]);
//        itemUS.insert(polygonItem);
//    }

//    Model modelImage(0);
//    qimagedriver imagedriver;
//    if(imagedriver.load(&modelImage, ":/test1"))
//       itemUS = cmerge(itemUS, modelImage.getItemL());


//    ImageItem * imageItem = (ImageItem*)*modelImage.getItemL().begin();

//    double k = 50;
//    CubeItem * item = new CubeItem();
//    item->setSide(CubeItem::BACK, CubeItem::SIDE(PointD(-k, k, -k),
//                                                 PointD(k, k, -k),
//                                                 PointD(k, -k, -k),
//                                                 PointD(-k, -k, -k),
//                                                 ColorD(1.0, 0, 0)));
//    item->getSide(CubeItem::BACK).setTexture(new ImageItem(*imageItem));

//    item->setSide(CubeItem::RIGHT, CubeItem::SIDE(PointD(k, -k, -k),
//                                                  PointD(k, k, -k),
//                                                  PointD(k, k, k),
//                                                  PointD(k, -k, k),
//                                                  ColorD(0, 1.0, 0)));
//    item->getSide(CubeItem::RIGHT).setTexture(new ImageItem(*imageItem));
//    item->setSide(CubeItem::LEFT, CubeItem::SIDE(PointD(-k, -k, k),
//                                                 PointD(-k, k, k),
//                                                 PointD(-k, k, -k),
//                                                 PointD(-k, -k, -k),
//                                                 ColorD(0, 0, 1.0)));
//    item->getSide(CubeItem::LEFT).setTexture(new ImageItem(*imageItem));

//    item->setSide(CubeItem::FRONT, CubeItem::SIDE(PointD(k, -k, k),
//                                                  PointD(k, k, k),
//                                                  PointD(-k, k, k),
//                                                  PointD(-k, -k, k),
//                                                  ColorD(1.0, 1.0, 1.0)));
//    item->getSide(CubeItem::FRONT).setTexture(new ImageItem(*imageItem));

//    item->setSide(CubeItem::BOTTOM, CubeItem::SIDE(PointD(k, -k, -k),
//                                                   PointD(k, -k, k),
//                                                   PointD(-k, -k, k),
//                                                   PointD(-k, -k, -k),
//                                                   ColorD(1.0, 1.0, 0)));
//    item->getSide(CubeItem::BOTTOM).setTexture(new ImageItem(*imageItem));

//    item->setSide(CubeItem::TOP, CubeItem::SIDE(PointD(k, k, k),
//                                                PointD(k, k, -k),
//                                                PointD(-k, k, -k),
//                                                PointD(-k, k, k),
//                                                ColorD(1.0, 0, 1.0)));
//    item->getSide(CubeItem::TOP).setTexture(new ImageItem(*imageItem));
//    itemUS.insert(item);

//    CubeItem * item_2 = new CubeItem(*item);
//    item_2->setPos(PointD(500, 0));
//    itemUS.insert(item_2);

    BoundingBox input(PointD(-1000, -1000, -1000), PointD(1000, 1000, 1000));
    plydriver driver;
    PlyModel * plyModel = new PlyModel(input);
    driver.load(plyModel, QString(":/bunny").toStdString().c_str());
    model = plyModel;
//    for(auto & v : itemUS)
//        BoundingBox::calculate(input, v->getBoundingBox());
//    qDebug() << input.min.x() << input.min.y() << input.min.z()
//             << input.max.x() << input.max.y() << input.max.z();
//    OctoModel * model = new OctoModel(4096, input);
//    itemUS.insert(new pointsitem());
//    model->setItemL(itemUS);
//    m_render->addModel(model);
}

void init_1(Model *& model)
{
    ItemUS itemUS;
    pointsitem * items = new pointsitem();
    for(int i = 0; i < 3; ++i)
    {

        PointF p;
        p[0] = -500 + (rand() % 1000);
        p[1] = -500 + (rand() % 1000);
        p[2] = -500 + (rand() % 1000);

        ColorF color(1.0, 0, 0, 1.0);
        items->m_vertexes.push_back(Vertex(p, color));
        items->m_indices.push_back(i);
    }
    BoundingBox input(PointD(-1000, -1000, -1000), PointD(1000, 1000, 1000));
    OctoModel * octomodel = new OctoModel(4096, input);
    itemUS.insert(items);
    octomodel->setItemL(itemUS);
    model = octomodel;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    Model * model = 0;
    init_0(model);
    w.addModel(model);
    return a.exec();
}
