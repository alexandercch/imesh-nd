/**
    Seminario de Tesis 10
    main.cpp
    Purpose: This file use the Generalized Graph and and the Imesh Algorithm
             to perform the image segmentation over the 4 context of structures
             to know images 2D, 3D, and meshes 2D, 3D.

    @author Alex Ccacyahuillca
    @version 1.0 04/015
*/
#include <iostream>
#include "CGraphImage3D.h"
#include "CGraphImage2D.h"
#include "CGraphMeshND.h"
#include "CSegmentator.h"
#include "CImage.h"
#include "CGraphUtils.h"
#include "CAppUtils.h"

using namespace std;

//some config vars
const string DATA_PATH ="..\\Data\\";

int segmentation_difference;
int segmentation_number_of_meshes;

//this is the current file we are working with
string data_file="pelotas.jpg";

CAppUtils app;
CGraphUtils<int> utils;

//void do_mesh();
void do_bin();
void do_ag1();
void do_ag2();
void do_ag3();

int main2()
{
    //freopen("out.txt", "w", stdout);
    freopen("in.txt", "r", stdin);

    int op;
    while(cin>>data_file, data_file!="")
    {
        cout<<data_file<<endl;
        //cin>>data_file;
        cin>>op;
        cin>>segmentation_difference;
        cin>>segmentation_number_of_meshes;
        switch (op)
        {

        case 1:
            do_ag1();
            break;
        case 2:
            do_ag2();
            break;
        case 3:
            do_ag3();
            break;
        /*case 4:
            do_bin();
            break;*/
        }
        data_file="";
    }
    return 0;
}

void do_ag1()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    imagen.display();

    CGraphImage2D<int> in, out;
    utils.ImageToGraph2D(&in, &imagen);

    CSegmentator<CGraphImage2D<int> > seg(&in, &out);

    cout<<"to gray scale"<<endl;
    app.begin_counter();
    imagen.to_gray_scale();
    app.show_duration();

    imagen.display();

    cout<<"Segmentando"<<endl;
    seg.m_max_segmentation_difference=segmentation_difference;

    cout<<"ACV"<<endl;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();
    utils.LabeledGraphToImage2D(&in, &imagen);

    imagen.display();

    cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.m_nregions = segmentation_number_of_meshes;
    seg.group_neighbor_regions();
    app.show_duration();

    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
}
void do_ag2()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    imagen.display();
    CGraphImage2D<int> in, out;
    utils.ImageToGraph2D(&in, &imagen);
    CSegmentator<CGraphImage2D<int> > seg(&in, &out);

    cout<<"Segmentando"<<endl;
    cout<<"ACV"<<endl;
    seg.m_max_segmentation_difference=segmentation_difference;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();

    utils.LabeledGraphToImage2D(&in, &imagen);
    imagen.display();

    seg.m_nregions = segmentation_number_of_meshes;

    cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.group_neighbor_regions();
    app.show_duration();

    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();

    cout<<"ASm_S"<<endl;
    app.begin_counter();
    seg.group_similar_regions();
    app.show_duration();

    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
}
void do_ag3()
{
    cout<<"loading data"<<endl;
    app.begin_counter();
    CImage imagen(DATA_PATH + data_file, true);//true beacuse it is 3d
    app.show_duration();
    //imagen.display3d();// too long

    CGraphImage3D<int> in, out;
    cout<<"img to graph"<<endl;
    app.begin_counter();
    utils.ImageToGraph3D(&in, &imagen);
    app.show_duration();

    CSegmentator<CGraphImage3D<int> > seg(&in, &out);

    cout<<"Segmentando"<<endl;
    seg.m_max_segmentation_difference=segmentation_difference;

    cout<<"ACV"<<endl;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();
    cout<<"labeled to img"<<endl;
    app.begin_counter();
    utils.LabeledGraphToImage3D(&in, &imagen);
    app.show_duration();

    imagen.display3d();

    /*cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.m_nregions = segmentation_number_of_meshes;
    seg.group_neighbor_regions();
    app.show_duration();

    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();*/
}

/*void do_mesh()
{
    CGraphMeshND<int> grafo_entrada, grafo_salida;
    grafo_entrada.load_data(DATA_PATH + data_file);
    grafo_salida=grafo_entrada;
    CSegmentator<CGraphMeshND<int> > seg(&grafo_entrada, &grafo_salida);
    seg.binary_segmentation();
    grafo_salida.print_mesh();
}*/
void do_bin()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    //imagen.to_gray_scale();
    CGraphUtils<int> utils;
    CGraphImage2D<int> in, out;
    utils.ImageToGraph2D(&in, &imagen);
    //out=in;
    CSegmentator<CGraphImage2D<int> > seg(&in, &out);
    //seg.group_neighbor_cells();
    seg.binary_segmentation();
    utils.GraphToImage2D(&in, &imagen);
    imagen.display();
}
