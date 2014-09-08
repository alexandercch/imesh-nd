#ifndef CGRAPHITERATORND_H_INCLUDED
#define CGRAPHITERATORND_H_INCLUDED

#include "CGraphIterator.h"

template<class T>
class CGraphMeshND;

template< class T>
class CGraphIteratorND:public CGraphIterator<T>
{
public:
    typedef typename CGraphMeshND<T>::node node;
    CGraphIteratorND();
    virtual ~CGraphIteratorND();

    //members
    CGraphMeshND<T> *m_pgraph;//pointo to graph we are navigating to
    int m_index;//holds position of the iterator
    bool m_end;//flag to check wether it has reach the end

    //methods
    bool end();
    void neighbor(CGraphIteratorND<T>* iter, int i);
    node *self();

    //overloaded operators
    void operator=(CGraphMeshND<T> *graph);
    void operator=(CGraphIteratorND<T> *iter);
    void operator++(int);
    node* operator->();
protected:
private:
};

template< class T>
CGraphIteratorND<T>::CGraphIteratorND()
{
    m_end= false;
};
template< class T>
CGraphIteratorND<T>::~CGraphIteratorND()
{
};

template< class T>
void CGraphIteratorND<T>::operator=(CGraphMeshND<T> *graph)
{
    m_pgraph = graph;
    m_index=0;
    graph->m_nodes[0].m_visited=true;
};
template< class T>
void CGraphIteratorND<T>::operator=(CGraphIteratorND<T> *iter)
{
    m_pgraph=iter->m_pgraph;
    m_index=iter->m_index;
};

template< class T>
void CGraphIteratorND<T>::operator++(int)
{
    //we navigate through the adjacency list
    int i=0;
    for(i=0; i< m_pgraph->m_number_of_neighbors; ++i)
    {
        if (m_pgraph->m_adj_list[m_index][i] ==-1 || m_pgraph->m_nodes[ m_pgraph->m_adj_list[m_index][i] ].m_visited)
            continue;
        m_pgraph->m_nodes[ m_pgraph->m_adj_list[m_index][i] ].m_visited = true;
        m_index =m_pgraph->m_adj_list[m_index][i];
        break;
    }
    //if all neighbors has been visited and not unvisited were found we have reach the end of the mesh
    if (i==m_pgraph->m_number_of_neighbors)
        m_end=true;//in order to stop bucle who is iterating
};

template< class T>
bool CGraphIteratorND<T>::end()
{
    return m_end;
};


template< class T>
typename CGraphIteratorND<T>::node* CGraphIteratorND<T>::operator->()
{
    return &m_pgraph->m_nodes[m_index];
};
template< class T>
void CGraphIteratorND<T>::neighbor( CGraphIteratorND<T>* iter, int i)
{
    m_index = m_pgraph->m_adj_list[iter->m_index][i];
};


#endif // CGRAPHITERATORND_H_INCLUDED
