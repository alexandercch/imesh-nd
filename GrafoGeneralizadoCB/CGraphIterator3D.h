#ifndef CGRAPHITERATOR3D_H
#define CGRAPHITERATOR3D_H

#include "CGraphIterator.h"

//this arrays are for the navigation through neigbours around a cell
int row_step3d[6]= {1,-1, 0, 0, 0, 0};
int col_step3d[6]= {0, 0, 1,-1, 0, 0};
int lay_step3d[6]= {0, 0, 0, 0, 1,-1};
#define RS3D row_step3d
#define CS3D col_step3d
#define LS3D lay_step3d

template<class T>
class CGraphImage3D;

template< class T>
class CGraphIterator3D:public CGraphIterator<T>
{
public:
    typedef typename CGraphImage3D<T>::node node;
    CGraphIterator3D();
    virtual ~CGraphIterator3D();

    //members
    CGraphImage3D<T> *m_pgraph;//pointo to graph we are navigating to
    int m_row, m_col, m_lay;   //holds position of the iterator
    bool m_end;//flag to check wether it has reach the end

    //methods
    bool end();
    void neighbor(CGraphIterator3D<T>* iter, int i);
    node *self();

    //overloaded operators
    void operator=(CGraphImage3D<T> *graph);
    void operator=(CGraphIterator3D<T> *iter);
    void operator++(int);
    node* operator->();
protected:
private:
};

template< class T>
CGraphIterator3D<T>::CGraphIterator3D()
{
    m_end= false;
};
template< class T>
CGraphIterator3D<T>::~CGraphIterator3D()
{
};

template< class T>
void CGraphIterator3D<T>::operator=(CGraphImage3D<T> *graph)
{
    m_pgraph= graph;
    m_row=m_col=m_lay=1;
    graph->m_matriz[1][1][1].m_visited=true;
};
template< class T>
void CGraphIterator3D<T>::operator=(CGraphIterator3D<T> *iter)
{
    m_pgraph=iter->m_pgraph;
    m_row=iter->m_row;
    m_col=iter->m_col;
    m_lay=iter->m_lay;
};

template< class T>
void CGraphIterator3D<T>::operator++(int)
{
    int i=0;
    for(i=0; i < m_pgraph->m_number_of_neighbors; ++i)
    {
        if (m_pgraph->m_matriz[m_row + RS3D[i]][m_col + CS3D[i]][m_lay + LS3D[i]].m_visited)
            continue;
        m_pgraph->m_matriz[m_row + RS3D[i]][m_col + CS3D[i]][m_lay + LS3D[i]].m_visited = true;

        m_row+=RS3D[i];
        m_col+=CS3D[i];
        m_lay+=LS3D[i];
        break;
    }
    //if all neighbors has been visited and not unvisited were found, we have reach the end of the matrix
    if (i == m_pgraph->m_number_of_neighbors)
        m_end=true;//in order to stop bucle who is iterating
};

template< class T>
bool CGraphIterator3D<T>::end()
{
    return m_end;
};


template< class T>
typename CGraphIterator3D<T>::node* CGraphIterator3D<T>::operator->()
{
    return &m_pgraph->m_matriz[m_row][m_col][m_lay];
};
template< class T>
typename CGraphIterator3D<T>::node* CGraphIterator3D<T>::self()
{
    return &m_pgraph->m_matriz[m_row][m_col][m_lay];
};

template< class T>
void CGraphIterator3D<T>::neighbor( CGraphIterator3D<T>* iter, int i)
{
    m_row = iter->m_row + RS3D[i];
    m_col = iter->m_col + CS3D[i];
    m_lay = iter->m_lay + LS3D[i];
};

#endif // CGRAPHITERATOR3D_H
