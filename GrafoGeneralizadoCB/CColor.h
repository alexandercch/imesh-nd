#ifndef CCOLOR_H
#define CCOLOR_H
#include <cstdlib>
#include <time.h>
using namespace std;
#define PALLET_SIZE 1000
class CColor
{
public:
    CColor();
    virtual ~CColor();
    int operator()(int i);
protected:
private:
    int pallete[PALLET_SIZE];
    void generate();
};
CColor::CColor()
{
    generate();
}
CColor::~CColor() {}

void CColor::generate()
{
    srand(time(0));
    for(int i=0; i< PALLET_SIZE; ++i)//generate random colors
        pallete[i]=0xff000000 | (0xffffff & rand());
}

int CColor::operator()(int i)
{
    return pallete[i % PALLET_SIZE ];//we return i-th color mod size of pallet
}

#endif // CCOLOR_H
