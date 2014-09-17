#ifndef CMESHREGION_H_INCLUDED
#define CMESHREGION_H_INCLUDED
#include "CNode.h"
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
    set<int> m_neighbors_set;
    void set_neighbor(int _neighbor);
    void Incorporate(node *cell);
    void Init(int label, vector<CMeshRegion*> *_m_pMeshRegionV);
    vector<CMeshRegion*> *m_pMeshRegionV;
};

template<class G>
CMeshRegion<G>::CMeshRegion():m_label(0), m_ncells(0), m_area(0.0), m_pattern(0.0)
{

}

template<class G>
void CMeshRegion<G>::set_neighbor(int _neighbor){
    m_neighbors_set.insert(_neighbor);
    if (_neighbor< m_pMeshRegionV->size())
        (*m_pMeshRegionV)[_neighbor]->set_neighbor(m_label);
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

        if (!(r++ % 5000)) cout<<r-1<<" patern of region "<<m_label<<" is "<<m_pattern<<" w data: "<<cell->m_data<<endl;
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
