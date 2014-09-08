#include <iostream>
#include "CGraphImage3D.h"
#include "CGraphImage2D.h"
#include "CGraphMeshND.h"
#include "CSegmentator.h"
#include "CImage.h"
#include "CGraphUtils.h"
#include "CAppUtils.h"
using namespace std;

void do_mesh()
{
	CGraphMeshND<int> grafo_entrada, grafo_salida;
	grafo_entrada.load_data("..\\..\\Data\\meshin");
	//grafo_entrada.print_mesh();
	grafo_salida=grafo_entrada;
	CSegmentator<CGraphMeshND<int> > seg(&grafo_entrada, &grafo_salida);
	seg.binary_segmentation();
	grafo_salida.print_mesh();
}
void do_bin()
{	
	CImage imagen("..\\..\\Data\\auto.jpg");//true beacuse it is 3d
	CGraphUtils<int> utils;
	cout<<"hello"<<endl;
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
	CImage imagen("..\\..\\Data\\auto.jpg", true);//true beacuse it is 3d
	CGraphUtils<int> utils;
	CGraphImage3D<int> in, out;
	
	utils.ImageToGraph3D(&in, &imagen);
	
	out=in;
	CSegmentator<CGraphImage3D<int> > seg(&in, &out);
	seg.m_max_segmentation_difference=30;
	//seg.gray_scale();
	seg.group_neighbor_cells();
	//seg.binary_segmentation();
	//utils.GraphToImage3D(&out, &imagen);
	utils.LabeledGraphToImage3D(&in, &imagen);
	imagen.display3d();
}
int main()
{
	int op;
	while(1){
		cin>>op;
		switch (op){
		case 1:
			do_bin();
			break;
		case 2:
			do_ag();
			break;
		case 3:
			do_mesh();
			break;
		}
	}
	return 0;
}


