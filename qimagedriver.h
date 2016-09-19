#ifndef QIMAGEDRIVER_H
#define QIMAGEDRIVER_H

#include "basedriver.h"

class qimagedriver : public basedriver
{
public:
    qimagedriver();
    bool load(Model * model, const char * filepath);
    bool save(Model * model, const char * filepath);
};

#endif // QIMAGEDRIVER_H
