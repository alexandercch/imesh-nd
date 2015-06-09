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

    void LabeledGraphToImage2D(CGraphImage2D<T> *graph, CImage *image);
    void LabeledGraphToImage3D(CGraphImage3D<T> *graph, CImage *image);

    void OverlapedGraphToImage2D(CGraphImage2D<T> *graph, vector<CMeshRegion<CGraphImage2D<T> >* > *p_mrv,  CImage *image);
    void OverlapedGraphToImage3D(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,  CImage *image);

    void Region_DFS2D(int v, int color);
    void Region_DFS3D(int v, int color);

    void OverlapedGraphToFile(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,CImage *image, string filename);


    CColor color_pallete;

    map<int , int> m_label_color_map;
    vector<CMeshRegion<CGraphImage2D<T> >* > *m_p_mrv2d;
    vector<CMeshRegion<CGraphImage3D<T> >* > *m_p_mrv3d;
    void p(CGraphImage3D<T> *graph);
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

    /*interresting :P
    int r=graph->m_rows-1, c=graph->m_cols-1, l=graph->m_lays-1;
    for(int i=0; i< graph->m_rows; ++i)
        graph->m_matriz[i][0][0].m_visited=
        graph->m_matriz[i][c][0].m_visited=
        graph->m_matriz[i][0][l].m_visited=
        graph->m_matriz[i][c][l].m_visited=true;
     for(int i=0; i< graph->m_cols; ++i)
        graph->m_matriz[0][i][0].m_visited=
        graph->m_matriz[r][i][0].m_visited=
        graph->m_matriz[0][i][l].m_visited=
        graph->m_matriz[r][i][l].m_visited=true;
    for(int i=0; i< graph->m_lays; ++i)
        graph->m_matriz[0][0][i].m_visited=
        graph->m_matriz[r][0][i].m_visited=
        graph->m_matriz[0][c][i].m_visited=
        graph->m_matriz[r][c][i].m_visited=true;*/
    cout<<"set of image colors:"<<endl;
    for(set<int>::iterator iter= colorset.begin();
            iter!= colorset.end(); iter++)
        printf("%x ", *iter);
    cout<<endl;

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
    /*for(int i=0; i< graph->m_rows; ++i)
        graph->m_matriz[i][0][0].m_visited=
        graph->m_matriz[i][c][0].m_visited=
        graph->m_matriz[i][0][l].m_visited=
        graph->m_matriz[i][c][l].m_visited=true;
     for(int i=0; i< graph->m_cols; ++i)
        graph->m_matriz[0][i][0].m_visited=
        graph->m_matriz[r][i][0].m_visited=
        graph->m_matriz[0][i][l].m_visited=
        graph->m_matriz[r][i][l].m_visited=true;
    for(int i=0; i< graph->m_lays; ++i)
        graph->m_matriz[0][0][i].m_visited=
        graph->m_matriz[r][0][i].m_visited=
        graph->m_matriz[0][c][i].m_visited=
        graph->m_matriz[r][c][i].m_visited=true;
    */
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
            //recorrer los hijos
            //cout<<"mr: "<<i<<endl;
            Region_DFS2D((*p_mrv)[i]->m_label, main_region_color);
        }
    }
    /*cout<<"xD"<<endl;
    cout<<"colors for mesh region"<<endl;
    cout<<"\tlbl\tcolor"<<endl;
    for(map<int, int>::iterator iter = m_label_color_map.begin();
        iter!=m_label_color_map.end(); ++iter)
        cout<<"\t"<<iter->first<<"\t"<<iter->second<<endl;*/

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
void CGraphUtils<T>::OverlapedGraphToFile(CGraphImage3D<T> *graph, vector<CMeshRegion<CGraphImage3D<T> >* > *p_mrv,CImage *image, string filename){
    ofstream out(filename.c_str());
    m_p_mrv3d = p_mrv;
    int main_region_color;
    //cout<<":3"<<endl;
    int region_counter=0;
    for(int i=0; i< p_mrv->size(); ++i)
    {
        if (!(*p_mrv)[i]->m_overlap)
        {
            Region_DFS3D((*p_mrv)[i]->m_label, region_counter++);
        }
    }

    out<<image->m_height<<" "<<image->m_width<<" "<<image->m_layers<<endl;

    for(int i=0; i< image->m_height; ++i){
        for(int j=0; j< image->m_width; ++j){
            for(int k=0; k < image->m_layers; ++k)
                out<<m_label_color_map[(int)graph->m_matriz[i+1][j+1][k+1].m_label]<<" ";
            out<<endl;
        }
        out<<endl;
    }
    out.close();
}
