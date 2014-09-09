#ifndef CMESHREGION_H_INCLUDED
#define CMESHREGION_H_INCLUDED
#include "CNode.h"

using namespace std;
template<class G>
class CMeshRegion{
public:
    typedef typename G::node node;
    CMeshRegion();
    virtual ~CMeshRegion();
    int m_label;
    int m_ncells;
    float m_area;
    void Incorporate(node *cell);
    void Init(int label);
};
template<class G>
CMeshRegion<G>::CMeshRegion():m_label(0), m_ncells(0), m_area(0.0)
{
}
template<class G>
void CMeshRegion<G>::Incorporate(node *cell)
{
        m_ncells++;
        m_area+=cell->m_area;
}
template<class G>
void CMeshRegion<G>::Init(int label)
{
    m_label=label;
}
template<class G>
CMeshRegion<G>::~CMeshRegion()
{

}
#endif // CMESHREGION_H_INCLUDED
