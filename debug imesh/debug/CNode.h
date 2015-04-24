#ifndef CNODE_H
#define CNODE_H
#define OUT_OF_REGION -5

#define DEFAULT_FLAG_VALUE -1
#define DEFAULT_AREA_VALUE 1.0
#define DEFAULT_VISITED_VALUE false

template<class T>
class CNode
{
public:
    CNode();
    CNode(const CNode &_node );
    CNode(T _data);
    virtual ~CNode();

    void operator=(CNode &_node);

    T m_data;
    //bool m_visited;
    //int m_label;//will hold the label number assigned
    //float m_area;
protected:
private:
};

template<class T>
CNode<T>::CNode()//:m_visited(DEFAULT_VISITED_VALUE), m_label(DEFAULT_FLAG_VALUE), m_area(DEFAULT_AREA_VALUE)
{
}

template<class T>
CNode<T>::CNode(T _data)//:m_visited(DEFAULT_VISITED_VALUE), m_label(DEFAULT_FLAG_VALUE), m_area(DEFAULT_AREA_VALUE)
{
}

template<class T>
CNode<T>::CNode(const CNode<T> &_node ):m_data(_node.m_data)//,m_visited(_node.m_visited),m_label(_node.m_label), m_area(_node.m_area)
{
}

template<class T>
void CNode<T>::operator=(CNode &_node)
{
    m_data      = _node.m_data;
    /*m_visited   = _node.m_visited;
    m_label     = _node.m_label;
    m_area      = _node.m_area;*/
}

template<class T>
CNode<T>::~CNode()
{
    //dtor
}

#endif // CNODE_H
