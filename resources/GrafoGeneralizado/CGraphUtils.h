#pragma once
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
	CColor color_pallete;
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
	cout<<"hello i2g:"<<image->m_height<<" "<<image->m_width<<" "<<image->m_layers<<endl;
    graph->config(image->m_height+2, image->m_width+2, image->m_layers+2);
	
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
            image->set_pixel(i, j,color_pallete((int)graph->m_matriz[i+1][j+1].m_flag));
}
template<class T>//image must be already configured height x width
void CGraphUtils<T>::LabeledGraphToImage3D(CGraphImage3D<T> *graph, CImage *image)
{
    for(int i=0; i< image->m_height; ++i)
        for(int j=0; j< image->m_width; ++j)
            for(int k=0; k< image->m_layers; ++k)
                image->set_pixel(i, j, k,color_pallete((int)graph->m_matriz[i+1][j+1][k+1].m_flag));
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