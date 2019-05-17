#include "/usr/local/dislin/Lorenz.h"

int main () {
    int N_max = 100;
    int tsteps = 1000;
    int steps = 2000;
    Vector esn_start(N_max);
    esn_start.RandomReals();
    Vector a (3);
    a[0] = a[1] = a[2] = 1;
    DiscreteTimeSeries* data = new LorenzApprox(a, steps);
    EchoStateNetwork esn(esn_start, data, steps);
    std::cout<<"starting Tune"<<std::endl;
    esn.Tune();
    std::cout<<"Tune OK"<<std::endl;
    esn.Listen();
    std::cout<<"Listen OK"<<std::endl;
    esn.Train(tsteps);
    //std::cout<<"Train OK"<<std::endl;
    // predict with optimal parameters
    esn.Predict();
    
    // make tray a dynamic array so we don't overload the stack
    double  *tray = new double[steps]
        ,      dt = .01         // Timestep
    ;

    // tray is a timeline for printing with Dislin::curve
    tray[0] = 0;
    for (int j=1; j<steps; ++j)
        tray[j] = tray[j-1] + dt;

    // initialize 2-d array to hold the original lorenz terms to compare to predicted
    double** og_series = new double*[data->Dim()];
    for (int i=0; i<data->Dim(); ++i)
        og_series[i] = new double[steps];

    // save original terms
    esn.Save_OG(og_series);

    // initialize 2-d array to hold predicted series terms
    double** pred_series = new double*[data->Dim()];
    for (int i=0; i<data->Dim(); ++i)
        pred_series[i] = new double[steps];

    esn.Save_Pred(pred_series);

    // initialize dislin and set it up
    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini();

    //g.incmrk(-1); //want points instead of smooth lines? uncomment.
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (tray[0], tray[0]+2000*dt, tray[0], 500, -50, 50, -50, 10);

    // a temp array to use with Dislin::curve()
    double* inray = new double[steps];

    // plot x component of Lorenz series for steps number of steps
    for (int j=0; j<steps; ++j) {
        inray[j] = og_series[0][j];
        // let's just print instead.
        //std::cout<< inray[j] << std::endl;
    }
    g.color("BLUE");
    g.curve(tray, inray, steps);

    std::cout<<std::endl;

    //plot x component predicted series
    for (int j=0; j<steps; ++j) {
        inray[j] = pred_series[0][j];
        // let's just print instead
        //std::cout<< inray[j] << std::endl;
    }
    g.color("RED");
    g.curve(tray, inray, steps);

    // output to screen
    g.disfin();
    
    // clean up dynamically allocated arrays
    delete[] tray;
    delete[] inray;

    return 0;
}    


