// predict lorenz series

#include "/usr/local/dislin/Lorenz.h"
#include "/Users/allense/projects/thesis/rc/generateW.cpp"
#include "/Users/allense/projects/thesis/rc/generateWin.cpp"
#include "/Users/allense/projects/thesis/rc/trainRC.cpp"
#include "/Users/allense/projects/thesis/rc/runRC.cpp"

int main() {
    // simulate lorenz system

    const int   train_time = 10000
            ,   run_time = 3000
            ,   K    = 3 
            ,   L    = 3
            ,   d    = 3
            ,   N    = 100
            ,   washout = 1000
            ;

    FuncPtr* v = new FuncPtr[3];

    v[0] = L_x;
    v[1] = L_y;
    v[2] = L_z;

    // make these dynamic arrays so we don't overload the stack
    double  *xout = new double[train_time]
        ,   *yout = new double[train_time]
        ,   *zout = new double[train_time]
        //,   *tray = new double[train_time] // for dislin
        ,    A[d] = {.5, .6, .4}
        ,      dt = .01
        ,       p = .9
        ,       a = .5
        ,       dW = .5
        ,       dfb = .5
        ,       din = .5
        ,       zeta = 1
        ,       sigma = .8
        ,       beta = .01
        ;

    // make a dif eq solver object with
    //   dynamics v
    //   initial condition a
    //   train_timestep dt
    //   dimension of system
    
    DifEqSolver des (v, A, dt, d);

    // make a teacher matrix where the rows are input vectors (x,y,z)
    Matrix<double>* teacher_ptr = new Matrix<double> (train_time, L);
    Matrix<double>& teacher = *teacher_ptr;

    for (int i=0; i<train_time; ++i) {
        teacher[i][0] = des[0];
        teacher[i][1] = des[1];
        teacher[i][2] = des[2];
        des.Iterate();
    }
    
    // generate W
    Matrix<double>  
            *Wptr = generateW(N, p, dW)
        ,   &W    = *Wptr     
        ;

    // generate Wfb (the same way Win would be generated if we had input)
    Matrix<double>
            *Wfbptr = generateWin(N, K, dfb, sigma)
        ,   &Wfb    = *Wfbptr
        ;

    // generate Win
    Matrix<double>
            *Winptr = generateWin(N, K, din, sigma)
        ,   &Win    = *Winptr
        ;

    // generate the Result structure containing Wout and the last row of the state
    //  collection matrix M
    Result* res_ptr = trainRC(W, Wfb, N, K, L, teacher, washout, a, zeta, beta, train_time);

    // generate the "output collecting matrix"
    Matrix<double> *outputs_ptr = runRC(W, Wfb, res_ptr, N, K, L, run_time, a, zeta)
                ,  &outputs = *outputs_ptr
                ;

    // get each component into a separate array
    double* predicted_outputs_x = new double[run_time];
    double* predicted_outputs_y = new double[run_time];
    double* predicted_outputs_z = new double[run_time];

    for (int i=0; i<run_time; ++i) {
        predicted_outputs_x[i] = outputs[i][0];
        predicted_outputs_y[i] = outputs[i][1];
        predicted_outputs_z[i] = outputs[i][2];
    }

    // generate the next $run_time number of points in the Lorenz curve

    double* real_outputs_x = new double[run_time];
    double* real_outputs_y = new double[run_time];
    double* real_outputs_z = new double[run_time];
    
    for (int i=0; i<run_time; ++i) {
        real_outputs_x[i] = des[0];
        real_outputs_y[i] = des[1];
        real_outputs_z[i] = des[2];
        des.Iterate();
    }
    
    for (int i=0; i<run_time; ++i) {
        std::cout << real_outputs_x[i] << "    " << predicted_outputs_x[i] << std::endl;
    }

    //dislin stuff
    
    // need to make this a dynamic array so we don't overload the stack,
    double* tray = new double[train_time];
    tray[0] = 0;
    for (int j=1; j<train_time; ++j)
        tray[j] = tray[j-1] + dt;

    Dislin g;
    g.metafl("CONS");
    g.setpag("USEL");
    g.disini(); 

    //g.incmrk(-1); //points instead of smooth lines
    g.axslen (10000, 5000);
    g.axspos (500, 5500);
    g.graf (tray[0], tray[run_time-1], tray[0], 100*dt, -50, 50, -50, 50);


    // get dislin going and then...

    g.chncrv("LINE");
    g.color("RED");
    g.curve(tray, real_outputs_x, run_time);
    g.color("BLUE");
    g.curve(tray, real_outputs_x, run_time);
    
    //g.color("BLUE");
    //g.curve(tray, predicted_outputs_x, run_time);

    // curve(tray, yout);
    // curve(tray, zout);


    g.disfin();

    // clean up dynamically allocated arrays
    delete[] tray;
    delete[] v;
    delete[] xout;
    delete[] yout;
    delete[] zout;

    return 0;
}
