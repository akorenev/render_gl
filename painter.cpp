#include "painter.h"
#include <QDebug>

void shaderRect(IFunctions * iFunctions,
                Shader::Ptr & shaderPolygon);
void shaderPoint(IFunctions * iFunctions,
                 Shader::Ptr & shaderPoint);

painter::painter()
{
}

painter::~painter()
{
    ImageTextures::iterator it = m_imageTextures.begin();
    while (it != m_imageTextures.end())
    {
        unsigned int textureId = it->second;
        m_IFunctions->glDeleteTextures(1, &textureId);
        it = m_imageTextures.erase(it);
    }

    CubeTextures::iterator i = m_cubeTextures.begin();
    while(i != m_cubeTextures.end())
    {
        ImageTextures::iterator j = i->second.imageTextures.begin();
        for(;j != i->second.imageTextures.end(); ++j)
            m_IFunctions->glDeleteTextures(1, &j->second);
        i = m_cubeTextures.erase(i);
    }
}

void painter::draw(IPainterInfo::Ptr painterInfo)
{
    std::vector<PointItem> pointItemV;
    std::vector<PolygonItem*> polygonItemV;
    std::vector<ImageItem*> imageItemV;
    std::vector<CubeItem*> cubeItemV;
    for(auto & e : m_model->getItemL())
    {
        switch (e->getType())
        {
        case Item::POINT:
        {
            PointItem * item = (PointItem*)e;
            pointItemV.push_back(*item);
        } break;
        case Item::POLYGON:
        {
            PolygonItem * item = (PolygonItem*)e;
            polygonItemV.push_back(item);
        } break;
        case Item::IMAGE:
        {
            ImageItem * item = (ImageItem*)e;
            imageItemV.push_back(item);
        } break;
        case Item::CUBE:
        {
            CubeItem * item = (CubeItem*)e;
            cubeItemV.push_back(item);
        } break;
        default:
            break;
        }
    }
    if(pointItemV.size() > 0)
        drawPoints(pointItemV, m_shaderPoint, m_IFunctions, painterInfo);
    if(polygonItemV.size() > 0)
        drawPolygons(polygonItemV, m_shaderPolygon, m_IFunctions, painterInfo);
    if(imageItemV.size() > 0)
        drawImages(imageItemV, m_IFunctions, m_imageTextures);
    if(cubeItemV.size() > 0)
        drawCubes(cubeItemV, m_IFunctions, m_cubeTextures);
//    if(m_model->getType() == Model::OCTOTREE)
//            drawOctoModel((OctoModel*)m_model, m_IFunctions);
}

void painter::setIFunctions(IFunctions *iFunctions)
{
    ipainter::setIFunctions(iFunctions);
    init_shaders();
}

void painter::init_shaders()
{
    //shaderRect(m_IFunctions, m_shaderPolygon);
    shaderPoint(m_IFunctions, m_shaderPoint);
}

