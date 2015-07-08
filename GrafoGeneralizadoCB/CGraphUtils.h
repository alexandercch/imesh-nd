#pragma once
#include <cstdio>
#include <set>
#include "CImage.h"


using namespace std;

template<class T>
class CGraphUtils
{
public:
    CGraphUtils();
    virtual ~CGraphUtils();

    void GraphToImage2D(CGraphImage2D<T> *graph, CImage *image);
    void ImageToGraph2D(CGraphImage2D<T> *graph, CImage *image);
    void GraphToImage3D(CGraphImage3D<T> *graph, CImage *image);
    void ImageToGraph3D(CGraphImage3D<T> *graph, CImage *image);

    void AdjacentListToMesh(CGraphMeshND<T> *graph, string filename);
    void MeshToAdjacentList(CGraphMeshND<T> *graph, string filename);


    void LabeledGraphToImage2D(CGraphImage2D<T> *graph, CImage *image);
    void LabeledGraphToImage3D(CGraphImage3D<T> *graph, CImage *image);

    void LabeledMeshToNDGraph(CGraphMeshND<T> *graph, string filename);


    void OverlapedGraphToImage2D(CGraphImage2D<T> *graph, vector<CMeshRegion<CGraphImage2D<T> >* > *p_mrv,  CImage *image);
    void OverlapedGraphToImage3D(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,  CImage *image);

    void Region_DFS2D(int v, int color);
    void Region_DFS3D(int v, int color);
    void Region_DFSND(int v, int color);

    void Overlaped3DGraphToFile(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,CImage *image, string filename);
    void OverlapedNDGraphToFile(CGraphMeshND<T> *graph, vector<CMeshRegion<CGraphMeshND<T> >* > *p_mrv, string filename);


    CColor color_pallete;

    map<int , int> m_label_color_map;
    vector<CMeshRegion<CGraphImage2D<T> >* > *m_p_mrv2d;
    vector<CMeshRegion<CGraphImage3D<T> >* > *m_p_mrv3d;
    vector<CMeshRegion<CGraphMeshND<T> >* > *m_p_mrvnd;

    void p(CGraphImage3D<T> *graph);
    int *neighbors_per_cell;
protected:
private:
};

template<class T>
CGraphUtils<T>::CGraphUtils()
{
    //ctor
}

template<class T>
CGraphUtils<T>::~CGraphUtils()
{
    //dtor
}

template<class T>//image must be already configured height x width
void CGraphUtils<T>::GraphToImage2D(CGraphImage2D<T> *graph, CImage *image)
{
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            image->set_pixel(i, j,(int)graph->m_matriz[i+1][j+1].m_data);
}

template<class T>
void CGraphUtils<T>::ImageToGraph2D(CGraphImage2D<T> *graph, CImage *image)
{
    graph->config(image->m_height+2, image->m_width+2);

    for(int i=0; i< graph->m_rows; ++i)
    {
        graph->m_matriz[i][0].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
        graph->m_matriz[i][graph->m_cols - 1].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
    }

    for(int j=0; j< graph->m_cols; ++j)
    {
        graph->m_matriz[0][j].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
        graph->m_matriz[graph->m_rows - 1][j].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
    }
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            graph->m_matriz[i+1][j+1].m_data = image->get_pixel(i, j);
}

template<class T>//image must be already configured height x width
void CGraphUtils<T>::GraphToImage3D(CGraphImage3D<T> *graph, CImage *image)
{
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            for(int k=0; k< image->m_layers; ++k)
                image->set_pixel(i, j, k,(int)graph->m_matriz[i+1][j+1][k+1].m_data);
}

