#ifndef SEGMENTATOR_H
#define SEGMENTATOR_H
#include <map>
#include <queue>
#include <iostream>
#include <vector>
#include "CMeshRegion.h"
#include "CColor.h"

#define FORVZ(x) for(int i = 0;i<x.size(); ++i)

using namespace std;
template<class G>
class CSegmentator
{
public:
    //typedef typename G::T T;
    typedef typename G::iterator iterator;
    typedef typename G::node node;
    //map<HashType, int> vis;
    int cntr;
    int m_max_segmentation_difference;
    CSegmentator();
    //CSegmentator(G *_m_pgraph);
    CSegmentator(G *_input, G *_output);
    CColor color;

    //G *m_pgraph;

    G *input;
    G *output;
    virtual ~CSegmentator();

    //segmentation algorithms
    void binary_segmentation();
    void group_neighbor_cells();
    void group_neighbor_regions();

    //imagen preprocesing
    void to_gray_scale();



    //debuging subroutines
    void show_mesh_region();


protected:
private:
    //private members
    vector<CMeshRegion<G>* > m_meshregionV;

    //processes to handle pixel by pixel
    void init();
    int rgb_difference(unsigned int a, unsigned int b);
    inline int gray_difference(int a, int b);
    int process_pixel_binary(int pixel);
    int process_gray_scale(int pixel);
    void get_rbg(int &r, int &g, int &b, int pixel);
};

template<class G>
CSegmentator<G>::CSegmentator():cntr(0)
{
    init();
};
template<class G>
CSegmentator<G>::CSegmentator(G *_input, G *_output):input(_input),output(_output),cntr(0)
{
    init();
};
template<class G>
CSegmentator<G>::~CSegmentator() {};

template<class G>
void CSegmentator<G>::init()
{
    m_max_segmentation_difference=20;
};

template<class G>
void CSegmentator<G>::get_rbg(int &r, int &g, int &b, int pixel)
{
    r=(pixel>>16)&0x000000ff;
    g=(pixel>>8)&0x000000ff;
    b=(pixel)&0x000000ff;
}


template<class G>
int CSegmentator<G>::process_pixel_binary(int pixel)
{
    int r, g, b;
    get_rbg(r, g, b, pixel);
    if (((r+g+b)>>1) < 126)
        pixel= 0x00000000;//negro
    else
    {
        pixel=0xffffffff;//blanco
        //cout<<"blanco fue asignado"<<endl;
    }
    return pixel;
}

template<class G>
int CSegmentator<G>::process_gray_scale(int pixel)
{
    int r, g, b, promedio;
    get_rbg(r, g, b, pixel);
    promedio=(r+g+b)/3;
    pixel=0xff000000;//blanco
    //pixel|= (promedio<<16) | (promedio<<8) | (promedio);
    pixel=promedio;
    return pixel;
}

template<class G>
int CSegmentator<G>::rgb_difference(unsigned int pixela, unsigned int pixelb)
{
    int ra, ga, ba;
    int rb, gb, bb;
    get_rbg(ra, ga, ba, pixela);
    get_rbg(rb, gb, bb, pixelb);
    //cout<<"diff:"<< abs(ra-rb) + abs(ga - gb) + abs(ba - bb)<<endl;
    return abs(ra-rb) + abs(ga - gb) + abs(ba - bb);
}

template<class G>
inline int CSegmentator<G>::gray_difference(int a, int b)
{
    return abs((a&0x000000ff)-(b&0x000000ff));
}

template<class G>
void CSegmentator<G>::binary_segmentation()
{
    iterator iter;
    int pixel;
    for(iter = input->begin(); !iter.end(); iter++)
    {
        //cout<<"vis"<<iter->m_data<<endl;
        pixel=iter->m_data;
        pixel=process_pixel_binary(pixel);
        output->set_at(iter, pixel);
    }
}

template<class G>
void CSegmentator<G>::to_gray_scale()
{
    int pixel;
    iterator iter ;
    for(iter = input->begin(); !iter.end(); iter++)
    {
        pixel=iter->m_data;
        pixel=process_gray_scale(pixel);
        input->set_at(iter, pixel);
    }
}

template<class G>
void CSegmentator<G>::group_neighbor_cells()
{
    int pixel;
    iterator iter ;
    int labeler=0;
    for(iter = input->begin(); !iter.end(); iter++)
    {
        if (iter->m_label !=-1) continue;
        queue<iterator> node_queue;
        iter->m_label=++labeler;
        CMeshRegion<G> *mr= new CMeshRegion<G>();
        mr->Init(labeler);
        node_queue.push(iter);
        while(node_queue.size())
        {
            iterator actual =node_queue.front();
            node_queue.pop();

            mr->Incorporate(actual.self());

            iterator i_neighbor_actual;
            i_neighbor_actual= actual;
            for(int i = 0 ; i< input->m_number_of_neighbors; ++i)
            {
                i_neighbor_actual.neighbor(&actual, i);
                //we add the neighbor of the
                if ()


                if (i_neighbor_actual->m_label>-1 || i_neighbor_actual->m_visited) continue;

                if ( gray_difference(i_neighbor_actual->m_data, actual->m_data)
                        < m_max_segmentation_difference )
                {
                    i_neighbor_actual->m_label=labeler;
                    node_queue.push(i_neighbor_actual);
                }
            }
        }
        m_meshregionV.push_back(mr);
    }
}



template<class G>
void CSegmentator<G>::group_neighbor_regions(){
    //the code goes here :3

}


template<class G>
void CSegmentator<G>::show_mesh_region(){
    cout<<"id: area label ncells mpattern"<<endl;
    FORVZ(m_meshregionV){
        cout<<i<<":"<<m_meshregionV[i]->m_area<<" "
            <<m_meshregionV[i]->m_label<<" "
            <<m_meshregionV[i]->m_ncells<<" "<<m_meshregionV[i]->m_pattern<<endl;
    }
}



#endif // SEGMENTATOR_H
