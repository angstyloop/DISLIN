#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
#include <iomanip>
int main ()
{
    int steps = 75;
    xdouble x_0, y_0, a, c=2;
    //dx = dy = 1;
    //generate a random value
    Vector rando(1);
    rando.random(1,.1,.49);
    a = rando[0];
    rando.random(1,-1,1);
    x_0 = rando[0];
    rando.random(1,-1,1);
    y_0 = rando[0];
    std::cout<<x_0 << " " << y_0<<std::endl<<std::endl;
    xdouble* x= new xdouble[steps];
    xdouble* y = new xdouble[steps];
    Vector start(2);
    start[0] = x_0;
    start[1] = y_0;
    
    BakersMap b(start, steps, .5, c);
    b.Listen();
    for (int i=0; i<steps; ++i) {
        x[i] = b[i][0];     
        y[i] = b[i][1];
        std::cout << std::setprecision(10);
        std::cout<< x[i] << " " << y[i] <<std::endl;
    }
    /*Dislin g;   
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
    g.disfin();*/
}