template<class T>
void CGraphUtils<T>::ImageToGraph3D(CGraphImage3D<T> *graph, CImage *image)
{
    graph->config(image->m_height+2, image->m_width+2, image->m_layers+2);

    set<int> colorset;
    cout<<"i h, w, l:"<<image->m_height<<" "<<image->m_width<<" "<<image->m_layers<<endl;
    cout<<"config"<<endl;
    for(int i=0; i< graph->m_rows; ++i)
    {
        for(int j=0; j< graph->m_cols; ++j)
        {
            graph->m_matriz[i][j][0].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
            graph->m_matriz[i][j][graph->m_lays - 1].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
        }
        for(int k=0; k< graph->m_lays; ++k)
        {
            graph->m_matriz[i][0][k].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
            graph->m_matriz[i][graph->m_cols - 1][k].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
        }
    }
    cout<<"sides"<<endl;

    for(int j=0; j< graph->m_cols; ++j)
        for(int k=0; k< graph->m_lays; ++k)
        {
            graph->m_matriz[0][j][k].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
            graph->m_matriz[graph->m_rows - 1][j][k].m_label = INVALID_NEIGHBOR_LABEL_VALUE;
        }
    cout<<"top & bottom"<<endl;

    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            for(int k=0; k< image->m_layers; ++k)
            {
                //cout<<i<<":"<<j<<":"<<k<<endl;
                graph->m_matriz[i+1][j+1][k+1].m_data= image->get_pixel(i, j, k);
                colorset.insert(image->get_pixel(i, j, k));
            }
    cout<<"data"<<endl;

    cout<<"set of image colors:"<<endl;
    for(set<int>::iterator iter= colorset.begin();
            iter!= colorset.end(); iter++)
        printf("%x ", *iter);
    cout<<endl;
}

template<class T>
void CGraphUtils<T>::AdjacentListToMesh(CGraphMeshND<T> *graph, string filename)
{
    ifstream in(filename.c_str());

    int size, dimension, data, neighbor, number_of_neighbors;
    float area;
    in>>size>>dimension;
    graph->config(size, dimension);
    neighbors_per_cell=new int[size];

    for(int i=0; i< size; ++i)
    {
        in>>number_of_neighbors;
        neighbors_per_cell[i] = number_of_neighbors;
        for(int j = 0; j < number_of_neighbors; ++j)
        {
            in>>neighbor;
            graph->m_adj_list[i][j] = neighbor;
        }
        for(int j = number_of_neighbors; j < dimension + 1; ++j)
        {
            ///points to the outer face of the mesh
            ///see CGraphMeshND.config method
            graph->m_adj_list[i][j] = size;
        }
        in>>data>>area;
        graph->m_nodes[i].m_data = data;
        graph->m_nodes[i].m_area = area;
    }
}

template<class T>
void CGraphUtils<T>::MeshToAdjacentList(CGraphMeshND<T> *graph, string filename)
{
    ofstream out(filename.c_str());
    out<<graph->m_size<<" "<<graph->m_number_of_neighbors - 1<<endl;
    for(int i=0; i < graph->m_size; ++i)
    {
        out << neighbors_per_cell[i];
        int cnt=0;
        for(int j = 0; j < graph->m_number_of_neighbors ; ++j)
        {
            if (graph->m_adj_list[i][j]!=graph->m_size)
            {
                out<<" "<<graph->m_adj_list[i][j];
                cnt++;
                //cout<<"FIND BORDER CELL!!"<<endl;
            }
        }
        out <<" "<<graph->m_nodes[i].m_data<<endl;

        if (cnt!=neighbors_per_cell[i])
            cout<<"ALERT!, different number of neighbors input- output :P"<<endl;
    }
    out.close();
    delete[] neighbors_per_cell;
}

