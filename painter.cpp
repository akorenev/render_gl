#include "painter.h"

painter::painter(){}

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

void painter::draw()
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
        drawPoints(pointItemV, m_IFunctions);
//    if(polygonItemV.size() > 0)
//        drawPolygons(polygonItemV);
    if(imageItemV.size() > 0)
        drawImages(imageItemV, m_IFunctions, m_imageTextures);
    if(cubeItemV.size() > 0)
        drawCubes(cubeItemV, m_IFunctions, m_cubeTextures);
//    if(m_model->getType() == Model::OCTOTREE)
//        drawOctoModel((OctoModel*)m_model, m_IFunctions);
}

void drawPoints(const std::vector<PointItem> &pointItemV,
                IFunctions * iFunctions)
{
    if(pointItemV.empty())
        return;
    const PointItem & pointItem = pointItemV[0];
    const PointD & p = pointItem.getPoint();

    iFunctions->glEnableClientState(GL_COLOR_ARRAY);
    iFunctions->glEnableClientState(GL_VERTEX_ARRAY);
    iFunctions->glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(PointItem) , pointItem.getFill());
    iFunctions->glVertexPointer(3, GL_DOUBLE, sizeof(PointItem), &(p.x()));
    iFunctions->glPointSize(4.0);
    iFunctions->glDrawArrays(GL_POINTS, 0, pointItemV.size());
    iFunctions->glDisableClientState(GL_COLOR_ARRAY);
    iFunctions->glDisableClientState(GL_VERTEX_ARRAY);
}

void drawOctoModel_p(const OctoTree * node, IFunctions * iFunctions)
{
    if(!node)
        return;
    iFunctions->glColor3d(0.0, 1.0, 0.0);
    const BoundingBox & bbox = node->bbox;
    const PointD& min = bbox.min;
    const PointD& max = bbox.max;
    // back
    iFunctions->glBegin(GL_LINE_LOOP);
    iFunctions->glVertex3d(min.x(), min.y(), min.z());
    iFunctions->glVertex3d(min.x(), max.y(), min.z());
    iFunctions->glVertex3d(max.x(), max.y(), min.z());
    iFunctions->glVertex3d(max.x(), min.y(), min.z());
    iFunctions->glEnd();

    // front
    iFunctions->glBegin(GL_LINE_LOOP);
    iFunctions->glVertex3d(min.x(), min.y(), max.z());
    iFunctions->glVertex3d(min.x(), max.y(), max.z());
    iFunctions->glVertex3d(max.x(), max.y(), max.z());
    iFunctions->glVertex3d(max.x(), min.y(), max.z());
    iFunctions->glEnd();

    // left
    iFunctions->glBegin(GL_LINE_LOOP);
    iFunctions->glVertex3d(min.x(), min.y(), min.z());
    iFunctions->glVertex3d(min.x(), max.y(), min.z());
    iFunctions->glVertex3d(min.x(), max.y(), max.z());
    iFunctions->glVertex3d(min.x(), min.y(), max.z());
    iFunctions->glEnd();

    // right
    iFunctions->glBegin(GL_LINE_LOOP);
    iFunctions->glVertex3d(max.x(), max.y(), max.z());
    iFunctions->glVertex3d(max.x(), min.y(), max.z());
    iFunctions->glVertex3d(max.x(), min.y(), min.z());
    iFunctions->glVertex3d(max.x(), max.y(), min.z());
    iFunctions->glEnd();

    // top
    iFunctions->glBegin(GL_LINE_LOOP);
    iFunctions->glVertex3d(max.x(), max.y(), max.z());
    iFunctions->glVertex3d(max.x(), max.y(), min.z());
    iFunctions->glVertex3d(min.x(), max.y(), min.z());
    iFunctions->glVertex3d(min.x(), max.y(), max.z());
    iFunctions->glEnd();

    // bottom
    iFunctions->glBegin(GL_LINE_LOOP);
    iFunctions->glVertex3d(min.x(), min.y(), min.z());
    iFunctions->glVertex3d(min.x(), min.y(), max.z());
    iFunctions->glVertex3d(max.x(), min.y(), max.z());
    iFunctions->glVertex3d(max.x(), min.y(), min.z());
    iFunctions->glEnd();

    for(int i = 1; i <OctoTree::size; ++i)
        drawOctoModel_p(node->nodes[i], iFunctions);
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
    for(ImageItem * item : imageItemV)
    {

        ImageTextures::iterator it = imageTextures.find(item);
        unsigned int textureId = 0;
        if(it == imageTextures.end())
        {
            iFunctions->glShadeModel(GL_FLAT);
            iFunctions->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            iFunctions->glGenTextures(1, &textureId);
            iFunctions->glBindTexture(GL_TEXTURE_2D,  textureId);
            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            iFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, item->w(), item->h(),
                                      0, GL_RGBA, GL_UNSIGNED_BYTE, item->bits());
            imageTextures[item] = textureId;
        }
        else
        {
            textureId = it->second;
        }

        const PointD & p = item->getPos();
        iFunctions->glEnable(GL_TEXTURE_2D);
        iFunctions->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        iFunctions->glBindTexture(GL_TEXTURE_2D, textureId);
        iFunctions->glBegin(GL_QUADS);
        double xmax = p.x() + item->w();
        double ymax = p.y() + item->h();
        iFunctions->glTexCoord2d(0, 1);
        iFunctions->glVertex2d(p.x(), p.y());

        iFunctions->glTexCoord2d(0, 0);
        iFunctions->glVertex2d(p.x(), ymax);

        iFunctions->glTexCoord2d(1, 0);
        iFunctions->glVertex2d(xmax, ymax);

        iFunctions->glTexCoord2d(1, 1);
        iFunctions->glVertex2d(xmax, p.y());

        iFunctions->glEnd();
        iFunctions->glDisable(GL_TEXTURE_2D);
    }
}



