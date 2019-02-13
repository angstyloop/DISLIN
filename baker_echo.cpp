#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
#include <boost/math/constants/constants.hpp>

double novel_function (double x) {
    return .5;
}
int main() {
    xdouble pi = boost::math::constants::pi<xdouble>();

    int wsteps = 1000;
    int tsteps = 1000;               //target number of training steps
    int steps = 2000;                 //number of total steps
    int N =5;                      //number of nodes
    double b = .0000001; 
    double c = 2;  //side length of baker domain square

    // Generate bm output, wash it, and put it in an array.
    Vector rando(1);
    rando.random(1,.01,.49);
    double a = 1./3;;
    
    Vector bm_start(2);
    Vector esn_start(N);            //reservoir initial state
    esn_start.random(N, -1, 1);

    DiscreteTimeSeries* bm = new BakersMap(bm_start, steps, a, c);
    EchoStateNetwork esn (esn_start, bm, steps);
    esn.RandomParms(.5,.5, 1);
    
    esn.SetB(b);
    ((BakersMap*)bm)->SetC(c); 

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
    DiscreteTimeSeries* novel_series = new BakersMap(bm_start, steps, 2*a/5, c); 
  
    // use the novel input series to drive the esn 
    esn.Drive(novel_series); 
   
    // starts observing at index curr, which is where Train() leaves off.
    // indices is an array of indices of input vectors that we want to observe.
    //const int indices_length = 1;
    //int indices[indices_length]= {0}; //observe the x component
    //esn.Observe(indices, indices_length);

    // initialize 2-d array to hold the original bakers series terms during prediction period
    double** og_series = new double*[novel_series->Dim()];
    for (int i=0; i<novel_series->Dim(); ++i) 
        og_series[i] = new double[steps];

    // initialize 2-d array to hold predicted series terms 
    double** pred_series = new double*[novel_series->Dim()];
    for (int i=0; i<novel_series->Dim(); ++i) 
        pred_series[i] = new double[steps];

    Save_Pred(pred_series, &esn);
    Save_OG(og_series, &esn);
   
    double nray[steps];
    for (int i=0; i<steps; ++i)
        nray[i] = i;

    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini();
    g.incmrk(-1);
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (nray[0], 75, nray[0], 10, -5, 5, -1, .5);
     
    double inray[steps];
    // plot x component of bakers series
    for (int j=0; j<steps; ++j) {
        inray[j] = og_series[0][j];
        //std::cout<< inray[j] << std::endl;
    }
    g.color("BLUE");
    g.curve(nray, inray, steps);

  
    // try to account for offset: experimental
    //double shift = 3.75;
    //for (int i=0; i<psteps; ++i)
    //    nray[i]+=shift;

    //plot x component predicted series
    for (int j=0; j<steps; ++j) {  
        inray[j] = pred_series[0][j];
        std::cout<< inray[j] << std::endl;
    }

   //std::cout<<"predicted: " << bakeray[0] << bakeray[1] << std::endl;
    g.color("RED");
    g.curve(nray, inray, steps);

    g.disfin();
    return 0;
} 

