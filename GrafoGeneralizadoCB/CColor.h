#ifndef CCOLOR_H
#define CCOLOR_H
#include <math.h>
#include <cstdlib>
#include <time.h>
using namespace std;
#define PALLET_SIZE 30

typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

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
    int get_hue(int _color);
};
CColor::CColor()
{
    generate();
}
CColor::~CColor() {}
inline int clampAndConvert(float v)
{
    if(v < 0)return v+255;
    if( v > 255)return v-255;
    return v;
}

int CColor::get_hue(int _color){
    int red=_color&0x00ff0000;
    int green=_color&0x0000ff00;
    int blue=_color&0x000000ff;
    red>>=16;
    green>>=8;

    float min_ = min(min(red, green), blue);
    float max_ = max(max(red, green), blue);

    float hue = 0.0f;
    if (max_ == red) {
        hue = (green - blue) / (max_ - min_);

    } else if (max_ == green) {
        hue = 2.0f + (blue - red) / (max_ - min_);

    } else {
        hue = 4.0f + (red - green) / (max_ - min_);
    }

    hue = hue * 60;
    if (hue < 0) hue = hue + 360;

    return round(hue);
}


void CColor::generate()
{
    srand(time(0));
    pallete[0]=rand()*rand();
    int hue;
    int R, G, B;
    for(int i=1; i< PALLET_SIZE; ++i){//generate random colors
        hue=get_hue(pallete[i-1]);
        unsigned char r = (pallete[i-1] & 0x00ff0000)>>16;;
        unsigned char g = (pallete[i-1] & 0x0000ff00)>>8;
        unsigned char b = (pallete[i-1] & 0x000000ff);
        RgbColor co={r,g,b};
        hue = clampAndConvert(hue);
        cout<<"hue:"<<hue<<endl;
        cout<<"Colors:"<<(int)r<<" "<<(int)g<<" "<<(int)b<<endl;
        HsvColor hs=RgbToHsv(co);
        hs.h= (unsigned char)hue;
        co=HsvToRgb(hs);
        R=co.r;
        G=co.g;
        B=co.b;
        cout<<"Colors:"<<R<<" "<<G<<" "<<B<<endl;
        pallete[i]=(R<<16)|(G<<8)|B;
    }
}

int CColor::operator()(int i)
{
    return pallete[i % PALLET_SIZE ];//we return i-th color mod size of pallet
}

#endif // CCOLOR_H
