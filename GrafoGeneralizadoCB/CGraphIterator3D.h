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
    typedef CGraphIterator3D<T> self;

    //members
    CGraphImage3D<T> *m_pgraph;//pointo to graph we are navigating to

    node ***m_prow;
    node **m_pcol;
    node *m_play;
    int m_rows;
    int m_cols;
    int m_lays;

    //overloaded operators
    void operator=(CGraphIterator3D<T> *iter);
    void operator=(CGraphIterator3D<T> iter);
    void operator++(int);
    bool operator==(CGraphIterator3D<T> *iter);
    bool operator!=(CGraphIterator3D<T> *iter);
    node* operator->();
    node*& operator*();

    //methods
    CGraphIterator3D();
    virtual ~CGraphIterator3D();

    node* neighbor_node_at(int i);
    self neighbor_at(int i);

protected:
private:
};

template< class T>
CGraphIterator3D<T>::CGraphIterator3D()
{
};

template< class T>
CGraphIterator3D<T>::~CGraphIterator3D()
{
};

template< class T>
void CGraphIterator3D<T>::operator=(CGraphIterator3D<T> iter)
{
    m_rows = iter.m_rows;
    m_cols = iter.m_cols;
    m_lays = iter.m_lays;

    m_prow = iter.m_prow;
    m_pcol = iter.m_pcol;
    m_play = iter.m_play;
};
template< class T>
void CGraphIterator3D<T>::operator=(CGraphIterator3D<T> *iter)
{
    m_rows = iter->m_rows;
    m_cols = iter->m_cols;
    m_lays = iter->m_lays;

    m_prow = iter->m_prow;
    m_pcol = iter->m_pcol;
    m_play = iter->m_play;
};

template< class T>
bool CGraphIterator3D<T>::operator==(CGraphIterator3D<T> *iter)
{
    return m_prow == iter->m_prow;// && m_pcol==iter->m_pcol;
};

template< class T>
bool CGraphIterator3D<T>::operator!=(CGraphIterator3D<T> *iter)
{
    return !(m_prow==iter->m_prow);// && m_pcol==iter->m_pcol);
};

template< class T>
void CGraphIterator3D<T>::operator++(int)
{
    //detect if layer pointer is in the final layer
    if( ++m_play - (*m_pcol) < m_lays)
        return;//if not go to next layer
    if( ++m_pcol - (*m_prow) < m_cols){
        m_play=(*m_pcol);
        return;//if not go to next column
    }//if it is go the next row and column to the initial column
    m_prow++;
    m_pcol=(*m_prow);
    m_play=(*m_pcol);

    /*int i=0;
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
    */
};

template< class T>
typename CGraphIterator3D<T>::node* CGraphIterator3D<T>::operator->()
{
    return m_play;
    //return &m_pgraph->m_matriz[m_row][m_col][m_lay];
};

template< class T>
typename CGraphIterator3D<T>::node*& CGraphIterator3D<T>::operator*()
{
    return m_play;
};

template< class T>
typename CGraphIterator3D<T>::node* CGraphIterator3D<T>::neighbor_node_at(int i)
{
    ///TODO fix width pointer distance
    return ((*(m_prow + RS3D[i]) + CS3D[i]));
};

template< class T>
typename CGraphIterator3D<T>::self CGraphIterator3D<T>::neighbor_at(int i)
{
    self neighbor;

    int lay_dist = m_play - (*m_pcol);
    int col_dist = m_pcol - (*m_prow);

    neighbor.m_rows = m_rows;
    neighbor.m_cols = m_cols;
    neighbor.m_lays = m_lays;

    neighbor.m_prow = m_prow + RS3D[i];
    neighbor.m_pcol = (*neighbor.m_prow) + col_dist + CS3D[i];
    neighbor.m_play = (*neighbor.m_pcol) + lay_dist + LS3D[i];

    return neighbor;
};


#endif // CGRAPHITERATOR3D_H