template<class T>
void CGraphUtils<T>::p(CGraphImage3D<T> *graph)
{
    int n=5;
    graph->config(n+2, n+2, n+2);
    for(int i=0; i< graph->m_rows; ++i)
        for(int j=0; j< graph->m_cols; ++j)
            for(int k=0; k< graph->m_lays; ++k)
                graph->m_matriz[i][j][k].m_visited=true;
    for(int i=0; i< n; ++i)
        for(int j=0; j< n; ++j)
            for(int k=0; k< n; ++k)
                graph->m_matriz[i+1][j+1][k+1].m_visited=false;
    int r=graph->m_rows-1, c=graph->m_cols-1, l=graph->m_lays-1;

    for(int i=0; i< graph->m_rows; ++i)
    {
        for(int j=0; j< graph->m_cols; ++j)
        {
            for(int k=0; k< graph->m_lays; ++k)
                cout<<graph->m_matriz[i][j][k].m_visited;
            cout<<endl;
        }
        cout<<endl;
    }
}



template<class T>//image must be already configured height x width
void CGraphUtils<T>::LabeledGraphToImage2D(CGraphImage2D<T> *graph, CImage *image)
{
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            image->set_pixel(i, j,color_pallete((int)graph->m_matriz[i+1][j+1].m_label));
}

template<class T>//image must be already configured height x width
void CGraphUtils<T>::LabeledGraphToImage3D(CGraphImage3D<T> *graph, CImage *image)
{
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            for(int k=0; k< image->m_layers; ++k)
                image->set_pixel(i, j, k,color_pallete((int)graph->m_matriz[i+1][j+1][k+1].m_label));
}

template<class T>//we dump the labeled mesh to the graph file
void CGraphUtils<T>::LabeledMeshToNDGraph(CGraphMeshND<T> *graph, string filename){
    ofstream out(filename.c_str());
    out<<graph->m_size<<" "<<graph->m_number_of_neighbors - 1<<endl;
    for(int i=0; i < graph->m_size; ++i)
    {
        out << neighbors_per_cell[i];
        int cnt = 0;
        for(int j = 0; j < graph->m_number_of_neighbors ; ++j)
        {
            if (graph->m_adj_list[i][j] != graph->m_size)
            {
                out<<" "<<graph->m_adj_list[i][j];
                cnt++;
                //cout<<"FIND BORDER CELL!!"<<endl;
            }
        }
        out <<" "<<color_pallete(graph->m_nodes[i].m_label)<<endl;

        if (cnt!=neighbors_per_cell[i])
            cout<<"ALERT!, different number of neighbors input- output :P"<<endl;
    }
    out.close();
    //delete[] neighbors_per_cell;
}


template<class T>//image must be already configured height x width
void CGraphUtils<T>::Region_DFS2D(int v, int color)
{
    //cout<<"RDFS:"<<v<<"-"<<color<<endl;
    m_label_color_map[(*m_p_mrv2d)[v]->m_label] = color;
    for(int i=0; i < (*m_p_mrv2d)[v]->m_overlaped_mr_ids.size(); ++i)
        Region_DFS2D((*m_p_mrv2d)[v]->m_overlaped_mr_ids[i], color);
}



template<class T>//image must be already configured height x width
void CGraphUtils<T>::OverlapedGraphToImage2D(CGraphImage2D<T> *graph, vector<CMeshRegion<CGraphImage2D<T> >* > *p_mrv, CImage *image)
{
    m_p_mrv2d = p_mrv;
    int main_region_color;
    //cout<<":3"<<endl;
    for(int i=0; i< p_mrv->size(); ++i)
    {
        if (!(*p_mrv)[i]->m_overlap)
        {
            main_region_color=color_pallete((*p_mrv)[i]->m_label);
            Region_DFS2D((*p_mrv)[i]->m_label, main_region_color);
        }
    }

    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            image->set_pixel(i, j,m_label_color_map[(int)graph->m_matriz[i+1][j+1].m_label]);
}



template<class T>//image must be already configured height x width
void CGraphUtils<T>::Region_DFS3D(int v, int color)
{
    //cout<<"RDFS:"<<v<<"-"<<color<<endl;
    m_label_color_map[(*m_p_mrv3d)[v]->m_label] = color;
    for(int i=0; i < (*m_p_mrv3d)[v]->m_overlaped_mr_ids.size(); ++i)
        Region_DFS3D((*m_p_mrv3d)[v]->m_overlaped_mr_ids[i], color);
}


