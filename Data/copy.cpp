#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

string int_to_str(int n){
    char *buffer= new char[64];
    sprintf(buffer,"%d", n );
    string res;
    res= buffer;
    delete[] buffer;
    return res;
}

int main() {
    string input, output;
    int n;
    cout<<"nombre del archivo:";
    cin>>input;
    cout<<"cuantas veces desea copiarlo?:";
    cin>>n;
    clock_t start, end;
    start = clock();
    ifstream source(input.c_str(), ios::binary);
    // file size
    source.seekg(0, ios::end);
    ifstream::pos_type size = source.tellg();
    source.seekg(0);
    // allocate memory for buffer
    char* buffer = new char[size];

    // copy file
    source.read(buffer, size);
    string name, ext, newname;
    int i= input.find('.');
    name=input.substr(0, i);
    ext=input.substr(i, input.size());
    string snumber;
    for(int i=0; i< n; ++i){
        snumber= int_to_str(i);
        newname= name + snumber + ext;
        ofstream dest(newname.c_str(), ios::binary);
        dest.write(buffer, size);
        dest.close();
        cout<<newname<<" created!!"<<endl;
    }
    // clean up
    delete[] buffer;
    source.close();
    //dest.close();
    return 0;
}