void createTextureSideCube_p(IFunctions * iFunctions,
                             unsigned int &textureId,
                             ImageItem * imageItem)
{
    iFunctions->glShadeModel(GL_FLAT);
    iFunctions->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    iFunctions->glGenTextures(1, &textureId);
    iFunctions->glBindTexture(GL_TEXTURE_2D,  textureId);
    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    iFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    iFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageItem->w(), imageItem->h(),
                             0, GL_RGBA, GL_UNSIGNED_BYTE, imageItem->bits());
}

void drawCubes(const std::vector<CubeItem *> & cubeItemV,
               IFunctions * iFunctions, CubeTextures & cubeTextures)
{
    for(auto & cubeItem : cubeItemV)
    {
        iFunctions->glCullFace(GL_BACK);
        iFunctions->glEnable(GL_CULL_FACE);
        for(int i = 0; i < 6; ++i)
        {
            const CubeItem::SIDE & side =
                    cubeItem->getSide(static_cast<CubeItem::TYPE>(i));

//            iFunctions->glBegin(GL_POLYGON);
//            const ColorD & color = side.getColor();
//            iFunctions->glColor3d(color.r(), color.g(), color.b());
//            for(int j = 0; j < 4; ++j)
//            {
//                const PointD & p = side.getData(j);
//                iFunctions->glVertex3d(p.x(), p.y(), p.z());
//            }
//            iFunctions->glEnd();
            ImageItem * imageItem = side.getTexture();
            if(!imageItem)
                continue;
            CubeTextures::iterator it = cubeTextures.find(cubeItem);
            unsigned int textureId = 0;
            if(it == cubeTextures.end())
            {
                createTextureSideCube_p(iFunctions, textureId, imageItem);
                CubeTexture cubeTexture;
                cubeTexture.imageTextures[imageItem] =  textureId;
                cubeTextures[cubeItem] = cubeTexture;
            }
            else
            {
                textureId = it->second.value(imageItem);
                if(!textureId)
                {
                    createTextureSideCube_p(iFunctions, textureId, imageItem);
                    it->second.imageTextures[imageItem] = textureId;
                }
            }


            const PointD & p1 = side.getData(0);
            const PointD & p2 = side.getData(1);
            const PointD & p3 = side.getData(2);
            const PointD & p4 = side.getData(3);
            iFunctions->glEnable(GL_TEXTURE_2D);
            iFunctions->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            iFunctions->glBindTexture(GL_TEXTURE_2D, textureId);
            iFunctions->glBegin(GL_QUADS);

            iFunctions->glTexCoord2d(0, 1);
            iFunctions->glVertex3d(p1.x(), p1.y(), p1.z());

            iFunctions->glTexCoord2d(0, 0);
            iFunctions->glVertex3d(p2.x(), p2.y(), p2.z());

            iFunctions->glTexCoord2d(1, 0);
            iFunctions->glVertex3d(p3.x(), p3.y(), p3.z());

            iFunctions->glTexCoord2d(1, 1);
            iFunctions->glVertex3d(p4.x(), p4.y(), p4.z());

            iFunctions->glEnd();
            iFunctions->glDisable(GL_TEXTURE_2D);
        }
        iFunctions->glDisable(GL_CULL_FACE);
    }
}


