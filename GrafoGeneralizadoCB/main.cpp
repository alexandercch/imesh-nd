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
int nmeshes_asmv;
int nmeshes_asms;

//this is the current file we are working with
string data_file="pelotas.jpg";

CAppUtils app;
CGraphUtils<int> utils;

//void do_mesh();
void do_bin();
void do_ag1();
void do_ag2();
void do_ag3();
void do_mesh1();
void do_mesh2();

int main()
{
    //freopen("out.txt", "w", stdout);
    /**
        input file format:
        filename
        number of operation
        segmentation differente
        number of meshes for asmv
        number of meshes for asms
    */
    freopen("in.txt", "r", stdin);
    freopen("cimgelog.txt", "w", stderr);


    int op;
    while(cin>>data_file, data_file!="")
    {
        cin>>op;
        cin>>segmentation_difference;
        cin>>nmeshes_asmv;
        cin>>nmeshes_asmv;

        //system("cls");
        cout<<"op:"<<op<<endl
            <<"seg diff:"<<segmentation_difference<<endl
            <<"# of meshes asmv:"<<nmeshes_asmv<<endl
            <<"# of meshes asms:"<<nmeshes_asms<<endl;

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
        case 4:
            do_mesh1();
            break;
        case 5:
            do_mesh2();
            break;
        }
        data_file="";
    }
    return 0;
}

void do_mesh1()
{
    cout<<"Testing mesh segmentation:"<<endl;
    cout<<"File:"<<DATA_PATH + data_file<<endl;
    CGraphMeshND<int> in, out;

    cout<<"loading..."<<endl;
    app.begin_counter();
    utils.AdjacentListToMesh(&in, DATA_PATH + data_file);
    app.show_duration();

    CSegmentator<CGraphMeshND<int> > seg(&in, &out);

    cout<<"Begin segmentation:"<<endl;
    app.begin_counter();
    seg.binary_segmentation();
    app.show_duration();
    cout<<"End Segmentation."<<endl<<endl;

    cout<<"Saving file:"<<endl;
    app.begin_counter();
    utils.MeshToAdjacentList(&in,DATA_PATH + data_file + "out");
    app.show_duration();
    cout<<"end saving."<<endl<<endl;
    //grafo_salida.print_mesh();
}

void do_mesh2()
{
    cout<<"Testing mesh segmentation:"<<endl;
    cout<<"File:"<<DATA_PATH + data_file<<endl;
    CGraphMeshND<int> in, out;

    cout<<"loading..."<<endl;
    app.begin_counter();
    utils.AdjacentListToMesh(&in, DATA_PATH + data_file);
    app.show_duration();

    CSegmentator<CGraphMeshND<int> > seg(&in, &out);
    seg.m_segmentation_difference=segmentation_difference;
    seg.m_nregions_asmv = nmeshes_asmv;
    seg.m_nregions_asms = nmeshes_asms;

    cout<<"Segmentando"<<endl;
    cout<<"ACV"<<endl;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();


    cout<<"Labeled Mesh to Graph"<<endl;
    app.begin_counter();
    utils.LabeledMeshToNDGraph(&in, "..\\resultados\\labeledgraph.txt");
    app.show_duration();
    //cout<<"Labeled Mesh to Graph"<<endl;

    cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.group_neighbor_regions();
    app.show_duration();

    cout<<"Saving file:"<<endl;
    app.begin_counter();
    utils.OverlapedNDGraphToFile(&in,&(seg.m_meshregionV), "..\\resultados\\overlapedgraph.txt");
    app.show_duration();
    cout<<"end saving."<<endl<<endl;


    //grafo_salida.print_mesh();
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
    seg.m_segmentation_difference=segmentation_difference;
    seg.m_nregions_asmv = nmeshes_asmv;
    seg.m_nregions_asms = nmeshes_asms;

    cout<<"ACV"<<endl;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();
    utils.LabeledGraphToImage2D(&in, &imagen);

    imagen.display();

    cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.group_neighbor_regions();
    app.show_duration();

    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
}
void do_ag2()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    cout<<"begin ag2 width file:"<<data_file<<endl;
    imagen.display();

    CGraphImage2D<int> in, out;

    utils.ImageToGraph2D(&in, &imagen);
    CSegmentator<CGraphImage2D<int> > seg(&in, &out);
    seg.m_segmentation_difference=segmentation_difference;
    seg.m_nregions_asmv = nmeshes_asmv;
    seg.m_nregions_asms = nmeshes_asms;

    cout<<"Segmentando"<<endl;
    cout<<"ACV"<<endl;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();

    utils.LabeledGraphToImage2D(&in, &imagen);
    imagen.display();

    cout<<"# de regiones:"<<seg.m_meshregionV.size()<<endl<<endl;
    cout<<"# de regiones a converger asmv:"<<nmeshes_asmv<<endl;
    cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.group_neighbor_regions();
    app.show_duration();

    cout<<"# de regiones convergidas:"<<endl;
    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();

    cout<<"ASm_S"<<endl;
    app.begin_counter();
    seg.group_similar_regions();
    app.show_duration();

    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
    cout<<"end processing width file:"<<data_file<<endl;
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
    seg.m_segmentation_difference=segmentation_difference;
    seg.m_nregions_asmv = nmeshes_asmv;
    seg.m_nregions_asms = nmeshes_asms;

    cout<<"Segmentando"<<endl;
    cout<<"ACV"<<endl;
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();

    cout<<"labeled to img"<<endl;
    app.begin_counter();
    utils.LabeledGraphToImage3D(&in, &imagen);
    app.show_duration();

    cout<<"ASm_V"<<endl;
    app.begin_counter();
    seg.group_neighbor_regions();
    app.show_duration();

    cout<<"overlaped to img"<<endl;
    app.begin_counter();
    utils.OverlapedGraphToImage3D(&in,&(seg.m_meshregionV),  &imagen);
    app.show_duration();
    //imagen.display3d();
    utils.Overlaped3DGraphToFile(&in,&(seg.m_meshregionV), &imagen, "..\\resultados\\3ddragon.txt");

}


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
