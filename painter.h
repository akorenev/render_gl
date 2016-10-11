#ifndef PAINTER_H
#define PAINTER_H

#include "ipainter.h"
#include "pointsitem.h"
#include "imageitem.h"
#include "cubeitem.h"
#include "polygonitem.h"
#include "octotree.h"
#include "shader.h"

typedef std::map<ImageItem *, unsigned int> ImageTextures;
struct CubeTexture
{
    unsigned int value(ImageItem * key)
    {
        ImageTextures::iterator it = imageTextures.find(key);
        return it == imageTextures.end() ? 0 : it->second;

    }
    ImageTextures imageTextures;
};
typedef std::map<CubeItem *, CubeTexture> CubeTextures;

class painter : public ipainter
{
public:
    painter();
    ~painter();
    void draw(IPainterInfo::Ptr painterInfo);
    void setIFunctions(IFunctions * iFunctions);
private:
    void init_shaders();
    ImageTextures m_imageTextures;
    CubeTextures m_cubeTextures;
    Shader::Ptr m_shaderPolygon;
    Shader::Ptr m_shaderPoint;
    Shader::Ptr m_shader;
};

void drawOctoModel(OctoModel * model, IFunctions * iFunctions);
void drawImages(const std::vector<ImageItem *> &imageItemV,
                IFunctions * iFunctions,
                ImageTextures & imageTextures);
void drawCubes(const std::vector<CubeItem *> & cubeItemV,
               IFunctions * iFunctions, CubeTextures & cubeTextures);
void drawPolygons(const std::vector<PolygonItem*> &polygonItemV,
                  Shader::Ptr shaderPolygon,
                  IFunctions * iFunctions, IPainterInfo::Ptr painterInfo);

#endif // PAINTER_H
