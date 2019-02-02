#include<iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"


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

int main() {
    int psteps = 100;               //target number of predicted steps
    int tsteps = 100;               //target number of training steps
    int steps = psteps + tsteps;    //number of total steps
    double step_size = .1;          //step size for ScalarFunction constructor
    int N = 5;                      //number of nodes
    double b = .0001;

    // Generate sine output, wash it, and put it in an array.
    DiscreteTimeSeries* sine = new ScalarFunction(sin, 0, steps, step_size);
    sine->Listen();

    Vector esn_start(N);            //reservoir initial state
    esn_start.random(N, -1, 1);

     double** sin_series = new double*[sine->Dim()];
     for (int i=0; i<sine->Dim(); ++i) {
         sin_series[i] = new double[psteps];
         for (int j=psteps; j<steps; ++j) {
             sin_series[i][j-psteps] = (*sine)[j][i];
         }
     }

    delete sine;
    
    // generate reservoir series, wash, ridgetrace, train, and
    //  get predicted output into an array
    sine = new ScalarFunction(sin, 0, steps, step_size);
    EchoStateNetwork esn (esn_start, sine, steps);
    esn.RandomParms(.5,.5);
    esn.Listen();
    esn.Wash(psteps);

    //esn.SetB(esn.PlotRidgeTrace());
    /*std::cout << "Enter ridge regression parameter: " << std::endl;
    std::string s;
    double b;
    getline(std::cin, s);
    std::stringstream(s) >> b;*/
    esn.SetB(b);

    esn.Train();
    esn.Predict();

    double** pred_series = new double*[sine->Dim()];

    for (int i=0; i<sine->Dim(); ++i) {
        pred_series[i] = new double[psteps];
        for (int j=tsteps; j<steps; ++j) {
            pred_series[i][j-tsteps] = (*sine)[j][i];
        }
    }
    
    //for (int i=0; i<sine->Dim(); ++i) {
    //    for (int j=0; j<psteps; ++j)
    //        std::cout << pred_series[i][j] << "   " << sin_series[i][j] << std::endl;
    //    std::cout << std::endl;
    //}
    

    double xray[psteps];
    double yray[psteps];
    
    for (int i=tsteps; i<steps; ++i)
        //std::cout<< 0+i*step_size << std::endl;
        xray[i-tsteps] = 0 + i*step_size;
    
    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini();
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (xray[0], xray[psteps-1]+1, xray[0], .5, -1, 1, -1, 0.5);
    //g.axgit();

    //plot predicted series
    for (int j=0; j<psteps; ++j) 
        yray[j] = pred_series[0][j];
    g.color("RED");
    g.curve(xray, yray, psteps);
     
    // plot sin series
    for (int j=0; j<psteps; ++j)
        yray[j] = sin_series[0][j];
    g.color("BLUE");
    g.curve(xray, yray, psteps);
   
    g.disfin();
    return 0;
}
