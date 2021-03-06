#ifndef PLYDRIVER_H
#define PLYDRIVER_H

#include "basedriver.h"

class plydriver : public basedriver
{
public:
    bool load(Model * model, const char * filepath);
    bool save(Model * model, const char * filepath);
};

#endif // PLYDRIVER_H
