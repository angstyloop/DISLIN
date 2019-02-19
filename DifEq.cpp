// A differential equation solver class that uses newton's method
#include "/usr/local/dislin/DifEq.h"

// returns ith component of current state
double& DifEqSolver::operator[] (long int j) {
    return x[j];
}

// computes the next state using the velocity field v,
//      and increments the internal counter.
void DifEqSolver::Iterate (void) {
    for (int j=0; j<d; ++j)
        x[j] = a[j] + dt * (*v[j])(a); 
    for (int j=0; j<d; ++j)
        a[j] = x[j];
    ++i;
}

