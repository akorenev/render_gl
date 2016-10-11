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
    m_IFunctions->glUseProgram(m_shader->getProgramId());

    int idMatrix = m_shader->getShaderInfo()[0]->getKeyAttribute("matrix", ShaderInfo::UNIFORM);
    m_IFunctions->glUniformMatrix4fv(idMatrix, 1, 0, painterInfo->topMatrix().data());

    int idPosition = m_shader->getShaderInfo()[0]->getKeyAttribute("vPosition", ShaderInfo::LOCATION);

    for(auto & e : m_model->getItemL())
    {
        switch (e->getType())
        {
        case Item::POINTS:
        {
            pointsitem * item = dynamic_cast<pointsitem*>(e);
            Q_ASSERT(item);

            m_IFunctions->glVertexAttribPointer(idPosition, 3, GL_FLOAT, GL_FALSE,
                                                sizeof(Vertex), &item->m_vertexes[0].position);
            m_IFunctions->glEnableVertexAttribArray(idPosition);

            int idColor = m_shader->getShaderInfo()[0]->getKeyAttribute("vColor", ShaderInfo::LOCATION);
            m_IFunctions->glVertexAttribPointer(idColor, 4, GL_FLOAT, GL_TRUE,
                                                sizeof(Vertex), &item->m_vertexes[0].color);
            m_IFunctions->glEnableVertexAttribArray(idColor);

            m_IFunctions->glDrawElements(GL_TRIANGLES, item->m_indices.size(), GL_UNSIGNED_SHORT, &item->m_indices[0]);

        } break;
        default:
            break;
        }
    }
    m_IFunctions->glDisableVertexAttribArray(idPosition);

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
    //shaderPoint(m_IFunctions, m_shaderPoint);
    const char * vShaderStr =
            "#ifdef GL_ES \n"
            "precision mediump int;\n"
            "precision mediump float;\n"
            "#endif \n"
            "attribute vec4 vPosition; \n"
            "attribute vec4 vColor;\n"
            "uniform mat4 matrix; \n"
            "varying vec4 rColor;\n"
            "void main() \n"
            "{ \n"
            " gl_Position = matrix * vPosition; \n"
            " gl_PointSize = 10.0; \n"
            " rColor = vColor; \n"
            "} \n";

    const char * fShaderStr =
            "#ifdef GL_ES \n"
            "precision mediump int;\n"
            "precision mediump float;\n"
            "#endif \n"
            "varying vec4 rColor;\n"
            "void main() \n"
            "{ \n"
            " gl_FragColor = rColor; \n"
            "} \n";

    ShaderInfoV shaderInfoV;
    ShaderInfo::Attributes attributesLocation;
    attributesLocation.push_back(Attribute("vPosition"));
    attributesLocation.push_back(Attribute("vColor"));
    ShaderInfo::Attributes attributesUniformLocation;
    attributesUniformLocation.push_back(Attribute("matrix"));
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(vShaderStr, ShaderInfo::VERTEX,
                                                         attributesLocation, attributesUniformLocation)));
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(fShaderStr, ShaderInfo::FRAGMENT)));
    m_shader = Shader::Ptr(new Shader(m_IFunctions, shaderInfoV));
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
//    ShaderInfoV shaderInfoV;


//    ShaderInfo::Attributes attributes;
//    const char * value = "varying vec4 firstColor;\n"
//                         "uniform vec4 color;\n"
//                         "uniform vec2 border;\n"
//                         "void main() {\n"
//                         " if ((abs(gl_TexCoord[0].x) < border.x) && (abs(gl_TexCoord[0].y) < border.y))  \n"
//                         " gl_FragColor = firstColor;\n"
//                         " else\n"
//                         " gl_FragColor = color;"

//                         "}\n";

//    attributes["color"] = 0;
//    attributes["border"] = 0;
//    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::FRAGMENT, attributes)));

//    attributes.clear();
//    attributes["matrix"] = 0;
//    value = "varying vec4 firstColor;\n"
//            "uniform mat4 matrix;\n"
//            "void main() {\n"
//            "gl_Position = matrix * gl_Vertex; \n"
//            "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
//            "firstColor = gl_Color;"
//            "}\n";
//    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::VERTEX, attributes)));



//    shaderPolygon = Shader::Ptr(new Shader(iFunctions, shaderInfoV));
}

void shaderPoint(IFunctions * iFunctions,
                 Shader::Ptr & shaderPoint)
{
//    ShaderInfoV shaderInfoV;


//    ShaderInfo::Attributes attributes;
//    const char * value = "varying vec4 firstColor;\n"
//                         "uniform vec4 color;\n"
//                         "void main() {\n"
//                         " gl_FragColor = firstColor;\n"
//                         "}\n";

//    attributes["color"] = 0;
//    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::FRAGMENT, attributes)));

//    attributes.clear();
//    attributes["matrix"] = 0;
//    value = "varying vec4 firstColor;\n"
//            "uniform mat4 matrix;\n"
//            "void main() {\n"
//            "gl_Position = matrix * gl_Vertex; \n"
//            "firstColor = gl_Color;"
//            "}\n";
//    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(value, ShaderInfo::VERTEX, attributes)));



//    shaderPoint = Shader::Ptr(new Shader(iFunctions, shaderInfoV));
}
