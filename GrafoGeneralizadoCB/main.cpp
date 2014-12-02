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
string data_file="pelotas.jpg";

CAppUtils app;

void do_mesh()
{
    CGraphMeshND<int> grafo_entrada, grafo_salida;
    grafo_entrada.load_data(DATA_PATH + data_file);
    //grafo_entrada.print_mesh();
    grafo_salida=grafo_entrada;
    CSegmentator<CGraphMeshND<int> > seg(&grafo_entrada, &grafo_salida);
    seg.binary_segmentation();
    grafo_salida.print_mesh();
}
void do_bin()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    imagen.to_gray_scale();
    CGraphUtils<int> utils;
    CGraphImage2D<int> in, out;
    utils.ImageToGraph2D(&in, &imagen);
    out=in;
    CSegmentator<CGraphImage2D<int> > seg(&in, &out);
    //seg.group_neighbor_cells();
    seg.binary_segmentation();
    utils.GraphToImage2D(&out, &imagen);
    imagen.display();
}
void do_ag()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    imagen.display();
    CGraphUtils<int> utils;
    CGraphImage2D<int> in, out;
    utils.ImageToGraph2D(&in, &imagen);
    out=in;
    CSegmentator<CGraphImage2D<int> > seg(&in, &out);
    cout<<"to gray scale"<<endl;
    app.begin_counter();
    imagen.to_gray_scale();
    app.show_duration();
    cout<<endl;
    imagen.display();
    cout<<"Segmentando"<<endl;
    seg.m_max_segmentation_difference=segmentation_difference;
    //seg.gray_scale();
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();
    cout<<endl;
    //seg.show_mesh_region();
    utils.LabeledGraphToImage2D(&in, &imagen);
    imagen.display();
    seg.m_nregions = segmentation_number_of_meshes;
    //seg.show_mesh_region();
    seg.group_neighbor_regions();
    cout<<"end ASm_V"<<endl;
    //seg.show_mesh_region();
    //seg.binary_segmentation();
    //utils.GraphToImage3D(&out, &imagen);
    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
}
void do_ag2()
{
    CImage imagen(DATA_PATH + data_file);//true beacuse it is 3d
    imagen.display();
    CGraphUtils<int> utils;
    CGraphImage2D<int> in, out;
    utils.ImageToGraph2D(&in, &imagen);
    out=in;
    CSegmentator<CGraphImage2D<int> > seg(&in, &out);
    /*cout<<"to gray scale"<<endl;
    app.begin_counter();
    imagen.to_gray_scale();
    app.show_duration();
    cout<<endl;*/
    imagen.display();
    cout<<"Segmentando"<<endl;
    seg.m_max_segmentation_difference=segmentation_difference;
    //seg.gray_scale();
    app.begin_counter();
    seg.group_neighbor_cells();
    app.show_duration();
    cout<<endl;
    //seg.show_mesh_region();
    utils.LabeledGraphToImage2D(&in, &imagen);
    imagen.display();
    seg.m_nregions = segmentation_number_of_meshes;
    //seg.show_mesh_region();
    seg.group_neighbor_regions();
    cout<<"end ASm_V"<<endl;
    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
    seg.group_similar_regions();
    cout<<"end ASm_S"<<endl;

    //seg.show_mesh_region();
    //seg.binary_segmentation();
    //utils.GraphToImage3D(&out, &imagen);
    utils.OverlapedGraphToImage2D(&in,&(seg.m_meshregionV),  &imagen);
    imagen.display();
}
int main()
{
    //freopen("out.txt", "w", stdout);
    freopen("in.txt", "r", stdin);

    int op;
    while(1)
    {
        cin>>data_file;
        cin>>op;
        cin>>segmentation_difference;
        cin>>segmentation_number_of_meshes;
        switch (op)
        {
        case 1:
            do_bin();
            break;
        case 2:
            do_ag2();
            break;
        case 3:
            do_mesh();
            break;
        }
        //cin.get();
    }
    return 0;
}
