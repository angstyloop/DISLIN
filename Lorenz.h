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

class LorenzApprox : public DiscreteTimeSeries {
        DifEqSolver *des;
        FuncPtr* v   = new FuncPtr[3];
        double  a[3]
            ,   dt;
    public:
        LorenzApprox (Vector _a, int _steps, double _dt=.01)
            : DiscreteTimeSeries (_a, _steps) 
            , dt (_dt) {
                a[0] = _a[0]; //x0
                a[1] = _a[1]; //y0
                a[2] = _a[2]; //z0

                v[0] = L_x;
                v[1] = L_y;
                v[2] = L_z;
               
                des = new DifEqSolver (v, a, dt, d);
            }
        
        void Map() {
            // use dif eq solver obj to produce next
            //  approximation to a trajectory of
            //  the Lorenz system of ODE
            (*this)[curr][0] = (*des)[0];
            (*this)[curr][1] = (*des)[1];
            (*this)[curr][2] = (*des)[2];
            des->Iterate();

            //DifEqSolver des (v, a, dt, 3);
            prev = curr++;
        }
};
