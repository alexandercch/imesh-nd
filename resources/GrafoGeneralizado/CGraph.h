#ifndef CGRAPH_H
#define CGRAPH_H
#include "CNode.h"
#include "CImage.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
//CGraph es la carcaza que pretendemos hacer 
template<class T>
class CGraph
{
public:
    typedef CNode<T> node;
    //methods
    CGraph();
    virtual ~CGraph();    
    void load_data(string filename);
    int weight();//number of nodes    
protected:
private:
};
template<class T>
CGraph<T>::CGraph()
{
    //ctor
}
template<class T>
CGraph<T>::~CGraph()
{
    //dtor
}

#endif // CGRAPH_H
