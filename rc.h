#include "MatrixLib.h"
#include <boost/multiprecision/cpp_dec_float.hpp>
typedef boost::multiprecision::cpp_dec_float_50 xdouble;
//typedef double xdouble;
xdouble pi = boost::math::constants::pi<xdouble>();

class DiscreteTimeSeries {
    protected:
        int prev, curr, steps, d;
        Vector** series;
    public:
        DiscreteTimeSeries ( Vector, const int& );
        virtual ~DiscreteTimeSeries () {if (series!=nullptr) delete[] series;}
        Vector& operator[] (const int);
        virtual void Map(void) =0; 
            //series[curr] = f (series[prev]);;
            //prev = curr++;=0
        
        // need this here for polymorphism with EchoStateNetwork
        virtual Vector O_Series (int i) { return (*this)[0]; }

        virtual void Wash (int);
        const int& Dim() const {return d;}
        Vector& Curr() {return (*this)[curr];}
        Vector& Prev() {return (*this)[prev];} // eq. to (*this)[PrevIndex()]
        int CurrIndex () {return curr;}
        int PrevIndex () {return prev;}
        void SetCurr(int i) {curr = i;}
        void SetPrev(int i) {prev = i;}
        void SetPrevComp(int i, double val) {(*this)[prev][i] = val;}
        void Listen(void);
        void PrintSeries();
        int Steps(void) {return steps;}
};

Matrix<double> RidgeRegress(Matrix<double>, Matrix<double>, double);

class EchoStateNetwork : public DiscreteTimeSeries {
    protected:
        DiscreteTimeSeries* in_series; 
        Vector** o_series;
        Matrix<double> W, W_in, W_out;
        const Vector offset;
        double b;
        double spec_rad = 0.9;
    public:
        EchoStateNetwork (Vector, DiscreteTimeSeries*, const int&);
        void Map(void);
        //fill M with rowvectors obt
        void Train(int);
        void Observe(int[], int);
        void PrintW_out(void) {W_out.Print();}
        void PrintTr_Series(void);
        void PrintPred_Series(void);
        Vector In_Series(int i) {return (*in_series)[i];}
        Vector O_Series (int i) {return *(o_series[i]);}
        void RidgeTrace(Matrix<double>**, int, double);
        void Wash(int);
        void Predict(void);
        void SetSpecRad (double rad) {spec_rad = rad;};
        double SpecRad (void) {return spec_rad;}

        void SetB (double _b) {b=_b;}
        double GetB () {return b;}
        void RandomParms (double, double, double); 
        double PlotRidgeTrace();
        void Listen();
        void Load(double**, int, int);
        void Drive(DiscreteTimeSeries*);
        int In_Series_Dim (void) {return in_series->Dim();}
};

class BakersMap : public DiscreteTimeSeries {
    protected:
        double a, c;
        xdouble x=xdouble(3)/xdouble(255);
        xdouble y=xdouble(192)/xdouble(255);

    public:
        BakersMap (Vector start, int steps, double _a, double _c)
            : DiscreteTimeSeries (start, steps), a (_a), c (_c) {}                
            
        void SetC (double _c) {c=_c;}
        double GetC () {return c;}
        void Map(void); 
};

// Ridge Regresssion Algorithm
// T : teacher matrix
// M : state matrix
// b : RR parameter
// returns the matrix (W_out) that minimizeds the RR error function 
Matrix<double> RidgeRegress(Matrix<double> T, Matrix<double> M, double b) {
    //build the matrix M * M.T() + b * I and invert
    Matrix<double> temp = M.T()*M;
    for (int i=0; i<temp.ncol; ++i)
        temp[i][i] += b;
    // invert temp and do the remaining matrix multiplications
    return T.T() * M * temp.inv_cholesky();
}

 // this is an important overload, since Vector** series is an array of Vector*,
 //   and the syntax to return a vector is cumbersome.
 Vector& DiscreteTimeSeries::operator[] (int i) {
     if (i>=0 && i<steps)
         return *(series[i]);
     std::cout << "DiscreteTimeSeries: index out of range." << std::endl;
     exit (EXIT_FAILURE);  
 }

// convert a double to a 1-d Vector
 const Vector doubleToVector(double d) {
    Vector* v = new Vector(1);
    (*v)[0] = d;
    return *v;
 }

// a generic class for functions of a single variable like sin(x)
class ScalarFunction : public DiscreteTimeSeries {
    // step size for generating {sin(i*stp_z)}
    double stp_sz;
    // generating function
    double (*f)(double);
    public:
        // constructor takes a function pointer and initializes base class
        ScalarFunction (double (*_f)(double), double _start, int _steps, double _stp_sz) 
            : DiscreteTimeSeries(doubleToVector(_start), _steps)
              , stp_sz (_stp_sz) 
              , f (_f) {}

        // pure virtual in DiscreteTimeSeries
        void Map(void);            
        double GetStepSize() {return stp_sz;}
        void SetStepSize(double _stp_sz) {stp_sz=_stp_sz;}
};

//debuggin 
/*int main() {
    Matrix<double> M (5,5);
    
}*/
