#ifndef CURVES_H
#define CURVES_H


#include <list>
#include <vector>
#include <utility>
#include <string>
#include <random>
#include <ctime>
#include "../fred/include/curve.hpp"

using namespace std;

// Class that stores the curves from the input file
class TimeSeries {

    private:
        double delta;
        double frequency;
        vector<pair<double,double>> tGrid; //Each available tx and ty for snapping

        list<pair<string, vector<double>>> curves;  // TimeSeries from the input file
        
    public:
        TimeSeries(double d,double freq, int L);

        pair<string, vector<double>> * insert(string id, vector<double> & p);
        
        pair<string, vector<double>> & getCurve(unsigned int i);
        
        inline unsigned int size(){return curves.size();}
        
        vector<double>  vectorization(vector<pair<double, double>> &f);
        
        vector<pair<double, double>> snappingDiscrete(vector<double> &y, int j);
        
        vector<double> snappingContinuous(vector<double> & y);
        
        vector<double> filtering(vector<double> &timeSerie);

        vector<double> minimaxima(vector<double> &timeSerie);

        void padVector(vector<double> &v, bool discrete=true);
        
        vector<pair<string, double>> findRealDistBruteForce_Discrete( vector<double> &q, int N , double freq);
        
        vector<pair<string, double>> findRealDistBruteForce_Continuous( vector<double> &q, int N);
        
        Curve transformer( const vector<double> &v );
        
        list<pair<string, vector<double>>> & getBegin();
        
        double getFreq(){ return frequency; }
};

extern TimeSeries *curves;

#endif

