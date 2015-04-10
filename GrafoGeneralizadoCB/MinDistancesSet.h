// MinDistancesSet.h: interface for the CMinDistancesSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINDISTANCESSET_H__2A9D4EEC_A614_48CF_A500_E1E5E755B07F__INCLUDED_)
#define AFX_MINDISTANCESSET_H__2A9D4EEC_A614_48CF_A500_E1E5E755B07F__INCLUDED_

#include <vector>
#include <set>

using namespace std;

template <class Obj>
class CMinDistancesSet
{
private:
//............................................................................................................
    //template <class Obj>
    struct CSetKey
    {
        typedef CSetKey/*<Obj>*/		Self;
        typedef /*const*/ Self*		SCPtr;
        CSetKey(Obj* _pObj1 = NULL, Obj* _pObj2 = NULL, int _x = 0, int _y = 0)
        {
            pObj1 = _pObj1;
            pObj2 = _pObj2;
            x = _x;
            y = _y;
        }
        bool operator< (const Self& mr) const
        {
            return fabs(*pObj1 - *pObj2) < fabs(*(mr.pObj1) - *(mr.pObj2));
        }

        Obj *pObj1, *pObj2;
        int x,y;
    };

    struct PLessCPtr
    {
        //typedef /*const*/ Self*		SCPtr;
        bool operator() (const CSetKey* c1, const CSetKey* c2)
        {
            return (*c1 < *c2);
        }
    };
//............................................................................................................
private:
    typedef CSetKey/*<Obj>*/							Set_key;
    typedef multiset<Set_key*, PLessCPtr>	Set;
    typedef typename Set::iterator							Set_it;
public:
    typedef vector<Obj*>							Obj_vector;

public:
    CMinDistancesSet();
    ~CMinDistancesSet();
    void Init(Obj_vector* pVector);
    int GetLessDistPair(Obj*& pObj1, Obj*& pObj2);
    int UpdtLessDistPair();
    void Erase(int x, int y);
    void Index(int x, int y);
    inline int Size();
    void PrintState();

private:
    Set			m_Set;
    Set_it**	m_pMatrix;
    Obj_vector*	m_pVector;
    int			m_nObjs;

};

template <class Obj>
CMinDistancesSet<Obj>::CMinDistancesSet()
{
    m_pMatrix = NULL;
    m_pVector = NULL;
    m_nObjs	  = 0;
}

template <class Obj>
CMinDistancesSet<Obj>::~CMinDistancesSet()
{
    if ( !m_pVector ) return;

    int i, j, size = m_nObjs = m_pVector->size();
    //destroying all the distances
    for ( j = 0; j <= size-2; j++ )
        for ( i = 0; i <= j; i++ )
            if ( m_pMatrix[j][i] != Set_it(NULL) )
                Erase(i,j);

    //destroying the matrix
    for ( j = 0; j < size-1; j++ )
        delete m_pMatrix[j];
    delete m_pMatrix;
}

template <class Obj>
void CMinDistancesSet<Obj>::Init(Obj_vector* pVector)
{
    m_pVector = pVector;
    int i, j, size = m_nObjs = m_pVector->size();
    m_pMatrix = new Set_it*[size-1];
    for ( j = 0; j < size-1; j++ )
        m_pMatrix[j] = new Set_it[j+1];

    for ( j = 0; j <= size-2; j++ )
        for ( i = 0; i <= j; i++ )
            m_pMatrix[j][i] = Set_it(NULL);//NULL;*/
    //making all the distances
    for ( j = 0; j <= size-2; j++ )
        for ( i = 0; i <= j; i++ )
            Index(i,j);
}

template <class Obj>
int CMinDistancesSet<Obj>::Size()
{
    return m_nObjs;
}

template <class Obj>
int CMinDistancesSet<Obj>::GetLessDistPair(Obj*& pObj1, Obj*& pObj2)
{
    if ( !m_nObjs )
        return 0;
    //assert(m_Set.size() > 0);
    typename Set::const_iterator ci;
    ci = m_Set.begin();
    pObj1 = (*ci)->pObj1;
    pObj2 = (*ci)->pObj2;
    /*int x,y;
    x = (*ci)->x;
    y = (*ci)->y;*/
    return m_nObjs;
}

template <class Obj>
int CMinDistancesSet<Obj>::UpdtLessDistPair()
{
    //updt obj1 , kill obj2

    typename Set::const_iterator ci;
    int x,y,i,j,n;

    ci = m_Set.begin();
    x = (*ci)->x;
    y = (*ci)->y;
    n = m_pVector->size();
    (*m_pVector)[y+1] = NULL;

    //Erasing distances of the objects obj1(x) and obj2(y)
    //y
    for ( i = 0; i < y+1; i++ )		//	---*----
        Erase(i,y);
    for ( j = y+1; j < n-1; j++ )	//			|
        Erase(y+1,j);

    //x
    for ( i = 0; i < x; i++ )		//	----
        Erase(i,x-1);
    for ( j = x; j < n-1; j++ )		//		|
        Erase(x,j);

    //Indexing distances of the obj1(x)
    //x
    for ( i = 0; i < x; i++ )		//	----
        Index(i,x-1);
    for ( j = x; j < n-1; j++ )		//		|
        Index(x,j);

    return --m_nObjs;
}

template <class Obj>
void CMinDistancesSet<Obj>::Index(int x, int y)
{
    Obj *ox, *oy;
    ox = (*m_pVector)[x];
    oy = (*m_pVector)[y+1];
    if ( !ox || !oy )
        return;

    Set_key* pKey = new Set_key(ox, oy, x, y);
    Set_it it = m_Set.insert(pKey);
    m_pMatrix[y][x] = it;	//i,j -> physical coordinate
}

template <class Obj>
void CMinDistancesSet<Obj>::Erase(int x, int y)
{
    Set_it& it = m_pMatrix[y][x];
    if ( it == Set_it(NULL) )
        return;
    Set_key* pKey = *it;
    /*int s2, s1;
    s1 = m_Set.size();
    m_Set.erase(it);
    s2 = m_Set.size();*/
    //assert(s2+1 == s1);
    it = Set_it(NULL);//NULL;//null
    delete pKey;
}

template <class Obj>
void CMinDistancesSet<Obj>::PrintState()
{
    typename Set::iterator it;

    it = m_Set.begin();
    if ( it == Set_it(NULL) )
        return;
    printf("\n\n(%d,%d)[%.1lf] ", (*it)->x, (*it)->y, *((*it)->pObj1) - *((*it)->pObj2) );

    printf("\n");
    int i, j, size;
    size = m_pVector->size();
    for ( j = 0; j < size-1; j++ )
    {
        for ( i = 0; i <= j; i++ )
        {
            it = m_pMatrix[j][i];
            if ( it == Set_it(NULL) )
                printf("*");
            else
            {
                Set_key *key = *it;
                printf("%.1lf", *(key->pObj1) - *(key->pObj2) );
            }
            printf("\t");
        }
        printf("\n");
    }
}

#endif // !defined(AFX_MINDISTANCESSET_H__2A9D4EEC_A614_48CF_A500_E1E5E755B07F__INCLUDED_)
