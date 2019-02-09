#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
#include <boost/math/constants/constants.hpp>


double EchoStateNetwork::PlotRidgeTrace ()
{ 
    Dislin g;
    if (g.getlev () != 0){ 
        std::cout << "level must start at 0" << std::endl;
        std::exit(EXIT_FAILURE); 
    }

    int ymin = -10;
    int ymax = 10;
    double l_exp = -5;
    double r_exp = 3;
    int num_inc = pow(10,r_exp);                 //number of computed increments for RT 
    double db = 1;                 //stepsize for RidgeTrace

    // make a static array of b values
    double bvals[num_inc];
    bvals[0] = 0.00001;
    for (int i=1; i<num_inc; ++i)
        bvals[i]=i*db;
        
    // allocate an array of Matrix pointers to hold the W_out's; delete later;
    Matrix<double>** trace = new Matrix<double>*[num_inc]; 
    for (int i=0; i<num_inc; ++i)
        trace[i] = new Matrix<double>(this->in_series->Dim(), this->Dim()); // dimensions of W_out:
                                                      //(input_dimension, reservoir_dimension) 


    // get num_inc W_out's into trace 
    this->RidgeTrace(trace, num_inc);
    

    // now we need to get make an array for each entry in W_out.
    // lets make an array of these arrays (we're flattening out W_out over 50 b steps).
    double** W_out_entries = new double*[this->in_series->Dim() * this->Dim()]; // [input_dim * res_dim]
    for (int i=0; i<this->in_series->Dim(); ++i) {
        for (int j=0; j<this->Dim(); ++j) {
        W_out_entries[i*this->Dim()+j] = new double[num_inc];
            for (int k=0; k<num_inc; ++k) {
            W_out_entries[i*this->Dim()+j][k] = (*trace[k])[i][j];
            }
        }
    }

    // for example, W_out_entries[i], i=0..this->in_series->Dim()*N is an array of size num_inc
    //  that can be fed to dislin's plot(). first it must be inserted in a static array though.
    //  We'll reuse the same temporary static array over and over again.

    double yray[num_inc];

    g.metafl("CONS");
    g.setpag("USEL");
    //g.page(3000, 3000);
    
    //g.disini ();
    
    //g.pagera ();
    //g.hwfont ();
    //g.axspos (500, 2500);
    //g.axslen (2000, 2000);
    
  //g.name   ("X-axis", "x");
  //g.name   ("Y-axis", "y");

    //g.labdig (5, "x");
    //g.ticks  (1, "xy");

    //g.titlin ("Demonstration of CURVE", 1);
    //g.titlin ("SIN(X), COS(X)", 3);


    g.disini();
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.axsscl ("log", "x");
    g.graf (l_exp, r_exp, l_exp, 1, ymin, ymax, ymin, 0.5);
    g.axgit();

    g.chncrv ("COLOR");

    // loop through rows of W_out_entries plotting each
    for (int i=0; i<this->Dim()*this->in_series->Dim(); ++i) {
        for (int j=0; j<num_inc; ++j)
            yray[j] = W_out_entries[i][j];
        g.curve(bvals, yray, num_inc);
    }

    g.disfin();
    std::cout << "Enter ridge regression parameter: " << std::endl; 
    std::string s;
    double b;
    getline(std::cin, s);
    std::stringstream(s) >> b;
    return b;
}

// save 2-d array of Vectors from DiscreteTimeSeries *sine to a (tposed) 2-d array sine_series

void Save_Pred (double** pred_series, EchoStateNetwork* esn) {
    // save sine_series; write a function Save(double sine_series**, DiscreteTimeSeries* sine);
    for (int i=0; i<esn->In_Series_Dim(); ++i) {
        for (int j=0; j<esn->Steps(); ++j) {
            pred_series[i][j] = esn->In_Series(j)[i];
        }
    }
}

void Save_OG (double** og_series, EchoStateNetwork* esn) {
    for (int i=0; i<esn->In_Series_Dim(); ++i) {
        for (int j=0; j<esn->Steps(); ++j) {
            og_series[i][j] = esn->OG_Series(j)[i];
        }
    }
}

int main() {

    int wsteps = 100;
    int tsteps = 100;               //target number of training steps
    int steps = 200;                 //number of total steps
    int N =5;                      //number of nodes
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
   
        

    // check on sine_series

    //washes out the first wsteps values. moves remaining to front and zeroes openings.
    //  make sure wsteps < steps ! 
    esn.Wash(wsteps);


    // train for tsteps number of steps
    esn.Train(tsteps); //tsteps < steps-wsteps


    // at this point we should have W_out and an array with one value in it.
    
    // fill the array with predicted values ( a total of steps-1 iterations )
    esn.Predict(); 
   

    // some code for user input 
    //esn.SetB(esn.PlotRidgeTrace());
    //std::cout << "Enter ridge regression parameter: " << std::endl;
    //std::string s;
    //double b;
    //getline(std::cin, s);
    //std::stringstream(s) >> b;

   
    // starts observing at index curr, which is where Train() leaves off.
    // indices is an array of indices of input vectors that we want to observe.
    //const int indices_length = 1;
    //int indices[indices_length]= {0}; //observe the x component
    //esn.Observe(indices, indices_length);

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
        //std::cout<< 0+i*step_size << std::endl;
        xray[i] = 0 + (wsteps+tsteps+i)*step_size;

    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini();
    g.incmrk(-1);
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (xray[0], xray[steps-1], xray[0], 50, -2, 2, -2, .5);
     
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

   //std::cout<<"predicted: " << bakeray[0] << bakeray[1] << std::endl;
    g.color("RED");
    g.curve(xray, inray, steps);

    g.disfin();
    return 0;
} 

