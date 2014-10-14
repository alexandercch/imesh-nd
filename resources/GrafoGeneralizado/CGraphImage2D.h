#ifndef CGRAPHIMAGE2D_H_INCLUDED
#define CGRAPHIMAGE2D_H_INCLUDED
#include "CGraph.h"
#include "CGraphIterator2D.h"

#define NUMBER_OF_NEIGHBOURS 4

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
	int weight();

    //methods - configuration
    void config(int _rows, int _cols);
    void load_data(string filename);    

    //methods - iteration
    CGraphImage2D* begin();
    void set_at(iterator& iter, T& data);

    //asignation operator
    void operator=(CGraphImage2D &_graph);
   
protected:
private:
};

template<class T>
CGraphImage2D<T>::CGraphImage2D ()
{
    m_number_of_neighbors = NUMBER_OF_NEIGHBOURS;
}

template<class T>
CGraphImage2D <T>::~CGraphImage2D ()
{
    //dtor
}

template<class T>
void CGraphImage2D <T>::config(int _rows, int _cols)
{
    m_rows = _rows, m_cols=_cols;
    m_matriz = new node*[_rows];
    for(int i = 0; i < _rows; ++i)
    {
        m_matriz[i] = new node[_cols];
    }
}

template<class T>
CGraphImage2D<T>* CGraphImage2D<T>::begin()
{
    return this;
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
    m_matriz[iter.m_row][iter.m_col].m_data = data;
}

#endif // CGRAPHIMAGE2D_H_INCLUDED






//old code, just to dont loose everything

//static int row_step[4], col_step[4];//inside class
//template<class T>
//int CGraphImage2D<T>::row_step[4]= {1,-1, 0, 0};
//template<class T>
//int CGraphImage2D<T>::col_step[4]= {0, 0, 1,-1};