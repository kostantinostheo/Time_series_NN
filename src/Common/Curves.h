#ifndef CURVES_H
#define CURVES_H


#include <list>
#include <vector>
#include <utility>
#include <string>
#include <random>
#include <ctime>

using namespace std;

class Curves {

    private:
        double delta;
        double frequency;
        vector<pair<double,double>> tGrid; //Each available tx and ty for snapping

        list<pair<string, vector<double>>> curves;
        list < pair < const pair < string, vector<double> > &, vector<pair<double, double>>>> gridCurves;
        
    public:
        Curves(double d,double freq, int L);

        pair<string, vector<double>> * insert(string id, vector<double> & p);
        
        pair<string, vector<double>> & getCurve(unsigned int i);
        
        inline unsigned int size(){return curves.size();}
        
        // Fernei pisw  to epexergasmeno kai ena deikti apo to curves
        vector<double>  gridCurveToVector(vector<pair<double, double>> &f);
        
        vector<pair<double, double>> curveTogrid(vector<double> &y, int j);
        vector<double> filtering(vector<double> timeSerie);

        void padVector(vector<double> &v);
        
        vector<pair<string, double>> findRealDistBruteForce( vector<double> &q, int N , double freq);
};

extern Curves *curves;

#endif /* CURVES_H */

