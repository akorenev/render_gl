#include "qimagedriver.h"
#include "imageitem.h"
#include <QImage>

qimagedriver::qimagedriver(){}

bool qimagedriver::load(Model *model, const char *filepath)
{
    QString path(filepath);
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
        return false;
    ImageItem * item = new ImageItem(bits, d, w, h);
    ItemUS itemUS;
    itemUS.insert(item);
    model->setItemL(itemUS);
    return true;
}

bool qimagedriver::save(Model *model, const char *filepath)
{
    return false;
}
