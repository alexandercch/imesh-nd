#ifndef CAPPUTILS_H
#define CAPPUTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>  /* defines FILENAME_MAX */

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

//this class will provide statistic, last, conversion and another useful functions
//in order to dont show this in the main file keep the main file clean :)
class CAppUtils
{
public:
    CAppUtils();
    ~CAppUtils();
    //members
    clock_t m_startTime;

    void begin_counter();
    void show_duration();
    void show_working_directory();
    static string int_to_str(int n);

private:
};


void CAppUtils::begin_counter()
{
    m_startTime = clock();
};
void CAppUtils::show_duration()
{
    cout << double( clock() - m_startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
};
void CAppUtils::show_working_directory()
{
    char cCurrentPath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        printf("error: %d", rand());
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    printf ("The current working directory is %s", cCurrentPath);
};

string CAppUtils::int_to_str(int n)
{
    char *buffer= new char[64];
    sprintf(buffer,"%d", n );
    string res;
    res= buffer;
    delete[] buffer;
    return res;
}

CAppUtils::CAppUtils()
{
};

CAppUtils::~CAppUtils()
{
};



#endif //CAPPUTILS_H
