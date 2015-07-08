/**
    Seminario de Tesis 10
    CSegmentator.h
    Purpose: This an implementation of the imesh algorithm, but it is oriented to
             the generalized structure.

    @author Alex Ccacyahuillca
    @version 1.0 04/015
*/
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

#define MAX_SEGMENTATION_DIFFERENCE 20

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

    G *input;
    G *output;

    int cntr;
    int m_segmentation_difference;
    int m_nregions_asmv;
    int m_nregions_asms;

    vector<CMeshRegion<G>* > m_meshregionV;

    CColor handle_color;

    //methods
    CSegmentator();
    CSegmentator(G *_input, G *_output);
    virtual ~CSegmentator();

    //segmentation algorithms
    void binary_segmentation();
    void group_neighbor_cells();
    void group_neighbor_regions();
    void group_similar_regions();

    //image preprocessing
    void to_gray_scale();

    //debugging subroutines
    void show_mesh_region();

protected:
private:
    //private members
    set<CMeshRegion<G>*, mr_cmp<G> > m_less_index_mr_set;

};

template<class G>
CSegmentator<G>::CSegmentator():cntr(0), m_segmentation_difference(MAX_SEGMENTATION_DIFFERENCE)
{
};

template<class G>
CSegmentator<G>::CSegmentator(G *_input, G *_output):input(_input),output(_output),cntr(0), m_segmentation_difference(MAX_SEGMENTATION_DIFFERENCE)
{
};

template<class G>
CSegmentator<G>::~CSegmentator() {};

template<class G>
void CSegmentator<G>::binary_segmentation()
{
    iterator iter;
    for(iter = input->begin(); iter != input->end(); iter++)
    {
        iter->m_data = handle_color.process_pixel_binary(iter->m_data);
    }
}

template<class G>
void CSegmentator<G>::to_gray_scale()
{
    iterator iter ;
    for(iter = input->begin(); iter != input->end(); iter++)
    {
        iter->m_data = handle_color.process_gray_scale(iter->m_data);
    }
}

template<class G>
void CSegmentator<G>::group_neighbor_cells()
{
    int current_label_value =   DEFAULT_LABEL_VALUE;
    int totalarea           =   input->area();
    int totalelements       =   input->weight();

    iterator iter;
    int nn=1;
    for(iter = input->begin(); iter != input->end(); iter++)
    {
        //if (nn++ %10000 ==0)

        if (iter->m_label != DEFAULT_LABEL_VALUE) {continue;}
        iter->m_label = ++current_label_value;
        //cout<<"f";
        queue<iterator> node_queue;
        node_queue.push(iter);
        //cout<<"f";
        CMeshRegion<G> *mr= new CMeshRegion<G>();
        mr->Init(current_label_value, &m_meshregionV);
        //cout<<"f";
        while(node_queue.size())
        {
            //cout<<"w";
            //cout<<"hello"<<endl;
            iterator actual =node_queue.front();
            node_queue.pop();
            //cout<<"w";
            mr->Incorporate(*actual);
            //cout<<"w";
            iterator neighbor;
            for(int i = 0 ; i< input->m_number_of_neighbors; ++i)
            {
                neighbor = actual.neighbor_at(i);
                //cout<<"i";
                if (neighbor->m_label == INVALID_NEIGHBOR_LABEL_VALUE)
                    continue;
                //cout<<"i";
                if (neighbor->m_label > -1)
                {
                    if (neighbor->m_label != iter->m_label)
                        mr->Set_Neighbor(neighbor->m_label);
                    continue;
                }
                //cout<<"i";
                if ( handle_color.rgb_difference(neighbor->m_data, actual->m_data)
                        < m_segmentation_difference )
                {
                    neighbor->m_label=current_label_value;
                    node_queue.push(neighbor);
                }
                //cout<<"i";
            }
        }
        mr->Set_Distance(totalelements, totalarea);
        m_meshregionV.push_back(mr);
    }
}

template<class G>
void CSegmentator<G>::group_neighbor_regions()
{
    //the code goes here :3
    int totalarea= input->area(), totalelements=input->weight();

    multiset<CMeshRegion<G>*, mr_cmp<G> > mr_priority_set(m_meshregionV.begin(), m_meshregionV.end());

    typename multiset<CMeshRegion<G>*, mr_cmp<G> >::iterator siter;
    typename CMeshRegion<G>::NeighborSet::iterator neighbor_iter;

    CMeshRegion<G>* min_neighbor;
    //int niterations=0;
    while(mr_priority_set.size() > m_nregions_asmv)
    {
        //niterations++;
        bool first=true;
        double min_dist, curr_dist;
        int min_index;

        siter = mr_priority_set.begin();
        for(neighbor_iter = (*siter)->m_neighbors_set.begin();
            neighbor_iter != (*siter)->m_neighbors_set.end();
            ++neighbor_iter)
        {
            if ((*neighbor_iter)->Is_Overlaped())continue;
            if (first)
            {
                first       = false;
                min_dist    = handle_color.rgb_difference((*siter)->m_pattern, (*neighbor_iter)->m_pattern);
                min_index   = (*neighbor_iter)->m_label;
                min_neighbor= m_meshregionV[(*neighbor_iter)->m_label];
                continue;
            }
            curr_dist = handle_color.rgb_difference((*siter)->m_pattern , (*neighbor_iter)->m_pattern);
            if (curr_dist < min_dist)
            {
                min_dist    = curr_dist;
                min_index   = (*neighbor_iter)->m_label;
                min_neighbor= m_meshregionV[(*neighbor_iter)->m_label];
            }
        }
        if (first)
        {
            cout<<"region alone found!"<<endl;
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
void CSegmentator<G>::group_similar_regions()
{

    CMinDistancesSet<CMeshRegion<G> > similarity_set;

    vector<CMeshRegion<G>* > v;
    for(int i=0; i< m_meshregionV.size(); ++i)
    {
        if (!(m_meshregionV[i]->Is_Overlaped()))
            v.push_back(m_meshregionV[i]);
    }
    similarity_set.Init(&v);
    while(similarity_set.Size() > m_nregions_asms)
    {
        CMeshRegion<G> *s1, *s2;
        similarity_set.GetLessDistPair(s1, s2);
        s1->Incorporate(s2);
        similarity_set.UpdtLessDistPair();
        //show_mesh_region();
    }
}

template<class G>//this code if debuging :3
void CSegmentator<G>::show_mesh_region()
{
    cout<<"Results of mesh region: ("<<m_meshregionV.size()<<" regions..)"<<endl;
    cout<<"id/lbl:\tarea\tindex\t\tncells\tmpattern\toverlaped\t[neighbors, ...]"<<endl;
    FORVZ(m_meshregionV)
    {
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
