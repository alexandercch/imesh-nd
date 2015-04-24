#ifndef CGRAPHITERATOR2D_H_INCLUDED
#define CGRAPHITERATOR2D_H_INCLUDED

#include "CGraphIterator.h"

//this arrays are for the navigation through neigbours around a cell
int row_step2d[4]= {-1, 0, 1, 0};
int col_step2d[4]= { 0, 1, 0,-1};

#define RS2D row_step2d
#define CS2D col_step2d

template<class T>
class CGraphImage2D;

template< class T>
class CGraphIterator2D:public CGraphIterator<T>
{
public:
    typedef typename CGraphImage2D<T>::node node;
    typedef CGraphIterator2D<T> self;

    //members
    node **m_prow;
    node *m_pcol;
    int m_rows;
    int m_cols;

    //overloaded operators
    void operator=(CGraphIterator2D<T> *iter);
    void operator=(CGraphIterator2D<T> iter);
    void operator++(int);
    bool operator==(CGraphIterator2D<T> *iter);
    bool operator!=(CGraphIterator2D<T> *iter);
    node* operator->();
    node*& operator*();

    //methods
    CGraphIterator2D();
    virtual ~CGraphIterator2D();

    node* neighbor_node_at(int i);
    self neighbor_at(int i);

protected:
private:
};

template< class T>
CGraphIterator2D<T>::CGraphIterator2D()
{
};

template< class T>
CGraphIterator2D<T>::~CGraphIterator2D()
{
};

template< class T>
void CGraphIterator2D<T>::operator=(CGraphIterator2D<T> iter)
{
    m_rows=iter.m_rows;
    m_cols=iter.m_cols;

    m_prow=iter.m_prow;
    m_pcol=iter.m_pcol;
};

template< class T>
void CGraphIterator2D<T>::operator=(CGraphIterator2D<T> *iter)
{
    m_rows = iter->m_rows;
    m_cols = iter->m_cols;

    m_prow = iter->m_prow;
    m_pcol = iter->m_pcol;
};

template< class T>
bool CGraphIterator2D<T>::operator==(CGraphIterator2D<T> *iter)
{
    return m_prow==iter->m_prow && m_pcol==iter->m_pcol;
};

template< class T>
bool CGraphIterator2D<T>::operator!=(CGraphIterator2D<T> *iter)
{
    return !(m_prow==iter->m_prow && m_pcol==iter->m_pcol);
};


template< class T>
void CGraphIterator2D<T>::operator++(int)
{
    //detect if column pointer is in the final column
    if( ++m_pcol - (*m_prow) < m_cols){
        return;//if not go to next column
    }//if it is go the next row and column to the initial column
    m_prow++;
    m_pcol=(*m_prow);
};

template< class T>
typename CGraphIterator2D<T>::node* CGraphIterator2D<T>::operator->()
{
    return m_pcol;
};

template< class T>
typename CGraphIterator2D<T>::node*& CGraphIterator2D<T>::operator*()
{
    return m_pcol;
};

template< class T>
typename CGraphIterator2D<T>::node* CGraphIterator2D<T>::neighbor_node_at(int i)
{
    ///TODO fix width pointer distance
    return (*(m_prow + RS2D[i]) + CS2D[i]);
};

template< class T>
typename CGraphIterator2D<T>::self CGraphIterator2D<T>::neighbor_at(int i)
{
    self neighbor;
    int dist = m_pcol - (*m_prow);

    neighbor.m_rows = m_rows;
    neighbor.m_cols = m_cols;

    neighbor.m_prow= m_prow + RS2D[i];
    neighbor.m_pcol= (*neighbor.m_prow) + dist + CS2D[i];

    return neighbor;
};

#endif // CGRAPHITERATOR2D_H_INCLUDED
