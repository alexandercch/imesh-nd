#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "CImg.h"
using namespace cimg_library;
using namespace std;

string int_to_str(int n)
{
    char *buffer= new char[64];
    sprintf(buffer,"%d", n );
    string res;
    res= buffer;
    delete[] buffer;
    return res;
}

class CImage
{
public:
    typedef unsigned char IType;
    CImg<IType> *m_imagen;//for 2d images
    CImg<IType> **m_imagenes;//for 3d images

    int m_height, m_width, m_layers;
    //load 2d  images
    CImage(string filename, bool is_3d=false)
    {
        if (is_3d)
        {
            int point_pos, layer_counter=0;
            string name, ext, newname, number;
            point_pos = filename.rfind('.');
            name=filename.substr(0, point_pos);
            ext= filename.substr(point_pos, filename.size());
            vector<string> file_name_vector;
            for(int i=0; i< 10000; ++i)
            {
                number="";
                layer_counter=i;
                number=  int_to_str(layer_counter);

                newname=name + number + ext;
                ifstream entrada(newname.c_str());
                if (entrada)
                {
                    file_name_vector.push_back(newname);
                    continue;
                }
                else break;
            }
            cout<<" number of files:"<<layer_counter<<endl;
            for(int i=0; i< (int)file_name_vector.size(); ++i)
            {
                cout<<file_name_vector[i]<<endl;
            }
            this->m_layers=layer_counter;
            m_imagenes = new CImg<IType>*[this->m_layers];

            for(int i=0; i< this->m_layers; ++i)
            {
                m_imagenes[i] = new CImg<IType>(file_name_vector[i].c_str());
                if (!i)//chekeamos las dimenciones de la primera imagen y l
                {
                    m_height = m_imagenes[i]->height();
                    m_width  = m_imagenes[i]->width();
                }
            }
        }
        else
        {
            m_imagen = new CImg<IType>(filename.c_str());
            m_height = m_imagen->height();
            m_width  = m_imagen->width();
        }
    };
    CImage(int height, int width)
    {
        m_imagen = new CImg<IType>(height,width,1,3);
    }
    CImage(int height, int width, int layers)
    {
        this->m_layers = layers;
        m_imagenes = new CImg<IType>*[layers];
        for(int i=0; i< layers; ++i)
            m_imagenes[i] = new CImg<IType>(height,width,1,3);
    }
    virtual ~CImage() {};
    //get 2d image pixel
    unsigned int get_pixel(int row, int column)
    {
        int r, g, b, pos, msize;
        unsigned int pixel;//msize is the the total size of pixels aka (height x widht)
        pos = m_width * row + column;
        msize = m_height * m_width;
        r = m_imagen->data()[pos];
        g = m_imagen->data()[msize + pos];
        b = m_imagen->data()[msize*2 + pos];
        pixel=255<<8;
        pixel|=r;
        pixel<<=8;
        pixel|=g;
        pixel<<=8;
        pixel|=b;
        return pixel;
    }
    //get 3d image pixel
    unsigned int get_pixel(int row, int column, int layer) //row and column form the matrix/image 2d
    {
        int r, g, b, pos, msize;//matrix size (height x widht x layers)
        unsigned int pixel;
        pos = m_width * row + column;
        msize = m_height * m_width;
        r = m_imagenes[layer]->data()[pos];
        g = m_imagenes[layer]->data()[msize + pos];
        b = m_imagenes[layer]->data()[msize*2 + pos];
        pixel=255<<8;
        pixel|=r;
        pixel<<=8;
        pixel|=g;
        pixel<<=8;
        pixel|=b;
        return pixel;
    }
    //set 2d image pixel
    void set_pixel(int row, int col,  unsigned int pixel)
    {
        unsigned char r, g, b;
        r = (unsigned char)(pixel>>16);
        g = (unsigned char)(pixel>>8);
        b = (unsigned char)(pixel);
        (*m_imagen)(col,row,0) = r;
        (*m_imagen)(col,row,1) = g;
        (*m_imagen)(col,row,2) = b;
    }
    //set 3d image pixel
    void set_pixel(int row, int col,int layer, unsigned int pixel)
    {
        unsigned char r, g, b;
        r = (unsigned char)(pixel>>16);
        g = (unsigned char)(pixel>>8);
        b = (unsigned char)(pixel);
        (*m_imagenes[layer])(col, row, 0) = r;
        (*m_imagenes[layer])(col, row, 1) = g;
        (*m_imagenes[layer])(col, row, 2) = b;
    }
    void display3d()
    {
        for(int i=0; i< m_layers; ++i)
            m_imagenes[i]->display();
    }
    void display()
    {
        m_imagen->display();
    }
protected:
private:
};
