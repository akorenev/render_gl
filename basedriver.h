#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include "model.h"

class basedriver
{
public:
    basedriver();
    virtual ~basedriver();
    virtual bool load(Model * model, const char * filepath) = 0;
    virtual bool save(Model * model, const char * filepath) = 0;
};

#endif // BASEDRIVER_H
