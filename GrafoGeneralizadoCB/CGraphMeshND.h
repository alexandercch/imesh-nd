/**
    Seminario de Tesis 10
    CGraphMeshND.h
    Purpose: Is a child class from CGraph will hold a mesh in any
        dimension, the dimencion is defined in the number of neighbors
        member
    @author Alex Ccacyahuillca
    @version 1.0 04/015
*/

#ifndef CGRAPHMESHND_H_INCLUDED
#define CGRAPHMESHND_H_INCLUDED

#include "CGraph.h"
#include "CGraphIteratorND.h"


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

    /**
        @return the total number of elements
    */
    int weight();

    /**
        @return the total space used by the graph (i.e. area, volumen, etc)
    */
    int area();

    /**
        Sets the size of the graph
        @param size the number of elements the graph will hold
        @param dimension the number neighbors each node will containt
        in this case that number will also define de dimension of the
        mesh(2D, 3D, ND).
    */
    void config(int size, int dimension);

    /**
        Read data from file
        @param filename path to the data file
    */
    void load_data(string filename);

    //methods - iteration
    /**
        @return a pointer to the beginning of the Graph, and the same
                for end method.
    */
    iterator* begin();
    iterator* end();

    /**
        Set an specific element with the specific data
        @param iter the iterator that point to the node
        @param data the data to put in the place pointed by iter
    */
    void set_at(iterator& iter, T& data);

    /**
        Copy all data from other graph, first sets to the new dimensions
        and begin the copy.
        @param _graph pointer to a same class graph
    */
    void operator=(CGraphMeshND &_graph);


    void print_mesh();//beacause still we dont draw a mesh we print its values temporaly

private:
    iterator *m_ibegin, *m_iend;
};

template<class T>CGraphMeshND<T>::CGraphMeshND ():
    m_ibegin(new iterator),
    m_iend(new iterator)
{

}
template<class T>CGraphMeshND<T>::~CGraphMeshND () {


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
    m_ibegin->m_pgraph  =this;
    m_ibegin->m_index   =0;

    m_iend->m_pgraph    =this;
    m_iend->m_index     =size;
}

template<class T>
typename CGraphMeshND<T>::iterator* CGraphMeshND<T>::begin()
{
    return m_ibegin;
}

template<class T>
typename CGraphMeshND<T>::iterator* CGraphMeshND<T>::end()
{
    return m_iend;
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
void CGraphMeshND<T>::load_data(string filename)
{
    ifstream in(filename.c_str());
    int size, dimension, data, neighbor;
    in>>size>>dimension;
    config(size, dimension);
    for(int i=0; i< size; ++i)
    {
        in>>data;
        m_nodes[i].m_data=data;
        for(int j=0; j< dimension+1; ++j)
        {
            in>>neighbor;
            m_adj_list[i][j]=neighbor;
        }
    }
}

template<class T>
int CGraphMeshND<T>::weight()
{
    return m_size;
}

template<class T>
int CGraphMeshND<T>::area()
{
    float tarea=0.0;
    for(int i=0; i< m_size; ++i)
        tarea+=m_nodes[i]->m_area;
    return tarea;
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
