/**
    Seminario de Tesis 10
    CGraphIteratorND.h
    Purpose: Is the iterator of CGraphMeshND, inherits from CGraphIterator
        in order to be a compatible iteration in the generalized graph
    @author Alex Ccacyahuillca
    @version 1.0 04/015
*/

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
    typedef CGraphIteratorND<T> self;

    //members
    CGraphMeshND<T>* m_pgraph;
    int m_index;

    //overloaded operators
    void operator=(CGraphIteratorND<T> *iter);
    void operator=(CGraphIteratorND<T> iter);
    void operator++(int);
    bool operator==(CGraphIteratorND<T> *iter);
    bool operator!=(CGraphIteratorND<T> *iter);
    node* operator->();
    node*& operator*();

    //methods
    CGraphIteratorND();
    virtual ~CGraphIteratorND();

    /**
        return the neighbor node at the position i
        @param i neighbor number, see CGraphMeshND::number of neighbors
    */
    node* neighbor_node_at(int i);

    /**
        return the neighbor iterator at position i
        @param i neighbor number, see CGraphMeshND::number of neighbors
    */
    self neighbor_at(int i);

protected:
private:
};

template< class T>
CGraphIteratorND<T>::CGraphIteratorND()
{
};

template< class T>
CGraphIteratorND<T>::~CGraphIteratorND()
{
};

template< class T>
void CGraphIteratorND<T>::operator=(CGraphIteratorND<T> iter)
{
    m_pgraph    = iter.m_pgraph;
    m_index     = iter.m_index;
};
template< class T>
void CGraphIteratorND<T>::operator=(CGraphIteratorND<T> *iter)
{
    m_pgraph    = iter->m_pgraph;
    m_index     = iter->m_index;
};

template< class T>
bool CGraphIteratorND<T>::operator==(CGraphIteratorND<T> *iter)
{
    return m_index == iter->m_index;// && m_pgraph=iter->m_pgraph;
};

template< class T>
bool CGraphIteratorND<T>::operator!=(CGraphIteratorND<T> *iter)
{
    return m_index != iter->m_index;// || m_pgraph!=iter->m_pgraph;
};

template< class T>
void CGraphIteratorND<T>::operator++(int)
{
    m_index++;
};

template< class T>
typename CGraphIteratorND<T>::node* CGraphIteratorND<T>::operator->()
{
    return &m_pgraph->m_nodes[m_index];
};

template< class T>
typename CGraphIteratorND<T>::node*& CGraphIteratorND<T>::operator*()
{
    return m_pgraph->m_nodes[m_index];
};

template< class T>
typename CGraphIteratorND<T>::node* CGraphIteratorND<T>::neighbor_node_at(int i)
{
    ///TODO fix width pointer distance
    return &m_pgraph->m_nodes[m_pgraph->m_adj_list[m_index][i]];
};

template< class T>
typename CGraphIteratorND<T>::self CGraphIteratorND<T>::neighbor_at(int i)
{
    self neighbor;
    neighbor.m_pgraph   = m_pgraph;
    neighbor.m_index    = m_pgraph->m_adj_list[m_index][i];
    return neighbor;
};

#endif // CGRAPHITERATORND_H_INCLUDED
