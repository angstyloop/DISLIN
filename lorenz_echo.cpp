// predicts the lorenz series using an echo state network

#include "/usr/local/dislin/Lorenz.h"

int main() {

    const int NUMIT = 10000; 
    
    const int d = 3;

    FuncPtr* v = new FuncPtr[3];

    v[0] = L_x;
    v[1] = L_y;
    v[2] = L_z;

    // make tray a dynamic array we don't overload the stack
    double  *tray= new double[NUMIT]
        ,    a[d]= {1, 1, 1}
        ,      dt= .01
        ;

    // make a dif eq solver object with
    //   dynamics v
    //   initial condition a
    //   timestep dt
    //   dimension of system
    DifEqSolver des (v, a, dt, d);
    // makes a DataSeries object (child of DiscreteTimeSeries)
    // to hold a number NUMIT of Vector objects of dimension d.
    DataSeries data (d, NUMIT);
    for (int i=0; i<NUMIT; ++i) {
        data[i][0] = des[0]; 
        data[i][1] = des[1];
        data[i][2] = des[2];
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

    //g.curve(tray, xout, NUMIT);
    // curve(tray, yout);
    // curve(tray, zout);


    g.disfin();

    // clean up dynamically allocated arrays
    delete[] v;
    delete[] tray;

    return 0;
}
