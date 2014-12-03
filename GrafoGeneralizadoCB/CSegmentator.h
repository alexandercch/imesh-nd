#ifndef SEGMENTATOR_H
#define SEGMENTATOR_H

#include <cstdio>
#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <vector>

#include "CMeshRegion.h"
#include "MinDistancesSet.h"
#include "CColor.h"

#define FORVZ(x) for(int i = 0;i<x.size(); ++i)

using namespace std;
//in future this is intended to be the nd-imesh
template<class G>
class CSegmentator
{
public:
    //members
    typedef typename G::iterator iterator;
    typedef typename G::node node;

    int cntr;
    int m_max_segmentation_difference;
    int m_nregions;
    vector<CMeshRegion<G>* > m_meshregionV;
    CColor color;
    G *input;
    G *output;

    //methods
    CSegmentator();
    CSegmentator(G *_input, G *_output);
    virtual ~CSegmentator();

    //segmentation algorithms
    void binary_segmentation();
    void group_neighbor_cells();
    void group_neighbor_regions();
    void group_similar_regions();

    //imagen preprocesing
    void to_gray_scale();

    //debuging subroutines
    void show_mesh_region();

protected:
private:
    //private members
    set<CMeshRegion<G>*, mr_cmp<G> > m_less_index_mr_set;

    //processes to handle pixel by pixel
    void init();
    inline int rgb_difference(unsigned int a, unsigned int b);
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
    int labeler=-1;
    iterator iter ;
    int totalarea= input->area(), totalelements=input->weight();

    for(iter = input->begin(); !iter.end(); iter++)
    {
        if (iter->m_label !=-1) continue;
        queue<iterator> node_queue;
        iter->m_label=++labeler;
        CMeshRegion<G> *mr= new CMeshRegion<G>();
        mr->Init(labeler, &m_meshregionV);
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
                if (i_neighbor_actual->m_visited || i_neighbor_actual->m_label>-1){

                        if (i_neighbor_actual->m_label>-1 && i_neighbor_actual->m_label != iter->m_label)
                            mr->Set_Neighbor(i_neighbor_actual->m_label);
                        //continue;

                    continue;
                }

                if ( rgb_difference(i_neighbor_actual->m_data, actual->m_data)
                        < m_max_segmentation_difference )
                {
                    i_neighbor_actual->m_label=labeler;
                    node_queue.push(i_neighbor_actual);
                }
            }
        }
        mr->Set_Distance(totalelements, totalarea);
        //cout<<labeler<<"-"<<endl;
        m_meshregionV.push_back(mr);
    }
}

template<class G>
void CSegmentator<G>::group_neighbor_regions(){
    //the code goes here :3
    int totalarea= input->area(), totalelements=input->weight();

    multiset<CMeshRegion<G>*, mr_cmp<G> > mr_priority_set(m_meshregionV.begin(), m_meshregionV.end());

    typename multiset<CMeshRegion<G>*, mr_cmp<G> >::iterator siter;
    typename CMeshRegion<G>::NeighborSet::iterator neighbor_iter;

    CMeshRegion<G>* min_neighbor;
    int niterations=0;
    while(mr_priority_set.size() > m_nregions){
        niterations++;
        siter = mr_priority_set.begin();
        bool first=true;
        double min_dist, curr_dist;
        int min_index;
        for(neighbor_iter = (*siter)->m_neighbors_set.begin();
            neighbor_iter != (*siter)->m_neighbors_set.end();
            ++neighbor_iter){
            if ((*neighbor_iter)->Is_Overlaped())continue;
            if (first){
                first = false;
                min_dist = rgb_difference((*siter)->m_pattern, (*neighbor_iter)->m_pattern);
                min_index=(*neighbor_iter)->m_label;
                min_neighbor= m_meshregionV[(*neighbor_iter)->m_label];
                continue;
            }
            curr_dist = rgb_difference((*siter)->m_pattern , (*neighbor_iter)->m_pattern);
            if (curr_dist < min_dist){
                min_dist = curr_dist;
                min_index = (*neighbor_iter)->m_label;
                min_neighbor= m_meshregionV[(*neighbor_iter)->m_label];
            }
        }
        if (first){
            mr_priority_set.erase(*siter);
            continue;
        }
        CMeshRegion<G>* new_region = m_meshregionV[min_index];
        new_region->Incorporate(*siter);
        mr_priority_set.erase(siter);
        mr_priority_set.erase(min_neighbor);
        new_region->Set_Distance(totalelements, totalarea);
        mr_priority_set.insert(new_region);
    }
}

template<class G>// :3
void CSegmentator<G>::group_similar_regions(){

    CMinDistancesSet<CMeshRegion<G> > similarity_set;

    vector<CMeshRegion<G>* > v;
    for(int i=0; i< m_meshregionV.size(); ++i){
        if (!(m_meshregionV[i]->Is_Overlaped()))
            v.push_back(m_meshregionV[i]);
    }
    similarity_set.Init(&v);
    while(similarity_set.Size() > m_nregions){
        CMeshRegion<G> *s1, *s2;

        similarity_set.GetLessDistPair(s1, s2);
        s1->Incorporate(s2);
        similarity_set.UpdtLessDistPair();
        //show_mesh_region();
    }
}

template<class G>//this code if debuging :3
void CSegmentator<G>::show_mesh_region(){
    cout<<"Results of mesh region: ("<<m_meshregionV.size()<<" regions..)"<<endl;
    cout<<"id/lbl:\tarea\tindex\t\tncells\tmpattern\toverlaped\t[neighbors, ...]"<<endl;
    FORVZ(m_meshregionV){
        cout<<i<<":\t"<<m_meshregionV[i]->m_area<<"\t"
            <<m_meshregionV[i]->m_index<<"\t"
            <<m_meshregionV[i]->m_ncells<<"\t";
            printf("%x\t\t",(int) m_meshregionV[i]->m_pattern);
            cout<<m_meshregionV[i]->m_overlap<<"\t";
            cout<<"\t";
            typename CMeshRegion<G>::NeighborSet::iterator sit;
            for(sit =  m_meshregionV[i]->m_neighbors_set.begin();
                sit != m_meshregionV[i]->m_neighbors_set.end(); ++sit)
                cout<<" "<<(*sit)->m_label<<",";
            cout<<endl;
            cout<<"ov regions:";
            for(int j = 0; j <  m_meshregionV[i]->m_overlaped_mr_ids.size(); ++j)
                cout<<"\t"<<m_meshregionV[i]->m_overlaped_mr_ids[j];
            cout<<endl;
            cout<<endl;
    }
}



#endif // SEGMENTATOR_H
