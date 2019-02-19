// defines the Lorenz equationsn

#include "/usr/local/dislin/DifEq.cpp"

// Lorenz equations

double L_x (double* vars) {
    const double    x = vars[0]
                ,   y = vars[1]
              //,   z = vars[2]
                ,   o  = 10
                ;
    return o * ( y - x );
}

double L_y (double* vars) {
    const double    x = vars[0]
                ,   y = vars[1]
                ,   z = vars[2]
                ,   p  = 28
                ;
    return x * ( p - z ) - y;
}

double L_z (double* vars) {
    const double    x = vars[0]
                ,   y = vars[1]
                ,   z = vars[2]
                ,   B  = 8./3
                ;
    return x * y - B * z;
}

