// Prints the Lorenz series.

#include "/usr/local/dislin/Lorenz.h"

int main() {
    // simulate lorenz system

    const int NUMIT = 30000;
    
    const int d = 3;

    FuncPtr* v = new FuncPtr[3];

    v[0] = L_x;
    v[1] = L_y;
    v[2] = L_z;

    // make these dynamic arrays so we don't overload the stack
    double  *xout= new double[NUMIT]
        ,   *yout= new double[NUMIT]
        ,   *zout= new double[NUMIT]
        ,   *tray= new double[NUMIT]
        ,    a[d]= {1, 1, 1}
        ,      dt= .001
        ;

    // make a dif eq solver object with
    //   dynamics v
    //   initial condition a
    //   timestep dt
    //   dimension of system
    DifEqSolver des (v, a, dt, d);

    for (int i=0; i<NUMIT; ++i) {
        xout[i] = des[0];
        yout[i] = des[1];
        zout[i] = des[2];
        des.Iterate();
    }

    // do stuff with xout, yout, and zout.    
    // e.g. print with dislin
    
    // need to make this a dynamic array so we don't overload the stack,
    tray[0] = 0;
    for (int j=1; j<NUMIT; ++j)
        tray[j] = tray[j-1] + dt;

    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini(); // this is the issue; i must be running out of stack?

    //g.incmrk(-1); //points instead of smooth lines
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (tray[0], tray[NUMIT-1], tray[0], 1000, -30, 30, -30, 10);


    // get dislin going and then...

    g.curve(tray, xout, NUMIT);
    // curve(tray, yout);
    // curve(tray, zout);


    g.disfin();

    // clean up dynamically allocated arrays
    delete[] v;
    delete[] xout;
    delete[] yout;
    delete[] zout;
    delete[] tray;

    return 0;
}
