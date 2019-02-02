#include<iostream>
#include <cmath>
#include <cstring>
#include <string>
#include "/usr/local/dislin/discpp.h"
#include "/Users/allense/projects/thesis/rc/rc.cpp"
Dislin g;

int main ()
{ 
    if (g.getlev () != 0){ 
        std::cout << "level must start at 0" << std::endl;
        std::exit(EXIT_FAILURE); 
    }

    int psteps = 50;               //target number of predicted steps
    int ymin = -10;
    int ymax = 10;
    double l_exp=-5;
    double r_exp = 3;
    int tsteps = 100;               //target number of training steps
    int steps = psteps + tsteps;    //number of total steps
    int N = 3;                      //number of nodes
    int num_inc=pow(10,r_exp);                 //number of computed increments for RT 
    double db = 1;                 //stepsize for RidgeTrace
    Vector esn_start(N);            //reservoir initial state
    esn_start.random(N, -1, 1);

    DiscreteTimeSeries* sine = new ScalarFunction(sin, 0, steps);
    EchoStateNetwork esn (esn_start, sine, steps);
    esn.Listen();
    esn.Wash(psteps);

    // make a static array of b values
    double bvals[num_inc];
    bvals[0] = 0.00001;
    for (int i=1; i<num_inc; ++i)
        bvals[i]=i*db;
        
    // allocate an array of Matrix pointers to hold the W_out's; delete later;
    Matrix<double>** trace = new Matrix<double>*[num_inc]; 
    for (int i=0; i<num_inc; ++i)
        trace[i] = new Matrix<double>(sine->Dim(), N); // dimensions of W_out:
                                                      //(input_dimension, reservoir_dimension) 


    // get num_inc W_out's into trace 
    esn.RidgeTrace(trace, num_inc);
    

    // now we need to get make an array for each entry in W_out.
    // lets make an array of these arrays (we're flattening out W_out over 50 b steps).
    double** W_out_entries = new double*[sine->Dim() * N]; // [input_dim * res_dim]
    for (int i=0; i<sine->Dim(); ++i) {
        for (int j=0; j<N; ++j) {
        W_out_entries[i*N+j] = new double[num_inc];
            for (int k=0; k<num_inc; ++k) {
            W_out_entries[i*N+j][k] = (*trace[k])[i][j];
            }
        }
    }

    // for example, W_out_entries[i], i=0..sine->Dim()*N is an array of size num_inc
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
    for (int i=0; i<N*sine->Dim(); ++i) {
        for (int j=0; j<num_inc; ++j)
            yray[j] = W_out_entries[i][j];
        g.curve(bvals, yray, num_inc);
    }

    g.disfin();

    /*do {
        g.disfin();
        g.disini();
        g.axslen (10000, 5000);
        g.axspos (500, 5500);
        g.axsscl ("log", "x");
        g.graf (l_exp, r_exp, l_exp, 1, -4.0, 4.0, -2.0, 0.5);
        g.csrmod("STANDARD","POS");
        ++r_exp;
        //++l_exp;
         
        // loop through rows of W_out_entries plotting each
        for (int i=0; i<N*sine->Dim(); ++i) {
            for (int j=0; j<num_inc; ++j)
                yray[j] = W_out_entries[i][j];
            g.curve(bvals, yray, num_inc);
        }
    } while (g.csrpos(dummy,dummy)!=0);
    */
    /*     


  


    //load up yray
    //for (int j=0; j<num_inc; ++j)
    //    yray[j] = W_out_entries[0][j];

    //g.title  ();
    g.incmrk(0);
    //g.color  ("red");


    //g.curve  (bvals, yray, num_inc);
    //g.color  ("green");

    // reuse yray
    //for (int j=0; j<num_inc; ++j)
    //    yray[j] = W_out_entries[1][j];
    //g.curve  (bvals, yray, num_inc);

        //g.color  ("fore");
    //g.dash   ();
    //g.xaxgit ();

    //g.endgrf();
    //g.xaxlg(-5.,0.,-5.,1.,10000,"x",0,bvals[0],yray[0]);
    g.disfin ();

    return 0;
*/
}

