#ifndef CMESHREGION_H_INCLUDED
#define CMESHREGION_H_INCLUDED
#include "CNode.h"
#include <math.h>
#include <vector>
#include <set>

using namespace std;


template<class G>
class CMeshRegion
{
public:
    typedef typename G::node node;

    CMeshRegion();
    virtual ~CMeshRegion();

    int m_label;
    int m_ncells;
    float m_area;
    float m_pattern;
    float m_index;
    set<int> m_neighbors_set;
    vector<CMeshRegion*> *m_pMeshRegionV;

    void Set_Neighbor(int _neighbor);
    void Incorporate(node *cell);
    void Init(int label, vector<CMeshRegion*> *_m_pMeshRegionV);
    void Set_Distance(int total_elements, int total_area);
};

template<class G>//mesh region comparator
struct mr_cmp{bool operator()(CMeshRegion<G>* a, CMeshRegion<G>* b){return a->m_index< b->m_index;}};

template<class G>
CMeshRegion<G>::CMeshRegion():m_label(0), m_ncells(0), m_area(0.0), m_pattern(0.0)
{

}

template<class G>
void CMeshRegion<G>::Set_Neighbor(int _neighbor){
    m_neighbors_set.insert(_neighbor);
    if (_neighbor< m_pMeshRegionV->size())
        (*m_pMeshRegionV)[_neighbor]->Set_Neighbor(m_label);
}

template<class G>
void CMeshRegion<G>::Set_Distance(int total_elements, int total_area){
    double x,y;
	x = m_area / total_area;		//0..1
	y = m_ncells/ total_elements;		//0..1
	m_index= sqrt(x*x + y*y);
}

int r=0;
template<class G>
void CMeshRegion<G>::Incorporate(node *cell)
{
        m_ncells++;
        m_area+=cell->m_area;
        //dilema
        //we must calculate teh average color pattern
        //based on number of cell or on area size?!?!
        m_pattern = (m_pattern * (m_area - cell->m_area) + cell->m_data * cell->m_area)/m_area;
        //if (!(r++ % 5000)) cout<<r-1<<" patern of region "<<m_label<<" is "<<m_pattern<<" w data: "<<cell->m_data<<endl;
        //is the weighted average
}

template<class G>
void CMeshRegion<G>::Init(int label, vector<CMeshRegion<G>*> *_m_pMeshRegionV)
{
    m_label=label;
    m_pMeshRegionV = _m_pMeshRegionV;
}

template<class G>
CMeshRegion<G>::~CMeshRegion()
{

}
#endif // CMESHREGION_H_INCLUDED
