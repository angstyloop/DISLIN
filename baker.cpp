#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
int main ()
{
    int steps = 100;
    double x_0, y_0, a;
    //dx = dy = 1;
    a = 1./8;
    x_0 = 1./3;
    y_0 = 1./3;
    double* x= new double[steps];
    double* y = new double[steps];
    Vector<double> start(2);
    start[0] = x_0;
    start[1] = y_0;
    
    BakersMap b(start, steps, a);
    b.run();
    for (int i=0; i<steps; ++i) {
        x[i] = b[i][0];     
        y[i] = b[i][1];
    }
    Dislin g;   
    g.metafl("XWIN");
    g.page(3000, 3000);
    g.disini();
    g.axspos(500, 2500);
    g.axslen(2000, 2000);
    g.labdig(2, "x");
    g.ticks(10, "xy");
    g.graf(0, 1.01, 0, .5, 0, 1.01, 0, .5);
    g.incmrk(-1);
    //g.color("red");
    g.curve(x, y, steps);
    g.disfin();
}
