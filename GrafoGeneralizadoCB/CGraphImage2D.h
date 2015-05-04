/**
    Seminario de Tesis 10
    CGraphImage2D.h
    Purpose: Is a child class from CGraph will hold a 2D matrix

    @author Alex Ccacyahuillca
    @version 1.0 04/015
*/

#ifndef CGRAPHIMAGE2D_H_INCLUDED
#define CGRAPHIMAGE2D_H_INCLUDED
#include "CGraph.h"
#include "CGraphIterator2D.h"

#define NUMBER_OF_NEIGHBOURS_2D 4

template<class T>
class CGraphImage2D : public CGraph<T>
{
public:
    typedef CGraphIterator2D<T> iterator;
    typedef typename CGraph<T>::node node;

    //members
    node **m_matriz;//this will hold the data
    int m_rows, m_cols;
    int m_number_of_neighbors;

    //methods
    CGraphImage2D ();
    virtual ~CGraphImage2D ();

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
        @param _rows the number of rows of the 2D matrix graph
        @param _cols the number of cols of the 2D matrix graph
    */
    void config(int _rows, int _cols);

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
    void operator=(CGraphImage2D &_graph);

protected:
private:
    iterator *m_ibegin, *m_iend;
    //end pointer
};

template<class T>
CGraphImage2D<T>::CGraphImage2D ():m_number_of_neighbors(NUMBER_OF_NEIGHBOURS_2D)
                                    ,m_ibegin(new iterator), m_iend(new iterator)
{
}

template<class T>
CGraphImage2D <T>::~CGraphImage2D ()
{
    //dtor
    //dtor
    for(int i=0; i<m_rows;++i)
        delete[] m_matriz[i];
    delete[] m_matriz;
}

template<class T>
void CGraphImage2D <T>::config(int _rows, int _cols)
{
    m_rows = _rows;
    m_cols = _cols;
    m_matriz = new node*[_rows];
    for(int i = 0; i < _rows; ++i)
    {
        m_matriz[i] = new node[_cols];
    }
    cout<<"begin setted!"<<endl;
    m_ibegin->m_prow = m_matriz;
    m_ibegin->m_pcol = (*m_matriz);
    m_ibegin->m_rows = m_rows;
    m_ibegin->m_cols = m_cols;


    m_iend->m_prow =   &(m_matriz[m_rows]);
    m_iend->m_pcol =   &(m_matriz[m_rows][0]);
    cout<<"end setted!"<<endl;
}

template<class T>
typename CGraphImage2D<T>::iterator* CGraphImage2D<T>::begin()
{
    return m_ibegin;
}

template<class T>
typename CGraphImage2D<T>::iterator* CGraphImage2D<T>::end()
{
    return m_iend;
}

template<class T>
void CGraphImage2D <T>::load_data(string filename)
{
    ifstream in(filename.c_str());//input stream
    in>>m_rows>>m_cols;
    config(m_rows, m_cols);
    for(int i=0; i< m_rows; ++i)
        for(int j=0; j< m_cols; ++j)
            in>>m_matriz[i][j];
    in.close();
}

template<class T>
int CGraphImage2D<T>::weight()
{
    return m_rows*m_cols; //number of elements
}

template<class T>
int CGraphImage2D<T>::area()
{
    return m_rows*m_cols; //number of area in pixels
}

template<class T>
void CGraphImage2D<T>::operator=(CGraphImage2D <T> &_graph)
{
    config(_graph.m_rows, _graph.m_cols);//config reset/set size of the matrix
    for(int i=0; i< m_rows; ++i)
        for(int j=0; j< m_cols; ++j)
            m_matriz[i][j] = _graph.m_matriz[i][j];
}

template<class T>
void CGraphImage2D<T>::set_at(iterator& iter, T& data)
{
    //m_matriz[iter.m_row][iter.m_col].m_data = data;
    iter->m_data = data;
}

#endif // CGRAPHIMAGE2D_H_INCLUDED






//old code, just to dont loose everything

//static int row_step[4], col_step[4];//inside class
//template<class T>
//int CGraphImage2D<T>::row_step[4]= {1,-1, 0, 0};
//template<class T>
//int CGraphImage2D<T>::col_step[4]= {0, 0, 1,-1};
