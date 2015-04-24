#include "CGraphImage2D.h"
#include <iostream>
#include <math.h>
#include "capputils.h"

using namespace std;

CAppUtils app;
int f(int d){
    //return d*d*d*sin(d)*cos(d)*tan(d)/(sin(d)*cos(d)*tan(d)  + 1);
    return d*d*d*sin(d)*cos(d)*tan(d);//(sin(d)*cos(d)*tan(d)  + 1);
    //return 1;
}
void matrix(int r, int c){
    cout<<"matrix:"<<endl;
    char **m=new char*[r];
    for(int i=0; i<r;++i)
        m[i]=new char[c];

    app.begin_counter();
    for(int i=0; i<r; ++i)
        for(int j=0; j<c; ++j){
            m[i][j] = f(m[i][j]);
        }
    app.show_duration();

    //destroy
    for(int i=0; i<r;++i)
        delete[] m[i];
    delete[] m;
}


void graph(int rows, int cols){
    cout<<"graph:"<<endl;
    CGraphImage2D<char> grafo;
    grafo.config(rows,cols);
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            CGraphImage2D<char>::node n;
            n.m_data = i*rows + j;
            grafo.m_matriz[i][j] = n;
        }
    }

    CGraphImage2D<char>::iterator iter;
    app.begin_counter();
    for(iter = grafo.begin(); iter!=grafo.end(); iter++){
        iter->m_data = f(iter->m_data);
    }
    app.show_duration();
}


int main(){
    for(int i=1; i<10000;i<<=1){
        cout<<"I:"<<i*i<<endl;
        graph(i, i);
        matrix(i, i);
        cout<<endl<<endl;
    }
}
