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
    //void OverlapedGraphToImage3D(CGraphImage3D<T> *graph, CImage *image);

    void Region_DFS(int v, int color);
    CColor color_pallete;

    map<int , int> m_label_color_map;
    vector<CMeshRegion<CGraphImage2D<T> >* > *m_p_mrv;
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
        for(int j=0; j< graph->m_cols; ++j)
            graph->m_matriz[i][j].m_visited=true;

    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
        {
            graph->m_matriz[i+1][j+1].m_data= image->get_pixel(i, j);
            graph->m_matriz[i+1][j+1].m_visited=false;
        }
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
    for(int i=0; i< graph->m_rows; ++i)
        for(int j=0; j< graph->m_cols; ++j)
            for(int k=0; k< graph->m_lays; ++k)
                graph->m_matriz[i][j][k].m_visited=true;

    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            for(int k=0; k< image->m_layers; ++k)
            {
                graph->m_matriz[i+1][j+1][k+1].m_data= image->get_pixel(i, j, k);
                graph->m_matriz[i+1][j+1][k+1].m_visited=false;
                colorset.insert(image->get_pixel(i, j, k));
            }
    /*int r=graph->m_rows-1, c=graph->m_cols-1, l=graph->m_lays-1;
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
void CGraphUtils<T>::Region_DFS(int v, int color){
    //cout<<"RDFS:"<<v<<"-"<<color<<endl;
    m_label_color_map[(*m_p_mrv)[v]->m_label] = color;
    for(int i=0; i < (*m_p_mrv)[v]->m_overlaped_mr_ids.size(); ++i)
        Region_DFS((*m_p_mrv)[v]->m_overlaped_mr_ids[i], color);
}



template<class T>//image must be already configured height x width
void CGraphUtils<T>::OverlapedGraphToImage2D(CGraphImage2D<T> *graph, vector<CMeshRegion<CGraphImage2D<T> >* > *p_mrv, CImage *image)
{
    m_p_mrv = p_mrv;
    int main_region_color;
    cout<<":3"<<endl;
    for(int i=0; i< p_mrv->size(); ++i){
        if (!(*p_mrv)[i]->m_overlap){
            main_region_color=color_pallete((*p_mrv)[i]->m_label);
            //recorrer los hijos
            cout<<"mr: "<<i<<endl;
            Region_DFS((*p_mrv)[i]->m_label, main_region_color);
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




//old methods
/*template<class T>//image must be already configured height x width
void CGraphUtils<T>::GraphToImage2D(CGraphImage2D<T> *graph, CImage *image)
{
    for(int i=0; i< graph->m_rows; ++i)
        for(int j=0; j< graph->m_cols; ++j)
            image->set_pixel(i, j,(int)graph->m_matriz[i][j].m_data);
}

template<class T>
void CGraphUtils<T>::ImageToGraph2D(CGraphImage2D<T> *graph, CImage *image)
{
    graph->config(image->m_height, image->m_width);
    for(int i=0; i< graph->m_rows; ++i)
        for(int j=0; j< graph->m_cols; ++j)
            graph->m_matriz[i][j].m_data =(T) image->get_pixel(i, j);
}*/
