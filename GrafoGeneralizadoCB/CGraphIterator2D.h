#ifndef CGRAPHITERATOR2D_H_INCLUDED
#define CGRAPHITERATOR2D_H_INCLUDED

#include "CGraphIterator.h"

//this arrays are for the navigation through neigbours around a cell
int row_step2d[4]= {1,-1, 0, 0};
int col_step2d[4]= {0, 0, 1,-1};

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
    int m_cols;//hold the matrix dimensions

    //CGraphImage2D<T> *m_pgraph;//pointo to graph we are navigating to

    //bool m_end;//flag to check wether it has reach the end

    //overloaded operators
    //void operator=(CGraphImage2D<T> *graph);
    void operator=(CGraphIterator2D<T> *iter);
    void operator=(CGraphIterator2D<T> iter);
    void operator++(int);
    node* operator->();
    node*& operator*();
    bool operator==(CGraphIterator2D<T> *iter);
    bool operator!=(CGraphIterator2D<T> *iter);

    //methods
    CGraphIterator2D();
    virtual ~CGraphIterator2D();
    //bool end();
    node* neighbor_node_at(int i);
    self neighbor_at(int i);

protected:
private:
};

template< class T>
CGraphIterator2D<T>::CGraphIterator2D()
{
    //m_end = false;//at begin iterator must be able to navigate though graph
};
template< class T>
CGraphIterator2D<T>::~CGraphIterator2D()
{
};

/*template< class T>
void CGraphIterator2D<T>::operator=(CGraphImage2D<T> *graph)
{
    m_pgraph= graph;
    m_row=m_col=1;
    //graph->m_matriz[1][1].m_visited=true;
};*/

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
    cout<<"iter= * called"<<endl;
    m_rows = iter->m_rows;
    m_cols = iter->m_cols;

    m_prow = iter->m_prow;
    m_pcol = iter->m_pcol;
    cout<<"iter= * called end"<<endl;
};

template< class T>
bool CGraphIterator2D<T>::operator==(CGraphIterator2D<T> *iter)
{
    return m_prow==iter->m_prow && m_pcol==iter->m_pcol;
};

template< class T>
bool CGraphIterator2D<T>::operator!=(CGraphIterator2D<T> *iter)
{
    cout<<"!= called"<<endl;
    return !(m_prow==iter->m_prow && m_pcol==iter->m_pcol);
};


template< class T>
void CGraphIterator2D<T>::operator++(int)
{
    //detect if row pointer is in the final column
    if( ++m_pcol - (*m_prow) < m_cols){
        return;//if not go to next column
    }//if it is go the next row and column to the initial column
    m_prow++;
    m_pcol=(*m_prow);
    cout<<"in ++"<<endl;
    /*int i = 0;
    for(i = 0; i < m_pgraph->m_number_of_neighbors; ++i)
    {
        if (m_pgraph->m_matriz[m_row + RS2D[i]][m_col + CS2D[i]].m_visited)
            continue;
        m_pgraph->m_matriz[m_row + RS2D[i]][m_col + CS2D[i]].m_visited = true;

        m_row+=RS2D[i];
        m_col+=CS2D[i];
        break;
    }
    //if all neighbors has been visited and not unvisited were found, we have reach the end of the matrix
    if (i == m_pgraph->m_number_of_neighbors)
        m_end=true;//in order to stop bucle who is iterating*/
};

/*template< class T>
bool CGraphIterator2D<T>::end()
{
    return m_end;
};*/


template< class T>
typename CGraphIterator2D<T>::node* CGraphIterator2D<T>::operator->()
{
    //return &m_pgraph->m_matriz[m_row][m_col];
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
    //m_row = iter->m_row + RS2D[i];
    //m_col = iter->m_col + CS2D[i];

    //find column position
    int dist = m_pcol - (*m_prow);

    return (*(m_prow + RS2D[i]) + CS2D[i]);
};

template< class T>
typename CGraphIterator2D<T>::self CGraphIterator2D<T>::neighbor_at(int i)
{
    self neighbor;

    neighbor.m_rows = m_rows;
    neighbor.m_cols = m_cols;

    neighbor.m_prow= m_prow + RS2D[i];
    neighbor.m_pcol= (*neighbor.m_prow) + CS2D[i];

    return neighbor;
};



#endif // CGRAPHITERATOR2D_H_INCLUDED