void drawPolygons(const std::vector<PolygonItem*> &polygonItemV,
                  IFunctions * iFunctions)
{
//    iFunctions->glUseProgram(m_programRect);
//    for(PolygonItem * item : polygonItemV)
//    {
//        iFunctions->glUniform4fv(attrib_fragment, 1, item->getFill());

//        float v1[] = {0.95, 0.95};
//        iFunctions->glUniform2fv(attrib_border, 1, &v1[0]);
//        iFunctions->glBegin(GL_POLYGON);
//        int i = 0;
//        float x[] = {-1, -1, 1,  1};
//        float y[] = {-1,  1, 1, -1};
//        for(const PointD & p : item->getPoints())
//        {
//            iFunctions->glTexCoord2f(x[i], y[i]);
//            ++i;

//            iFunctions->glVertex3d(p.x(), p.y(), p.z());
//        }
//        iFunctions->glEnd();
//    }
//    iFunctions->glUseProgram(0);
}



void shaderRect(IFunctions * iFunctions)
{
    const char * valueV = "void main() {\n"
                          "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
                         " gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
                         "}\n";

    const char * valueF = "uniform vec4 color;\n"
                          "uniform vec2 border;"
                          "void main() {\n"
                          "if(step(abs(gl_TexCoord[0].x), border.x) \n"
                          " && step(abs(gl_TexCoord[0].y), border.y)) \n"
                          " //if ((abs(gl_TexCoord[0].x) < border.x) && (abs(gl_TexCoord[0].y) < border.y))  \n"
                          " gl_FragColor = color;\n"
                          " else\n"
                          " gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \n"
                          "}\n";


    unsigned int vShader = iFunctions->glCreateShader(GL_VERTEX_SHADER);
    iFunctions->glShaderSource(vShader, 1, &valueV, NULL); // передать текст шейдера
    iFunctions->glCompileShader(vShader); // скомпилировать шейдер

//    unsigned int fShader = iFunctions->glCreateShader(GL_FRAGMENT_SHADER);
//    iFunctions->glShaderSource(fShader, 1, &valueF, NULL); // передать текст шейдера
//    iFunctions->glCompileShader(fShader); // скомпилировать шейдер

//    m_programRect = iFunctions->glCreateProgram();
//    iFunctions->glAttachShader(m_programRect, vShader); // подключить шейдер к проге
//    iFunctions->glAttachShader(m_programRect, fShader);
//    iFunctions->glLinkProgram(m_programRect); // слинковать прогу

//    int ok;
//    iFunctions->glGetProgramiv(m_programRect, GL_LINK_STATUS, &ok); // проверить сборку
//    if(!ok)
//    {
//        qDebug() << "error attach shaders";
//        return;
//    }

//    const char * attr_name_color = "color";
//    attrib_fragment = iFunctions->glGetUniformLocation(m_programRect, attr_name_color); // проверить сущ. аттрибута
//    if(attrib_fragment == -1)
//    {
//        qDebug() << "could not bind attribute coord" << attr_name_color;
//        return;
//    }
//    const char * attrib_border_name = "border";
//    attrib_border = iFunctions->glGetUniformLocation(m_programRect, attrib_border_name); // проверить сущ. аттрибута
//    if(attrib_border == -1)
//    {
//        qDebug() << "could not bind attribute border" << attrib_border;
//        return;
//    }
}

