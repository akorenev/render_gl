#include "plydriver.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include "pointitem.h"
#include <QDebug>
#include "plymodel.h"

using namespace std;

struct Header
{
    string name;
    string version;
    string format;
    list<string> comments;
    // face, vertex
    list<string> properties;
    unsigned int countVertex;
    unsigned int countFace;
};

bool plydriver::load(Model * model, const char * filepath)
{
    PlyModel * plyModel = dynamic_cast<PlyModel*>(model);
    Q_ASSERT(plyModel);
    string line;
    ifstream file(filepath);
    if(file.is_open())
    {
        string header_line;
        bool read_header = false;
        Header header;
        int countVertex;
        int countfaceVertex;
        unsigned int * faceVertex;
        unsigned int faceVertexSize;
        int count = 0;
        int count_v = 0;
        double * vertexes;
        unsigned int vertexSize;
        while (getline(file, line))
        {

            if(!read_header)
            {
                if((line.compare("end_header") == 0))
                {
                    read_header = true;
                    string value;
                    int counttag = 0;
                    for(unsigned int i = 0; i < header_line.size(); ++i)
                    {
                        if(header_line.substr(i, 1) == " "
                                || header_line.substr(i, 1) == "\n")
                        {
                            if(value.compare("ply") == 0
                                    && counttag == 0)
                            {
                                header.name = value;
                                ++counttag;
                            }
                            else if(value.compare("format") == 0)
                            {
                                value = "";
                                ++counttag;
                                int sequence = 0;
                                ++i;
                                while(i < header_line.size()
                                      && header_line.substr(i, 1) != "\n")
                                {
                                    if(header_line.substr(i, 1) == " ")
                                    {
                                        if(sequence == 0)
                                        {
                                            header.format = value;
                                            value = "";
                                            ++sequence;
                                        }
                                    }
                                    else
                                    {
                                        value += header_line.substr(i, 1);
                                    }
                                    ++i;
                                }
                                if(sequence == 1)
                                {
                                    header.version = value;
                                    ++counttag;
                                }
                            }
                            else if(value.compare("element") == 0)
                            {
                                ++i;
                                value = "";
                                while(i < header_line.size()
                                      && header_line.substr(i, 1) != "\n")
                                {
                                    if(header_line.substr(i, 1) == " ")
                                    {
                                        if(value.compare("vertex") == 0)
                                        {
                                            ++i;
                                            value = "";
                                            while(i < header_line.size()
                                                  && header_line.substr(i, 1) != "\n")
                                            {
                                                value += header_line.substr(i++, 1);
                                            }
                                            header.countVertex = atoi(value.c_str());
                                            break;
                                        }
                                        else if(value.compare("face") == 0)
                                        {
                                            ++i;
                                            value = "";
                                            while(i < header_line.size()
                                                  && header_line.substr(i, 1) != "\n")
                                            {
                                                value += header_line.substr(i++, 1);
                                            }
                                            header.countFace = atoi(value.c_str());
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        value += header_line.substr(i, 1);
                                    }
                                    ++i;
                                }
                            }
                            value = "";
                        }
                        else
                        {
                            value += header_line.substr(i, 1);
                        }

                    }
                    countVertex = vertexSize = header.countVertex;
                    vertexSize *= 3;
                    vertexes = new double[vertexSize];
                    countfaceVertex = faceVertexSize = header.countFace;
                    faceVertexSize *= 3;
                    faceVertex = new unsigned int [faceVertexSize];
                }
                else
                {
                    header_line += line + "\n";
                }
            }
            else
            {
                if(countVertex > 0)
                {
                    unsigned int i = 0;
                    std::string value;
                    int sequence = 0;
                    while(i < line.size()
                          && line.substr(i, 1) != "\n")
                    {
                       if(line.substr(i, 1).compare(" ") == 0)
                       {
                           if(sequence < 3)
                           {
                               vertexes[count_v++] = QString(value.c_str()).toDouble();
                               sequence++;
                           }
                           value = "";
                       }
                       else
                       {
                           value += line.substr(i, 1);
                       }
                       ++i;
                    }
                    --countVertex;
                }
                else
                {
                    if(countfaceVertex > 0)
                    {
                        unsigned int i = 0;
                        string value;
                        int sequence = 0;
                        int j = 0;
                        while(i < line.size()
                              && line.substr(i, 1) != "\n")
                        {
                           if(line.substr(i, 1).compare(" ") == 0)
                           {
                               if(sequence == 0)
                               {
                                   sequence = atoi(value.c_str());
                               }
                               else
                               {
                                   if(j < sequence)
                                   {
                                       faceVertex[count++] = atoi(value.c_str());
                                       ++j;
                                   }
                               }
                               value = "";
                           }
                           else
                           {
                               value += line.substr(i, 1);
                           }
                           ++i;
                        }
                        --countfaceVertex;
                    }

                }

            }

        }
        plyModel->setVertexes(vertexes, vertexSize);
        plyModel->setFaceVertexes(faceVertex, faceVertexSize);
    }
    file.close();
    return true;
}

bool plydriver::save(Model * model, const char * filepath)
{
    return false;
}
