#ifndef SEGMENTATOR_H
#define SEGMENTATOR_H
#include <cstdio>
#include <map>
#include <set>
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
    int m_nregions;
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
    set<CMeshRegion<G>*, mr_cmp<G> > m_less_index_mr_set;

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

                if (i_neighbor_actual->m_visited) continue;
                if (i_neighbor_actual->m_label>-1){
                    //we add the neighbor of the region
                    if (i_neighbor_actual->m_label != iter->m_label){
                        mr->Set_Neighbor(i_neighbor_actual->m_label);
                        //mr->m_neighbors_set.insert(i_neighbor_actual->m_label);
                        //cout<<"region "<<i_neighbor_actual->m_label<<" added "
                        //    <<iter->m_label<<" as neighbor"<<endl;
                    }
                    continue;
                }

                if ( gray_difference(i_neighbor_actual->m_data, actual->m_data)
                        < m_max_segmentation_difference )
                {
                    i_neighbor_actual->m_label=labeler;
                    node_queue.push(i_neighbor_actual);
                }
            }
        }
        mr->Set_Distance(totalelements, totalarea);
        m_meshregionV.push_back(mr);
    }
}

template<class G>
void CSegmentator<G>::group_neighbor_regions(){
    //the code goes here :3
    int totalarea= input->area(), totalelements=input->weight();

    set<CMeshRegion<G>*, mr_cmp<G> > mr_priority_set(m_meshregionV.begin(), m_meshregionV.end());
    typename set<CMeshRegion<G>*, mr_cmp<G> >::iterator siter;
    typename CMeshRegion<G>::NeighborSet::iterator neighbor_iter;
    cout<<"ASmV:"<<endl;
    cout<<"m_nregions: "<<m_nregions<<endl;

    while(mr_priority_set.size() > m_nregions){
        siter = mr_priority_set.begin();

        bool first=true;
        double min_dist, curr_dist;
        int min_index;
        //we find the most similar neighbor region
        cout<<":3"<<endl;
        for(neighbor_iter = (*siter)->m_neighbors_set.begin();
            neighbor_iter != (*siter)->m_neighbors_set.end();
            ++neighbor_iter){
            cout<<":P"<<endl;
            if (first){
                first = false;
                min_dist = fabs((*siter)->m_pattern - (m_meshregionV[*neighbor_iter])->m_pattern);
                min_index=(m_meshregionV[*neighbor_iter])->m_index;
                cout<<"xD"<<endl;
                continue;
            }
            cout<<":P"<<endl;
            curr_dist = fabs((*siter)->m_pattern - (m_meshregionV[*neighbor_iter])->m_pattern);
            if (curr_dist < min_dist){
                min_dist = curr_dist;
                min_index = (*neighbor_iter);
            }
            cout<<":P"<<endl;
        }
        cout<<"mergin regions:"<<(*siter)->m_label<<" - "
            <<(m_meshregionV[min_index])->m_label<<endl;

        //remove that region :3
        CMeshRegion<G>* new_region = m_meshregionV[min_index];
        new_region->Incorporate(*siter);
        mr_priority_set.erase(*siter);
        mr_priority_set.erase(m_meshregionV[min_index]);
        new_region->Set_Distance(totalelements, totalarea);
        mr_priority_set.insert(new_region);
    }


    /*cout<<"size:"<<mr_priority_set.size()<<endl;
    typename set<CMeshRegion<G>*, mr_cmp<G> >::iterator it=mr_priority_set.begin();
    cout<<"hello"<<endl;
    do{
        cout<<(*it)->m_index<<" cells: "<<(*it)->m_ncells<<" area :"<<(*it)->m_area<<endl;
    }while(++it!= mr_priority_set.end());
    */
    //for(int i=0; i< m_meshregionV.size(); ++i){
    //    mr_priority_set.insert(m_meshregionV[i]);
    //}


}


template<class G>//this code if debuging :3
void CSegmentator<G>::show_mesh_region(){
    cout<<"Results of mesh region: ("<<m_meshregionV.size()<<" regions..)"<<endl;
    cout<<"id:\tarea\tlabel\tncells\tmpattern\t[neighbors, ...]"<<endl;
    FORVZ(m_meshregionV){
        cout<<i<<":\t"<<m_meshregionV[i]->m_area<<"\t"
            <<m_meshregionV[i]->m_label<<"\t"
            <<m_meshregionV[i]->m_ncells<<"\t";
            printf("%x",(int) m_meshregionV[i]->m_pattern);
            cout<<"\t"<<"\t";
            set<int>::iterator sit;
            for(sit =  m_meshregionV[i]->m_neighbors_set.begin();
                sit != m_meshregionV[i]->m_neighbors_set.end(); ++sit)
                cout<<" "<<*sit<<",";
            cout<<endl;
            cout<<endl;
    }
}



#endif // SEGMENTATOR_H
