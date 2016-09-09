#include "ipainter.h"

ipainter::ipainter()
    : m_IFunctions(0)
    , m_model(0){}

ipainter::~ipainter()
{
    if(m_model)
        delete m_model;
}

void ipainter::setModel(Model *model)
{
    m_model = model;
}

Model * ipainter::getModel()
{
    return m_model;
}

void ipainter::setIFunctions(IFunctions * iFunctions)
{
    m_IFunctions = iFunctions;
}
