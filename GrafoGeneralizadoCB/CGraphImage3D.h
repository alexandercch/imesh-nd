/**
    Seminario de Tesis 10
    CGraphImage3D.h
    Purpose: Is a child class will hold a 3D matrix

    @author Alex Ccacyahuillca
    @version 1.0 04/015
*/

#ifndef CIMAGE3D_H
#define CIMAGE3D_H

#include "CGraph.h"
#include "CGraphIterator3D.h"

#define NUMBER_OF_NEIGHBOURS_3D 6

template<class T>
class CGraphImage3D : public CGraph<T>
{
public:
    typedef CGraphIterator3D<T> iterator;
    typedef typename CGraph<T>::node node;

    //members
    node ***m_matriz;//this will hold the data
    int m_rows, m_cols, m_lays;
    int m_number_of_neighbors;

    //methods
    CGraphImage3D ();
    virtual ~CGraphImage3D ();

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
        @param _rows the number of rows of the 3D matrix graph
        @param _cols the number of columns of the 3D matrix graph
        @param _lays the number of layers of the 3D matrix graph
    */
    void config(int _rows, int _cols, int _lays);

    /**
        Read data from file
        @param filename path to the data file
    */
    void load_data(string filename);

    //methods - iteration
    /**
        @return a pointer to the beginning of the Graph
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
    void operator=(CGraphImage3D &_graph);

protected:
private:
    iterator *m_ibegin, *m_iend;
};

template<class T>
CGraphImage3D<T>::CGraphImage3D():
m_number_of_neighbors(NUMBER_OF_NEIGHBOURS_3D),
m_ibegin(new iterator),
m_iend(new iterator)
{
}

template<class T>
CGraphImage3D <T>::~CGraphImage3D ()
{
    //dtor
    for(int i=0; i<m_rows;++i){
        for(int j=0; j< m_cols; ++j)
            delete[] m_matriz[i][j];
        delete[] m_matriz[i];
    }
    delete[] m_matriz;
}

template<class T>
void CGraphImage3D <T>::config(int _rows, int _cols, int _lays)
{
    m_rows = _rows;
    m_cols = _cols;
    m_lays = _lays;
    m_matriz= new node**[_rows];
    for(int i=0; i < _rows; ++i)
    {
        m_matriz[i]= new node*[_cols];
        for(int j=0; j< _cols; ++j)
            m_matriz[i][j]= new node[_lays];
    }
    cout<<"iters"<<endl;
    m_ibegin->m_prow = m_matriz;
    m_ibegin->m_pcol = (*m_matriz);
    m_ibegin->m_play = (**m_matriz);
    m_ibegin->m_rows = m_rows;
    m_ibegin->m_cols = m_cols;
    m_ibegin->m_lays = m_lays;
    cout<<"begin"<<endl;
    m_iend->m_prow =   &(m_matriz[m_rows]);
    //m_iend->m_pcol =   &(m_matriz[m_rows][0]);
    //m_iend->m_play =   &(m_matriz[m_rows][0][0]);
    cout<<"end"<<endl;
}

template<class T>
typename CGraphImage3D<T>::iterator* CGraphImage3D<T>::begin()
{
    return m_ibegin;
}

template<class T>
typename CGraphImage3D<T>::iterator* CGraphImage3D<T>::end()
{
    return m_iend;
}

template<class T>
void CGraphImage3D <T>::load_data(string filename)
{
    ifstream in(filename.c_str());
    in>>m_rows>>m_cols>>m_lays;
    config(m_rows, m_cols, m_lays);
    for(int i=0; i< m_rows; ++i)
        for(int j=0; j< m_cols; ++j)
            for(int k=0; k< m_lays; ++k)
                in>>m_matriz[i][j][k];
    in.close();
}


template<class T>
int CGraphImage3D<T>::weight()
{
    return m_rows*m_cols*m_lays;//number of elements
}

template<class T>
int CGraphImage3D<T>::area()
{
    return m_rows*m_cols*m_lays; //number of area in pixels
}

template<class T>
void CGraphImage3D<T>::operator=(CGraphImage3D <T> &_graph)
{
    config(_graph.m_rows, _graph.m_cols, _graph.m_lays);
    for(int i=0; i< m_rows; ++i)
        for(int j=0; j< m_cols; ++j)
            for(int k=0; k< m_lays; ++k)
                m_matriz[i][j][k] = _graph.m_matriz[i][j][k];
}


template<class T>
void CGraphImage3D<T>::set_at(iterator& iter, T& data)
{
    m_matriz[iter.m_row][iter.m_col][iter.m_lay].m_data = data;
}

#endif // CIMAGE3D_H
