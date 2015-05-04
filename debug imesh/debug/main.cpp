#include "CGraphImage2D.h"
#include "CGraphImage3D.h"
#include "CGraphMeshND.h"
#include <iostream>
#include <math.h>
#include "capputils.h"

using namespace std;

CAppUtils app;

int f(int d){
    //return d*d*d*sin(d)*cos(d)*tan(d)/(sin(d)*cos(d)*tan(d)  + 1);
    //return d*d*d*sin(d)*cos(d)*tan(d);//(sin(d)*cos(d)*tan(d)  + 1);
    return 1;
}
char f_ch(int d){
    return 'A' + d;
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

void test2d(){
    for(int i=1; i<10000;i<<=1){
        cout<<"I:"<<i*i<<endl;
        graph(i, i);
        matrix(i, i);
        cout<<endl<<endl;
    }
}

void matrix3d(int r, int c, int l){
    cout<<"matrix:"<<endl;
    char ***m=new char**[r];
    for(int i=0; i<r;++i){
        m[i]=new char*[c];
        for(int j=0; j<c;++j)
            m[i][j]=new char[l];
    }

    app.begin_counter();
    for(int i=0; i<r; ++i)
        for(int j=0; j<c; ++j)
            for(int k=0; k<l; ++k)
                m[i][j][k] = f(m[i][j][k]);
    app.show_duration();

    //destroy
    for(int i=0; i<r;++i){
        for(int j=0; j<c; ++j)
            delete[] m[i][j];
        delete[] m[i];
    }

    delete[] m;
}

void graph3d(int rows, int cols, int lays){
    cout<<"graph:"<<endl;
    CGraphImage3D<char> grafo;
    grafo.config(rows, cols, lays);
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            for(int k = 0; k < lays; ++k){
                CGraphImage3D<char>::node n;
                n.m_data = i*rows + j*cols + k;
                grafo.m_matriz[i][j][k] = n;
            }


    CGraphImage3D<char>::iterator iter;
    app.begin_counter();
    int i=0;
    for(iter = grafo.begin(); iter!=grafo.end(); iter++){
        //cout<<"inside for "<<++i<<endl;
        iter->m_data = f(iter->m_data);
    }
    //cout<<"out"<<endl;
    app.show_duration();
}

void test3d(){
    for(int i=3; i<500;i+=10){
        cout<<"I:"<<i*i*i<<endl;
        matrix3d(i, i, i);
        graph3d(i, i, i);
        cout<<endl<<endl;
        //cin.get();
    }
}

void test_neighbors_3d(){
    CGraphImage3D<char> grafo;
    int rows=3, cols=3, lays=3;
    grafo.config(rows, cols, lays);
    char a='A';
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            for(int k = 0; k < lays; ++k){
                CGraphImage3D<char>::node n;
                n.m_data = a++;//'A' + i*rows + j*cols + k;
                grafo.m_matriz[i][j][k] = n;
            }
    CGraphImage3D<char>::iterator iter;
    for(iter = grafo.begin(); iter!=grafo.end();iter++)
        cout<<iter->m_data<<endl;
    iter = grafo.begin();
    for(int i=0; i< 13;++i)iter++;
    cout<<"element in cuestion:"<<iter->m_data<<endl;
    for(int i=0; i< grafo.m_number_of_neighbors; ++i){
        cout<<"i:"<<iter.neighbor_at(i)->m_data<<endl;
    }
}

void test_ND(){
    CGraphMeshND<char> grafo;
    grafo.load_data("..\\..\\Data\\meshin.txt");
    cout<<"loaded!!"<<endl;
    CGraphMeshND<char>::iterator iter;
    app.begin_counter();int i=0;
    for( iter = grafo.begin(), i=0; iter!=grafo.end(); iter++, i++){
        //cout<<"inside for "<<++i<<endl;
        //iter->m_data = f_ch(iter->m_data);
        cout<<"node:"<<i<<" data:"<<iter->m_data<<" neighbors:"<<endl;
        CGraphMeshND<char>::iterator neighbor_iter;
        for(int n=0; n< grafo.m_number_of_neighbors; ++n){
            neighbor_iter =  iter.neighbor_at(n);
            if (neighbor_iter->m_label != INVALID_NEIGHBOR_LABEL_VALUE){
                cout<<"->"<<neighbor_iter->m_data<<" ";
            }
        }
        cout<<endl;
    }
    //cout<<"out"<<endl;
    app.show_duration();

}


int main(){
    //test_neighbors_3d();
    test_ND();

}
