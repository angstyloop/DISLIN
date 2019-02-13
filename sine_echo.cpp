#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
#include <boost/math/constants/constants.hpp>
//xdouble pi = boost::math::constants::pi<xdouble>();

double novel_function (double x) {
    return .5;
}

int main() {

    int wsteps = 100;
    int tsteps = 100;               //target number of training steps
    int steps = 200;                 //number of total steps
    int N =20;                      //number of nodes
    double step_size = .1;          //step size for ScalarFunction constructor
    double b = .0000001; 

    // Generate sine output, wash it, and put it in an array.
    
    Vector esn_start(N);            //reservoir initial state
    esn_start.random(N, -1, 1);

    DiscreteTimeSeries* sine = new ScalarFunction(sin, 0, steps, step_size);
    EchoStateNetwork esn (esn_start, sine, steps);
    esn.RandomParms(.5,.5, 1);
    
    esn.SetB(b);

    // populate esn.series with steps number of values
    esn.Listen();
   

    //washes out the first wsteps values. moves remaining to front and zeroes openings.
    //  make sure wsteps < steps ! 
    esn.Wash(wsteps);


    // train for tsteps number of steps
    esn.Train(tsteps); //tsteps < steps-wsteps


    // at this point we should have W_out and an array with one value in it.
    
    // fill the array with predicted values ( a total of steps-1 iterations )
    //esn.Predict(); 
   
    // Drive the esn with a novel input series, stored as a 2d array where the columns are vectors.
    
    // Make a novel series. in this case, a sine wave with half the amplitude.
    DiscreteTimeSeries* novel_series = new ScalarFunction(novel_function, 0, steps, step_size);

    // use the novel input series to drive the esn
    //esn.Drive(novel_series);

    // starts observing at index curr, which is where Train() leaves off.
    // indices is an array of indices of input vectors that we want to observe.
    const int indices_length = 1;
    int indices[indices_length]= {0}; //observe the x component
    esn.Observe(indices, indices_length);

    // note: when using Drive(), pred_series is the original series, and og_series is the response
    
    // initialize 2-d array to hold the original sine series terms during prediction period
    double** og_series = new double*[novel_series->Dim()];
    for (int i=0; i<novel_series->Dim(); ++i) 
        og_series[i] = new double[steps];
    
    // initialize 2-d array to hold predicted series terms 
    double** pred_series = new double*[novel_series->Dim()];
    for (int i=0; i<novel_series->Dim(); ++i) 
        pred_series[i] = new double[steps];

    Save_Pred(pred_series, &esn);
    Save_OG(og_series, &esn);
   
    double xray[steps];

    for (int i=0; i<steps; ++i)
        //std::cout<< 0+i*step_size << std::endl;
        xray[i] = 0 + (wsteps+tsteps+i)*step_size;

    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini();
    g.incmrk(-1);
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (xray[0], xray[steps-1], xray[0], 1, -1, 1, -2, .5);
     
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
    //for (int i=0; i<psteps; ++i)
    //    nray[i]+=shift;

    //plot x component predicted series
    for (int j=0; j<steps; ++j) {  
        inray[j] = pred_series[0][j];
        //std::cout<< inray[j] << std::endl;
    }
    g.color("RED");
    g.curve(xray, inray, steps);

    for (int j=0; j<steps; ++j)
        std:: cout << "response: " << og_series[0][j] << "    "
                   << "drive: " << pred_series[0][j] << std::endl;


    g.disfin();
    return 0;
} 

