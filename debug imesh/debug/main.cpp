#include "CGraphImage2D.h"
#include <iostream>

using namespace std;

int main(){
    CGraphImage2D<int> grafo;
    int rows = 3, cols = 3;
    grafo.config(rows,cols);
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            CGraphImage2D<int>::node n;
            n.m_data = i*rows + j;
            grafo.m_matriz[i][j] = n;
        }
    }
    CGraphImage2D<int>::iterator iter;
    for(iter = grafo.begin(); iter!=grafo.end(); iter++){
        cout<<"p:"<<endl;
        cout<<iter->m_data<<endl;
    }
    //CGraphImage2D<int>::iterator iter2;
    for(iter = grafo.begin(); iter!=grafo.end(); iter++){
        cout<<"p:"<<endl;
        cout<<iter->m_data<<endl;
    }

}
