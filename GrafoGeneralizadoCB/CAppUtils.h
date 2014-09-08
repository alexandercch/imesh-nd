#ifndef CAPPUTILS_H
#define CAPPUTILS_H

#include <iostream>
#include <fstream>
#include <time.h>
#include <direct.h>
#include <cstdio>
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
    if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
        printf("error: %d", errno);
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    printf ("The current working directory is %s", cCurrentPath);
};
CAppUtils::CAppUtils()
{
};

CAppUtils::~CAppUtils()
{
};



#endif //CAPPUTILS_H
