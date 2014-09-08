#ifndef CNODE_H
#define CNODE_H
#define OUT_OF_REGION -5
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
    bool m_visited;
    int m_flag;
protected:
private:
};

template<class T>
CNode<T>::CNode():m_visited(false), m_flag(-1)
{
}

template<class T>
CNode<T>::CNode(T _data):m_data(_data), m_visited(false), m_flag(-1)
{
}

template<class T>
CNode<T>::CNode(const CNode<T> &_node ):m_data(_node.m_data),m_visited(_node.m_visited),m_flag(_node.m_flag)
{
}

template<class T>
void CNode<T>::operator=(CNode &_node)
{
    m_data = _node.m_data;
    m_visited = _node.m_visited;
    m_flag = _node.m_flag;
}

template<class T>
CNode<T>::~CNode()
{
    //dtor
}

#endif // CNODE_H