template<class T>//image must be already configured height x width
void CGraphUtils<T>::OverlapedGraphToImage3D(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,  CImage *image)
{
    m_p_mrv3d = p_mrv;
    int main_region_color;
    //cout<<":3"<<endl;
    for(int i=0; i< p_mrv->size(); ++i)
    {
        if (!(*p_mrv)[i]->m_overlap)
        {
            main_region_color=color_pallete((*p_mrv)[i]->m_label);
            //recorrer los hijos
            Region_DFS3D((*p_mrv)[i]->m_label, main_region_color);
        }
    }
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            for(int k=0; k < image->m_layers; ++k)
                image->set_pixel(i, j, k,m_label_color_map[(int)graph->m_matriz[i+1][j+1][k+1].m_label]);
}

template<class T>//image must be already configured height x width
void CGraphUtils<T>::Overlaped3DGraphToFile(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,CImage *image, string filename)
{
    ofstream out(filename.c_str());
    m_p_mrv3d = p_mrv;
    int main_region_color;
    //cout<<":3"<<endl;
    int region_counter=0;
    for(int i=0; i< p_mrv->size(); ++i)
    {
        if (!(*p_mrv)[i]->m_overlap)
        {
            Region_DFS3D((*p_mrv)[i]->m_label, region_counter++);//instead of give a color we just count the final regions
        }
    }

    out<<image->m_height<<" "<<image->m_width<<" "<<image->m_layers<<endl;

    for(int i=0; i< image->m_height; ++i)
    {
        for(int j=0; j< image->m_width; ++j)
        {
            for(int k=0; k < image->m_layers; ++k)
                out<<m_label_color_map[(int)graph->m_matriz[i+1][j+1][k+1].m_label]<<" ";
            out<<endl;
        }
        out<<endl;
    }
    out.close();
}

template<class T>//image must be already configured height x width
void CGraphUtils<T>::Region_DFSND(int v, int color)
{
    //cout<<"RDFS:"<<v<<"-"<<color<<endl;
    m_label_color_map[(*m_p_mrvnd)[v]->m_label] = color;
    for(int i=0; i < (*m_p_mrvnd)[v]->m_overlaped_mr_ids.size(); ++i)
        Region_DFSND((*m_p_mrvnd)[v]->m_overlaped_mr_ids[i], color);
}

template<class T>//
void CGraphUtils<T>::OverlapedNDGraphToFile(CGraphMeshND<T> *graph, vector<CMeshRegion<CGraphMeshND<T> >* > *p_mrv, string filename)
{
    ofstream out(filename.c_str());
    m_p_mrvnd = p_mrv;
    int main_region_color;
    //cout<<":3"<<endl;
    int mesh_region_color=0;
    for(int i=0; i< p_mrv->size(); ++i)
    {
        if (!(*p_mrv)[i]->m_overlap)
        {
            mesh_region_color = color_pallete((*p_mrv)[i]->m_label);
            Region_DFSND((*p_mrv)[i]->m_label, mesh_region_color );
        }
    }

    out<<graph->m_size<<" "<<graph->m_number_of_neighbors - 1<<endl;

    for(int i=0; i < graph->m_size; ++i)
    {
        out << neighbors_per_cell[i];
        int cnt=0;
        for(int j = 0; j < graph->m_number_of_neighbors ; ++j)
        {
            if (graph->m_adj_list[i][j]!=graph->m_size)
            {
                out<<" "<<graph->m_adj_list[i][j];
                cnt++;
                //cout<<"FIND BORDER CELL!!"<<endl;
            }
        }
        out <<" "<<m_label_color_map[graph->m_nodes[i].m_label]<<endl;

        if (cnt!=neighbors_per_cell[i])
            cout<<"ALERT!, different number of neighbors input- output :P"<<endl;
    }
    out.close();
    delete[] neighbors_per_cell;
}






