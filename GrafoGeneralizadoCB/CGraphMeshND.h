#ifndef CGRAPHMESHND_H_INCLUDED
#define CGRAPHMESHND_H_INCLUDED
#include "CGraph.h"
#include "CGraphIteratorND.h"
#include <vector>
template<class T>
class CGraphMeshND : public CGraph<T>
{
public:
    typedef CGraphIteratorND<T> iterator;
    typedef typename CGraph<T>::node node;

    //members
    node *m_nodes ;
    int **m_adj_list;
    int m_number_of_neighbors, m_size;//number of neighbours define the dimension, msize

    //methods
    CGraphMeshND ();
    virtual ~CGraphMeshND ();
    void config(int size, int dimension);

    //methods - configuration
    void load_data(string filename);
    int weight();

    //methods - iteration
    CGraphMeshND* begin();
    void set_at(iterator& iter, T& data);

    //asignation operator
    void operator=(CGraphMeshND &_graph);
    void print_mesh();//beacause still we dont draw a mesh we print its values temporaly
};

template<class T>CGraphMeshND<T>::CGraphMeshND () {}
template<class T>CGraphMeshND<T>::~CGraphMeshND () {}

template<class T>
CGraphMeshND<T>* CGraphMeshND<T>::begin()
{
    return this;
}

template<class T>
void CGraphMeshND<T>::set_at(iterator& iter, T& data)
{
    m_nodes[iter.m_index].m_data= data;
}

template<class T>
void CGraphMeshND<T>::operator=(CGraphMeshND &_graph)
{
    config(_graph.m_size, _graph.m_number_of_neighbors-1);
    for(int i=0; i< _graph.m_size; ++i)
    {
        m_nodes[i]=_graph.m_nodes[i];
        for(int j=0; j < _graph.m_number_of_neighbors; ++j)
        {
            m_adj_list[i][j]=_graph.m_adj_list[i][j];
        }
    }
}

template<class T>
void CGraphMeshND<T>::config(int size, int dimension) //dimensio should be 2 or 3
{
    m_size=size;
    dimension++;
    m_number_of_neighbors= dimension;
    m_nodes= new node[size];
    m_adj_list= new int*[size];
    for(int i=0; i< size; ++i)
    {
        m_adj_list[i]= new int[dimension];
    }
}

template<class T>
void CGraphMeshND<T>::load_data(string filename)
{
    ifstream in(filename.c_str());
    int size, dimension, data;
    in>>size>>dimension;
    config(size, dimension);
    for(int i=0; i< size; ++i)
    {
        in>>data;
        m_nodes[i].m_data=data;
        for(int j=0; j< dimension+1; ++j)
        {
            in>>data;
            m_adj_list[i][j]=data;
        }
    }
}

template<class T>
int CGraphMeshND<T>::weight()
{
    return m_size;
}

template<class T>
void CGraphMeshND<T>::print_mesh()
{
    for(int i=0; i< m_size; ++i)
    {
        cout<<m_nodes[i].m_data<<endl;
    }
}
#endif // CGRAPHMESHND_H_INCLUDED
