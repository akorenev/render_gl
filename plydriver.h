#ifndef PLYDRIVER_H
#define PLYDRIVER_H

#include "plymodel.h"

class plydriver
{
public:
    bool load(PlyModel * model, const char * filepath);
    bool save(PlyModel * model, const char * filepath);
};

#endif // PLYDRIVER_H
