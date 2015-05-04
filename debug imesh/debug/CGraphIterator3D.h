#ifndef CGRAPHITERATOR3D_H
#define CGRAPHITERATOR3D_H

#include "CGraphIterator.h"

//this arrays are for the navigation through neigbours around a cell
int row_step3d[6]= { 0,-1, 0, 1, 0, 0};
int col_step3d[6]= { 0, 0,-1, 0, 1, 0};
int lay_step3d[6]= {-1, 0, 0, 0, 0, 1};

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
    node ***m_prow, ***m_prowbegin;
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

    m_prowbegin = iter.m_prow;
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

    m_prowbegin = iter->m_prow;

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
    //cout<<"i++-";
    //detect if layer pointer is in the final layer

    if( ++m_play - (*m_pcol) < m_lays){//cout<<"~";
        return;}//if not go to next layer
    if( ++m_pcol - (*m_prow) < m_cols){
        m_play=(*m_pcol);//cout<<"*";
        return;//if not go to next column
    }//if it is go the next row and column to the initial column

    if (++m_prow - m_prowbegin < m_rows){
        //cout<<".";
        m_pcol=(*m_prow);//cout<<".";
        m_play=(*m_pcol);//cout<<".";
        //cout<<"i++"<<endl;
    }
};

template< class T>
typename CGraphIterator3D<T>::node* CGraphIterator3D<T>::operator->()
{
    return m_play;
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