void drawPoints(const std::vector<PointItem> &pointItemV, Shader::Ptr shaderPoint,
                IFunctions * iFunctions, IPainterInfo::Ptr painterInfo)
{
//    if(pointItemV.empty())
//        return;
//    const PointItem & pointItem = pointItemV[0];
//    const PointD & p = pointItem.getPoint();

//    iFunctions->glEnableClientState(GL_COLOR_ARRAY);
//    iFunctions->glEnableClientState(GL_VERTEX_ARRAY);
//    iFunctions->glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(PointItem) , pointItem.getFill());
//    iFunctions->glVertexPointer(3, GL_DOUBLE, sizeof(PointItem), &(p.x()));
//    iFunctions->glPointSize(4.0);
//    iFunctions->glDrawArrays(GL_POINTS, 0, pointItemV.size());
//    iFunctions->glDisableClientState(GL_COLOR_ARRAY);
//    iFunctions->glDisableClientState(GL_VERTEX_ARRAY);

//    iFunctions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
//    iFunctions->glEnableVertexAttribArray(0);
//    iFunctions->glDrawArrays(GL_TRIANGLES, 0, 3);

    iFunctions->glUseProgram(shaderPoint->getProgramId());
    const ShaderInfoV & shaderInfoV = shaderPoint->getShaderInfo();
    int matrixId = shaderInfoV[1]->getKeyAttribute("matrix");
    if(matrixId != -1)
        iFunctions->glUniformMatrix4fv(matrixId, 1, 0, painterInfo->topMatrix().data());
    int colorId = shaderInfoV[0]->getKeyAttribute("color");
    if(colorId != -1)
    {
        float v[] = {0.0, 1.0, 0.0, 1.0};
        iFunctions->glUniform4fv(colorId, 1, &v[0]);
    }
    GLfloat vVertices[] = {0.0f, 500.0f, 0.0f,
                           -500.0f, -500.0f, 0.0f,
                           500.0f, -500.0f, 0.0f};

    iFunctions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    iFunctions->glEnableVertexAttribArray(0);
    iFunctions->glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawOctoModel_p(const OctoTree * node, IFunctions * iFunctions)
{
//    if(!node)
//        return;
//    iFunctions->glColor3d(0.0, 1.0, 0.0);
//    const BoundingBox & bbox = node->bbox;
//    const PointD& min = bbox.min;
//    const PointD& max = bbox.max;
//    // back
//    iFunctions->glBegin(GL_LINE_LOOP);
//    iFunctions->glVertex3d(min.x(), min.y(), min.z());
//    iFunctions->glVertex3d(min.x(), max.y(), min.z());
//    iFunctions->glVertex3d(max.x(), max.y(), min.z());
//    iFunctions->glVertex3d(max.x(), min.y(), min.z());
//    iFunctions->glEnd();

//    // front
//    iFunctions->glBegin(GL_LINE_LOOP);
//    iFunctions->glVertex3d(min.x(), min.y(), max.z());
//    iFunctions->glVertex3d(min.x(), max.y(), max.z());
//    iFunctions->glVertex3d(max.x(), max.y(), max.z());
//    iFunctions->glVertex3d(max.x(), min.y(), max.z());
//    iFunctions->glEnd();

//    // left
//    iFunctions->glBegin(GL_LINE_LOOP);
//    iFunctions->glVertex3d(min.x(), min.y(), min.z());
//    iFunctions->glVertex3d(min.x(), max.y(), min.z());
//    iFunctions->glVertex3d(min.x(), max.y(), max.z());
//    iFunctions->glVertex3d(min.x(), min.y(), max.z());
//    iFunctions->glEnd();

//    // right
//    iFunctions->glBegin(GL_LINE_LOOP);
//    iFunctions->glVertex3d(max.x(), max.y(), max.z());
//    iFunctions->glVertex3d(max.x(), min.y(), max.z());
//    iFunctions->glVertex3d(max.x(), min.y(), min.z());
//    iFunctions->glVertex3d(max.x(), max.y(), min.z());
//    iFunctions->glEnd();

//    // top
//    iFunctions->glBegin(GL_LINE_LOOP);
//    iFunctions->glVertex3d(max.x(), max.y(), max.z());
//    iFunctions->glVertex3d(max.x(), max.y(), min.z());
//    iFunctions->glVertex3d(min.x(), max.y(), min.z());
//    iFunctions->glVertex3d(min.x(), max.y(), max.z());
//    iFunctions->glEnd();

//    // bottom
//    iFunctions->glBegin(GL_LINE_LOOP);
//    iFunctions->glVertex3d(min.x(), min.y(), min.z());
//    iFunctions->glVertex3d(min.x(), min.y(), max.z());
//    iFunctions->glVertex3d(max.x(), min.y(), max.z());
//    iFunctions->glVertex3d(max.x(), min.y(), min.z());
//    iFunctions->glEnd();

//    for(int i = 1; i <OctoTree::size; ++i)
//        drawOctoModel_p(node->nodes[i], iFunctions);
}

void drawOctoModel(OctoModel *model, IFunctions * iFunctions)
{
    const OctoTree * o = model->root();
    if(o)
        drawOctoModel_p(o, iFunctions);
}

void drawImages(const std::vector<ImageItem *> &imageItemV,
                IFunctions * iFunctions,
                ImageTextures & imageTextures)
{
//    for(ImageItem * item : imageItemV)
//    {

//        ImageTextures::iterator it = imageTextures.find(item);
//        unsigned int textureId = 0;
//        if(it == imageTextures.end())
//        {
//            iFunctions->glShadeModel(GL_FLAT);
//            iFunctions->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//            iFunctions->glGenTextures(1, &textureId);
//            iFunctions->glBindTexture(GL_TEXTURE_2D,  textureId);
//            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            iFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, item->w(), item->h(),
//                                      0, GL_RGBA, GL_UNSIGNED_BYTE, item->bits());
//            imageTextures[item] = textureId;
//        }
//        else
//        {
//            textureId = it->second;
//        }

//        const PointD & p = item->getPos();
//        iFunctions->glEnable(GL_TEXTURE_2D);
//        iFunctions->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//        iFunctions->glBindTexture(GL_TEXTURE_2D, textureId);
//        iFunctions->glBegin(GL_QUADS);
//        double xmax = p.x() + item->w();
//        double ymax = p.y() + item->h();
//        iFunctions->glTexCoord2d(0, 1);
//        iFunctions->glVertex2d(p.x(), p.y());

//        iFunctions->glTexCoord2d(0, 0);
//        iFunctions->glVertex2d(p.x(), ymax);

//        iFunctions->glTexCoord2d(1, 0);
//        iFunctions->glVertex2d(xmax, ymax);

//        iFunctions->glTexCoord2d(1, 1);
//        iFunctions->glVertex2d(xmax, p.y());

//        iFunctions->glEnd();
//        iFunctions->glDisable(GL_TEXTURE_2D);
//    }
}



void createTextureSideCube_p(IFunctions * iFunctions,
                             unsigned int &textureId,
                             ImageItem * imageItem)
{
//    iFunctions->glShadeModel(GL_FLAT);
//    iFunctions->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    iFunctions->glGenTextures(1, &textureId);
//    iFunctions->glBindTexture(GL_TEXTURE_2D,  textureId);
//    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    iFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageItem->w(), imageItem->h(),
//                             0, GL_RGBA, GL_UNSIGNED_BYTE, imageItem->bits());
}

void drawCubes(const std::vector<CubeItem *> & cubeItemV,
               IFunctions * iFunctions, CubeTextures & cubeTextures)
{
//    for(auto & cubeItem : cubeItemV)
//    {
//        iFunctions->glCullFace(GL_BACK);
//        iFunctions->glEnable(GL_CULL_FACE);
//        for(int i = 0; i < 6; ++i)
//        {
//            const CubeItem::SIDE & side =
//                    cubeItem->getSide(static_cast<CubeItem::TYPE>(i));

////            iFunctions->glBegin(GL_POLYGON);
////            const ColorD & color = side.getColor();
////            iFunctions->glColor3d(color.r(), color.g(), color.b());
////            for(int j = 0; j < 4; ++j)
////            {
////                const PointD & p = side.getData(j);
////                iFunctions->glVertex3d(p.x(), p.y(), p.z());
////            }
////            iFunctions->glEnd();
//            ImageItem * imageItem = side.getTexture();
//            if(!imageItem)
//                continue;
//            CubeTextures::iterator it = cubeTextures.find(cubeItem);
//            unsigned int textureId = 0;
//            if(it == cubeTextures.end())
//            {
//                createTextureSideCube_p(iFunctions, textureId, imageItem);
//                CubeTexture cubeTexture;
//                cubeTexture.imageTextures[imageItem] =  textureId;
//                cubeTextures[cubeItem] = cubeTexture;
//            }
//            else
//            {
//                textureId = it->second.value(imageItem);
//                if(!textureId)
//                {
//                    createTextureSideCube_p(iFunctions, textureId, imageItem);
//                    it->second.imageTextures[imageItem] = textureId;
//                }
//            }


//            const PointD & p1 = side.getData(0);
//            const PointD & p2 = side.getData(1);
//            const PointD & p3 = side.getData(2);
//            const PointD & p4 = side.getData(3);
//            iFunctions->glEnable(GL_TEXTURE_2D);
//            iFunctions->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//            iFunctions->glBindTexture(GL_TEXTURE_2D, textureId);
//            iFunctions->glBegin(GL_QUADS);

//            iFunctions->glTexCoord2d(0, 1);
//            iFunctions->glVertex3d(p1.x(), p1.y(), p1.z());

//            iFunctions->glTexCoord2d(0, 0);
//            iFunctions->glVertex3d(p2.x(), p2.y(), p2.z());

//            iFunctions->glTexCoord2d(1, 0);
//            iFunctions->glVertex3d(p3.x(), p3.y(), p3.z());

//            iFunctions->glTexCoord2d(1, 1);
//            iFunctions->glVertex3d(p4.x(), p4.y(), p4.z());

//            iFunctions->glEnd();
//            iFunctions->glDisable(GL_TEXTURE_2D);
//        }
//        iFunctions->glDisable(GL_CULL_FACE);
//    }
}


void drawPolygons(const std::vector<PolygonItem*> &polygonItemV,
                  Shader::Ptr shaderPolygon,
                  IFunctions * iFunctions, IPainterInfo::Ptr painterInfo)
{
//    if(shaderPolygon)
//    {
//        iFunctions->glUseProgram(shaderPolygon->getProgramId());
//        for(PolygonItem * item : polygonItemV)
//        {
//            const ShaderInfoV & shaderInfoV = shaderPolygon->getShaderInfo();
//            int matrixId = shaderInfoV[1]->getKeyAttribute("matrix");
//            if(matrixId != -1)
//                iFunctions->glUniformMatrix4fv(matrixId, 1, 0, painterInfo->topMatrix().data());
//            int colorId = shaderInfoV[0]->getKeyAttribute("color");
//            if(colorId != -1)
//                iFunctions->glUniform4fv(colorId, 1, &item->getFill()[0]);
//            float v[] = {0.8, 0.8};
//            int borderId = shaderInfoV[0]->getKeyAttribute("border");
//            if(borderId != -1)
//                iFunctions->glUniform2fv(borderId, 1, &v[0]);

//            iFunctions->glBegin(GL_POLYGON);
//            int i = 0;
//            float x[] = {-1, -1, 1,  1};
//            float y[] = {-1,  1, 1, -1};
//            for(const PointD & p : item->getPoints())
//            {
//                iFunctions->glTexCoord2f(x[i], y[i]);
//                ++i;
////                const float* color = item->getFill();
////                iFunctions->glColor3f(0, 1, 0);
//                iFunctions->glVertex3d(p.x(), p.y(), p.z());
//            }
//            iFunctions->glEnd();
//        }
//        iFunctions->glUseProgram(0);
//    }
//    else
//    {
//        for(PolygonItem * item : polygonItemV)
//        {
//            iFunctions->glBegin(GL_POLYGON);
//            const float * color = item->getFill();
//            iFunctions->glColor3f(color[0], color[1], color[2]);
//            for(auto & v: item->getPoints())
//                iFunctions->glVertex3d(v.x(), v.y(), v.z());
//            iFunctions->glEnd();
//        }

//    }
}


void shaderRect(IFunctions * iFunctions,
                Shader::Ptr & shaderPolygon)
{
    ShaderInfoV shaderInfoV;


    ShaderInfo::Attributes attributes;
    const char * value = "varying vec4 firstColor;\n"
                         "uniform vec4 color;\n"
                         "uniform vec2 border;\n"
                         "void main() {\n"
                         " if ((abs(gl_TexCoord[0].x) < border.x) && (abs(gl_TexCoord[0].y) < border.y))  \n"
                         " gl_FragColor = firstColor;\n"
                         " else\n"
                         " gl_FragColor = color;"

                         "}\n";

    attributes["color"] = 0;
    attributes["border"] = 0;
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::FRAGMENT, attributes)));

    attributes.clear();
    attributes["matrix"] = 0;
    value = "varying vec4 firstColor;\n"
            "uniform mat4 matrix;\n"
            "void main() {\n"
            "gl_Position = matrix * gl_Vertex; \n"
            "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
            "firstColor = gl_Color;"
            "}\n";
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::VERTEX, attributes)));



    shaderPolygon = Shader::Ptr(new Shader(iFunctions, shaderInfoV));
}

void shaderPoint(IFunctions * iFunctions,
                 Shader::Ptr & shaderPoint)
{
    ShaderInfoV shaderInfoV;


    ShaderInfo::Attributes attributes;
    const char * value = "varying vec4 firstColor;\n"
                         "uniform vec4 color;\n"
                         "void main() {\n"
                         " gl_FragColor = firstColor;\n"
                         "}\n";

    attributes["color"] = 0;
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::FRAGMENT, attributes)));

    attributes.clear();
    attributes["matrix"] = 0;
    value = "varying vec4 firstColor;\n"
            "uniform mat4 matrix;\n"
            "void main() {\n"
            "gl_Position = matrix * gl_Vertex; \n"
            "firstColor = gl_Color;"
            "}\n";
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::VERTEX, attributes)));



    shaderPoint = Shader::Ptr(new Shader(iFunctions, shaderInfoV));
}
