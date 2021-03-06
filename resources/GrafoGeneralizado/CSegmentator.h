#ifndef SEGMENTATOR_H
#define SEGMENTATOR_H
#include <map>
#include <queue>
#include <iostream>
#include "CColor.h"
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
    CSegmentator(G *_input, G *_output);
    CColor color;

    G *input;
    G *output;
    virtual ~CSegmentator();

	//segmentation algorithms
    void binary_segmentation();
    void gray_scale();
    void group_neighbor_cells();

protected:
private:
	//processes to handle pixel by pixel
    void init();
    int difference(unsigned int a, unsigned int b);
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
    pixel=0xff00000000 ;//blanco
    pixel|= (promedio<<16) | (promedio<<8) | (promedio);
    return pixel;
}

template<class G>
int CSegmentator<G>::difference(unsigned int pixela, unsigned int pixelb)
{
    int ra, ga, ba;
    int rb, gb, bb;
    get_rbg(ra, ga, ba, pixela);
    get_rbg(rb, gb, bb, pixelb);
    //cout<<"diff:"<< abs(ra-rb) + abs(ga - gb) + abs(ba - bb)<<endl;
    return abs(ra-rb) + abs(ga - gb) + abs(ba - bb);
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
void CSegmentator<G>::gray_scale()
{
    int pixel;
    iterator iter ;
    for(iter = input->begin(); !iter.end(); iter++)
    {
        pixel=iter->m_data;
        pixel=process_gray_scale(pixel);
        output->set_at(iter, pixel);
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
        //cout<<iter.m_row<<" "<<iter.m_col<<" "<<iter.m_lay<<endl;
        if (iter->m_flag !=-1) continue;
        queue<iterator> node_queue;
        iter->m_flag=++labeler;
        node_queue.push(iter);
        //cout<<"label:"<<labeler<<endl;
        while(node_queue.size())
        {
            iterator actual =node_queue.front();
            node_queue.pop();
            //if (actual->m_flag>-1) continue;
            //actual->m_flag=labeler;
            //pixel = color(labeler);
            //output->set_at(actual, pixel);
            iterator i_neighbor_actual;
            i_neighbor_actual= actual;
            for(int i = 0 ; i< input->m_number_of_neighbors; ++i)
            {
                i_neighbor_actual.neighbor(&actual, i);
                if (i_neighbor_actual->m_flag>-1 || i_neighbor_actual->m_visited ) continue;
                if ( difference(i_neighbor_actual->m_data, actual->m_data)
                        < m_max_segmentation_difference )
                {
                    i_neighbor_actual->m_flag=labeler;
                    node_queue.push(i_neighbor_actual);
                }
            }
        }
    }
    //now we assign the respective color to out
    /*int col;
    for(iter = input->begin(); !iter.end(); iter++)
    {
        col=color(iter->m_flag);
        output->set_at(iter,col);
    }*/


}
#endif // SEGMENTATOR_H
