#ifndef CIMAGE3D_H
#define CIMAGE3D_H
#include "CGraph.h"
#include "CGraphIterator3D.h"

#define NUMBER_OF_NEIGHBOURS 6

template<class T>
class CGraphImage3D : public CGraph<T>
{
public:
    //typedef T T;
    typedef CGraphIterator3D<T> iterator;
    typedef typename CGraph<T>::node node;

    //members
    node ***m_matriz;//this will hold the data
    int m_rows, m_cols, m_lays;
    int m_number_of_neighbors;

    //methods
    CGraphImage3D ();
    virtual ~CGraphImage3D ();
    int weight();

    //methods - configuration
    void config(int _rows, int _cols, int _lays);
    void load_data(string filename);

    //methods - iteration
    CGraphImage3D* begin();
    void set_at(iterator& iter, T& data);

    //asignation operator
    void operator=(CGraphImage3D &_graph);

protected:
private:
};

template<class T>
CGraphImage3D<T>::CGraphImage3D ()
{
    m_number_of_neighbors = NUMBER_OF_NEIGHBOURS;
}

template<class T>
CGraphImage3D <T>::~CGraphImage3D ()
{
    //dtor
}

template<class T>
void CGraphImage3D <T>::config(int _rows, int _cols, int _lays)
{
    m_rows=_rows, m_cols=_cols, m_lays= _lays;
    m_matriz= new node**[_rows];
    for(int i=0; i < _rows; ++i)
    {
        m_matriz[i]= new node*[_cols];
        for(int j=0; j< _cols; ++j)
            m_matriz[i][j]= new node[_lays];
    }
}

template<class T>
CGraphImage3D<T>* CGraphImage3D<T>::begin()
{
    return this;
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
    return m_rows*m_cols*m_lays;
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
    //cout<<"in graph "<<iter.m_row<<" "<<iter.m_col<<" "<<iter.m_lay<<endl;
    m_matriz[iter.m_row][iter.m_col][iter.m_lay].m_data = data;
}

#endif // CIMAGE3D_H













//old code, just to dont loose everything

/*template<class T>
typename CGraphImage3D<T>::node* CGraphImage3D <T>::at(iterator l)
{
    return &m_matriz[l.m_row][l.m_col][l.m_lay];
}

template<class T>
void CGraphImage3D <T>::set(iterator l, node value)
{
    m_matriz[l.m_row][l.m_col][l.m_lay]= value;
}
*/
/*template<class T>
int CGraphImage3D <T>::count_adjacents_at(CLocator3D location)
{
    if(location.m_row &&
            location.m_col &&
            location.m_lay &&
            location.m_row < m_rows - 1 &&
            location.m_col < m_cols - 1 &&
            location.m_lay < m_lays - 1)
        return 6;
    return 6-( (location.m_row == 0) +
               (location.m_row == m_rows - 1) +
               (location.m_col == 0) +
               (location.m_col == m_cols - 1) +
               (location.m_lay == 0) +
               (location.m_lay == m_lays - 1)
             );
}

template<class T>
CLocator3D CGraphImage3D <T>::adjacent_at_i(CLocator3D location, int i)
{
    if (count_adjacents_at(location)==6)
        return CLocator3D( location.m_row + row_step[i],
                           location.m_col + col_step[i],
                           location.m_lay + lay_step[i]);
    int _i=0, ni=-1;
    while(_i<i+1)
    {
        ni++;
        if (    location.m_row + row_step[ni] >= 0 &&
                location.m_col + col_step[ni] >= 0 &&
                location.m_lay + lay_step[ni] >= 0 &&
                location.m_row + row_step[ni] < m_rows &&
                location.m_col + col_step[ni] < m_cols &&
                location.m_lay + lay_step[ni] < m_lays)
            _i++;
    }
    return CLocator3D( location.m_row + row_step[ni],
                       location.m_col + col_step[ni],
                       location.m_lay + lay_step[ni]);
}

*/

//static int row_step[6], col_step[6], lay_step[6];
//template<class T>
//int CGraphImage3D<T>::row_step[6]= {1,-1, 0, 0, 0, 0};
//template<class T>
//int CGraphImage3D<T>::col_step[6]= {0, 0, 1,-1, 0, 0};
//template<class T>
//int CGraphImage3D<T>::lay_step[6]= {0, 0, 0, 0, 1,-1};
