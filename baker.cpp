#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
#include <iomanip>

int main ()
{
    int steps = 50;
    double x_0, y_0, a, c=2;
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
    double* x= new double[steps];
    double* y = new double[steps];
    double* n = new double[steps];
    for (int i=0; i<steps; ++i )
        n[i] = i;
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
     Dislin g;
     g.metafl("CONS");
     g.setpag("USEL");
     g.disini();
 
     //g.incmrk(-1); //want points instead of smooth lines? uncomment.
     g.axslen (10000, 5000);
     g.incmrk(-1);
     g.axspos (500, 5500);
     g.graf(0, steps, 0, 10, -1, 1, 1, 1);
     g.color("RED");
     g.curve(n, y, steps);

     g.color("BLUE");
     // change x to have dummy values decaying to zero
     for (int i=0; i<steps; ++i)
         y[] = y[8] * pow(.5, i);
     g.curve(n,x,steps);
     g.disfin();
}
