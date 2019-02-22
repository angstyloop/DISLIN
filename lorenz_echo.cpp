// predicts the lorenz series using an echo state network

#include "/usr/local/dislin/Lorenz.h"

int main() {
    //ESN parameters
    int     wsteps  = 1000
        ,   tsteps  = 1000
        ,   steps   = 2000
        ,       N   = 100
    ;

    // make tray a dynamic array so we don't overload the stack
    double  *tray = new double[steps]
        ,      dt = .01         // Timestep 
        ,       b = .00001    // Ridge Regression parameter
    ;

    // tray is a timeline for printing with Dislin::curve
    tray[0] = 0;
    for (int j=1; j<steps; ++j)
        tray[j] = tray[j-1] + dt;

    // esn initial state
    Vector esn_start(N);
    esn_start.random(N, -1, 1);

    // lorenz initial state
    Vector a(3);
    a[0] = 1;
    a[1] = 1;
    a[2] = 1;

    DiscreteTimeSeries* data = new LorenzApprox (a, steps);
    
    EchoStateNetwork esn (esn_start, data, steps);
    
    // randomly generates internal matrices
    esn.RandomParms(.5, .5, 1);

    // sets rr parameter
    esn.SetB(b);

    // populate esn.series with steps number of values
    esn.Listen();

    //washes out the first wsteps values. moves remaining to front and zeroes openings.
    //  make sure wsteps < steps !
    esn.Wash(wsteps);

    // train for tsteps number of steps
    esn.Train(tsteps); //tsteps < steps-wsteps

    // NOTE: At this point we have initialized esn.W_out, and both esn.series and
    //  esn.in_series have one element.

    //for (int i=1; i<steps; ++i) {
    //    std::cout << (*data)[i][0] << std::endl;
    //}

    // Fill the array with predicted values ( a total of steps-1 iterations )
    esn.Predict();

    //const int indices_length = 1;
    //int indices[indices_length]= {2}; //observe the y component
    //esn.Observe(indices, indices_length);

    //for (int i=1; i<steps; ++i) {
    //    std::cout << (*data)[i][0] << std::endl;
    //}

    // initialize 2-d array to hold the original lorenz terms to compare to predicted
    double** og_series = new double*[data->Dim()];
    for (int i=0; i<data->Dim(); ++i)
        og_series[i] = new double[steps];

    // save original terms
    Save_OG(og_series, &esn);

    // initialize 2-d array to hold predicted series terms
    double** pred_series = new double*[data->Dim()];
    for (int i=0; i<data->Dim(); ++i)
        pred_series[i] = new double[steps];

    Save_Pred(pred_series, &esn);

    // initialize dislin and set it up
    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini(); 

    //g.incmrk(-1); //want points instead of smooth lines? uncomment.
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (tray[0], tray[0]+500*dt, tray[0], 1, -50, 0, -50, 10);

    // old code you might need again
    // g.curve(tray, xout, steps);
    // g.curve(tray, yout, steps);
    // g.curve(tray, zout, steps);

    // a temp array to use with Dislin::curve()
    double* inray = new double[steps];

    // plot z component of Lorenz series for steps number of steps
    for (int j=0; j<steps; ++j) {
        inray[j] = og_series[1][j];
        // let's just print instead.
        //std::cout<< inray[j] << std::endl;
    }
    g.color("BLUE");
    g.curve(tray, inray, steps);

    //plot z component predicted series
    for (int j=0; j<steps; ++j) {
        inray[j] = pred_series[1][j];
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
