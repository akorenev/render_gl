#include "plydriver.h"
#include <iostream>
#include <fstream>
#include <QFile>
#include <string>
#include <list>
#include <stdlib.h>
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
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file.close();
        return false;
    }
    string header_line;
    bool read_header = false;
    Header header;
    pointsitem * item = new pointsitem;
    int countVertex = 0;
    int countfaceVertex = 0;
    int count = 0;
    int count_v = 0;
    while (!file.atEnd())
    {
        QString v(file.readLine());
        line = v.remove("\n").toStdString();
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
                countVertex = header.countVertex;
                item->m_vertexes.resize(countVertex);
                countfaceVertex = header.countFace * 3;
                item->m_indices.resize(countfaceVertex);
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
                Vertex vertex;
                while(i < line.size()
                      && line.substr(i, 1) != "\n")
                {
                    if(line.substr(i, 1).compare(" ") == 0)
                    {
                        if(sequence < 3)
                        {
                            vertex.point[sequence] = QString(value.c_str()).toFloat();
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
                vertex.color = ColorF(0.0, 1.0, 0.0, 1.0);
                item->m_vertexes[count_v++] = vertex;
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
                                   item->m_indices[count++] = QString(value.c_str()).toUShort();
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
//    pointsitem * item1 = new pointsitem;
//    item1->m_indices.resize(3);
//    item1->m_indices[0] = 0;
//    item1->m_indices[1] = 1;
//    item1->m_indices[2] = 2;

//    item1->m_vertexes.resize(3);
//    item1->m_vertexes[0] = Vertex(PointF(0.0f, 0.5f, 0.0f),
//                                  ColorF(1.0, 0.0, 0.0, 1.0));
//    item1->m_vertexes[1] = Vertex(PointF(-0.5f, -0.5f, 0.0f),
//                                  ColorF(1.0, 0.0, 0.0, 1.0));
//    item1->m_vertexes[2] = Vertex(PointF(0.5f, -0.5f, 0.0f),
//                                  ColorF(0.0, 1.0, 0.0, 1.0));

    plyModel->setModel(item);
    file.close();
    return true;
}

bool plydriver::save(Model * model, const char * filepath)
{
    return false;
}
