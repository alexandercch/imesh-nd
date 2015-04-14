#include "CGraphImage2D.h"
#include <iostream>

using namespace std;

int main(){
    CGraphImage2D<int> grafo;
    int rows = 2, cols = 2;
    grafo.config(rows,cols);
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            grafo.m_matriz[i][j] = i*rows + j;
        }
    }

}
