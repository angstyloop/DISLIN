#include<iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"

double MyFunc(double x) { return sin(2*x)*exp(-x/10); }

int main() {
    int wsteps = 50;
    int tsteps = 50;               //target number of training steps
    int steps = 100;    //number of total steps
    double step_size = .1;          //step size for ScalarFunction constructor
    int N = 5;                      //number of nodes
    double b = .0000000000001;

    // Generate sine output, wash it, and put it in an array.
    DiscreteTimeSeries* sine = new ScalarFunction(MyFunc, 0, steps, step_size);

    Vector esn_start(N);            //reservoir initial state
    esn_start.random(N, -1, 1);
    EchoStateNetwork esn (esn_start, sine, steps);

    esn.RandomParms(.2,.2, 1);
    esn.SetB(b); 

    esn.Listen();

    esn.Wash(wsteps);

    esn.Train(tsteps);

    esn.Predict();

    // initialize 2-d array to hold the original sine series terms during prediction period
    double** og_series = new double*[sine->Dim()];
    for (int i=0; i<sine->Dim(); ++i)
        og_series[i] = new double[steps]; 

    // initialize 2-d array to hold predicted series terms 
    double** pred_series = new double*[sine->Dim()];
    for (int i=0; i<sine->Dim(); ++i)
        pred_series[i] = new double[steps];

    Save_Pred(pred_series, &esn);
    Save_OG(og_series, &esn);
 
    double xray[steps];
 
    for (int i=0; i<steps; ++i)
        xray[i] = 0 + (wsteps+tsteps+i)*step_size;
    
    
    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini();
    g.axslen (10000, 5000);
    g.incmrk(-1);
    g.axspos (500, 5500);
    g.graf (xray[0], xray[steps-1], xray[0], .5, -1, 1, -1, 0.5);
    //g.axgit();

    double inray[steps];
     
    // plot x component of sine series (it's the only component)
    for (int j=0; j<steps; ++j) {
        inray[j] = og_series[0][j];
        //std::cout << inray[j] << std::endl;
    }
    g.color("BLUE");
    g.curve(xray, inray, steps); 

    // try to account for offset: experimental
    //double shift = 3.75;
    //
    //for (int i=0; i<psteps; ++i)
    //    xray[i]+=shift;

    //plot x component predicted series
    for (int j=0; j<steps; ++j) {
        inray[j] = pred_series[0][j];
        //std::cout<< inray[j] << std::endl;
    }
    g.color("RED");
    g.curve(xray, inray, steps);

    g.disfin();
    return 0;
}