void shaderCube()
{
    const char * valueV = "attribute highp vec4 vertex;\n"
                          "attribute mediump vec4 texCoord;\n"
                          "varying mediump vec4 texc;\n"
                          "void main(void)\n"
                          "{\n"
                          "glPosition = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
                          "texc = texCoord;\n"
                          "}\n";

    const char * valueF = "uniform sampler2D texture;\n"
                          "varying mediump vec4 texc;\n"
                          "void main(void)\n"
                          "{\n"
                          "gl_FragColor = texture2D(texture, texc.st);\n"
                          "}\n";

//    const char * valueV = "void main() {\n"
//                          "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
//                         " gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
//                         "}\n";

//    const char * valueF = "uniform vec4 color;\n"
//                          "uniform vec2 border;"
//                          "void main() {\n"
//                          " if ((abs(gl_TexCoord[0].x) < border.x) && (abs(gl_TexCoord[0].y) < border.y))  \n"
//                          " gl_FragColor = color;\n"
//                          " else\n"
//                          " gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0); \n"
//                          "}\n";


//    unsigned int vShader = iFunctions->glCreateShader(GL_VERTEX_SHADER);
//    iFunctions->glShaderSource(vShader, 1, &valueV, NULL); // передать текст шейдера
//    iFunctions->glCompileShader(vShader); // скомпилировать шейдер

//    unsigned int fShader = iFunctions->glCreateShader(GL_FRAGMENT_SHADER);
//    iFunctions->glShaderSource(fShader, 1, &valueF, NULL); // передать текст шейдера
//    iFunctions->glCompileShader(fShader); // скомпилировать шейдер

//    m_programCube = iFunctions->glCreateProgram();
//    iFunctions->glAttachShader(m_programCube, vShader); // подключить шейдер к проге
////    iFunctions->glAttachShader(m_programCube, fShader);
//    iFunctions->glBindAttribLocation(m_programCube, vShader, "vertex");
//    iFunctions->glBindAttribLocation(m_programCube, vShader, "texCoord");
//    iFunctions->glLinkProgram(m_programCube); // слинковать прогу

//    int ok;
//    iFunctions->glGetProgramiv(m_programCube, GL_LINK_STATUS, &ok); // проверить сборку
//    qDebug() << iFunctions->glGetError();
//    if(!ok)
//    {
//        qDebug() << "error attach shaders";
//        return;
//    }


//    const char * attr_name_color = "color";
//    attrib_fragment = iFunctions->glGetUniformLocation(m_programCube, attr_name_color); // проверить сущ. аттрибута
//    if(attrib_fragment == -1)
//    {
//        qDebug() << "could not bind attribute coord" << attr_name_color;
//        return;
//    }
//    const char * attrib_border_name = "border";
//    attrib_border = iFunctions->glGetUniformLocation(m_programCube, attrib_border_name); // проверить сущ. аттрибута
//    if(attrib_border == -1)
//    {
//        qDebug() << "could not bind attribute border" << attrib_border;
//        return;
    //    }
}
