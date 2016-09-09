#ifndef PAINTER_H
#define PAINTER_H

#include "ipainter.h"
#include "pointitem.h"
#include "imageitem.h"
#include "cubeitem.h"
#include "polygonitem.h"
#include "octotree.h"

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
    void draw();
private:
    ImageTextures m_imageTextures;
    CubeTextures m_cubeTextures;
};

void drawPoints(const std::vector<PointItem> &pointItemV, IFunctions * iFunctions);
void drawOctoModel(OctoModel * model, IFunctions * iFunctions);
void drawImages(const std::vector<ImageItem *> &imageItemV,
                IFunctions * iFunctions,
                ImageTextures & imageTextures);
void drawCubes(const std::vector<CubeItem *> & cubeItemV,
               IFunctions * iFunctions, CubeTextures & cubeTextures);
void drawPolygons(const std::vector<PolygonItem*> &polygonItemV,
                  IFunctions * iFunctions);

#endif // PAINTER_H
