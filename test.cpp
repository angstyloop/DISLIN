#include <iostream>
#include "/usr/local/dislin/discpp.h"  

int main()
{
    const int MAX = 2000000;
    double*  arr1 = new double[MAX];
    //double arr2[MAX];
    for (int i=0; i<MAX; ++i)
        /*arr2[i] =*/ arr1[i] = 5.0;
    Dislin g;
    g.disini();
    return 0;
}
