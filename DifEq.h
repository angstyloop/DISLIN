//#include <iostream>
#include <cmath>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"

typedef double (*FuncPtr)(double*);

class DifEqSolver {

    FuncPtr *v;   // pointer to array of velocity functions

    double  *a   // pointer to array of previous state, init. to init. conds.
        ,   *x              // pointer to array of current state
        ,   dt              // time increment
        ;
    
    int d                   // dimension of system
      , i                   // step counter
      ;
         
    public:

        DifEqSolver (FuncPtr* _v, double _a[], double _dt, int _d)
            :   v (_v)
            ,   dt(_dt)
            ,   d (_d)
            ,   i (0) {
            x = new double[d]; 
            a = new double[d];
            for (int j=0; j<d; ++j)
                x[j] = a[j] = _a[j];
        }

        double& operator[] (long int);
        void Iterate (void);
        int Steps();
        
};
