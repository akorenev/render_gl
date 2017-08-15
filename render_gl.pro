QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = render_gl
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp \
    render.cpp \
    model.cpp \
    filter.cpp \
    item.cpp \
    polygonitem.cpp \
    imageitem.cpp \
    cubeitem.cpp \
    octotree.cpp \
    ray.cpp \
    plydriver.cpp \
    plymodel.cpp \
    ipainter.cpp \
    plypainter.cpp \
    painter.cpp \
    qimagedriver.cpp \
    basedriver.cpp \
    shader.cpp \
    pointsitem.cpp
HEADERS  += widget.h \
    render.h \
    model.h \
    ifunctions.h \
    filter.h \
    item.h \
    polygonitem.h \
    imageitem.h \
    cubeitem.h \
    octotree.h \
    color.h \
    point.h \
    e_data/swap.h \
    e_data/bubble_sort.h \
    e_data/heap_sort.h \
    ray.h \
    boundingbox.h \
    item_t.h \
    plydriver.h \
    plymodel.h \
    ipainter.h \
    plypainter.h \
    painter.h \
    e_data/insertion_sort.h \
    qimagedriver.h \
    basedriver.h \
    shader.h \
    shaderinfo.h \
    pointsitem.h \
    vertex.h

RESOURCES += \
    res.qrc
