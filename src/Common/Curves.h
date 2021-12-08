#ifndef CURVES_H
#define CURVES_H


#include <list>
#include <vector>
#include <utility>
#include <string>
#include <random>

using namespace std;

class Curves {

    private:
        double delta;
        double frequency;
        vector<pair<double,double>> tGrid; //Each available tx and ty for snapping

        list<pair<string, vector<double>>> curves;
        list < pair < pair < string, vector<double> > &, vector<pair<double, double>>>> gridCurves;
        
    public:
        Curves(double d,double freq, int L);

        void insert(string id, vector<double> & p);
        
        pair<string, vector<double>> & getCurve(unsigned int i);
        
        inline unsigned int size(){return curves.size();}
        
        // Fernei pisw  to epexergasmeno kai ena deikti apo to curves
        pair<vector<double>, pair<string, vector<double>> *>  gridCurveToVector(unsigned int i);
        
        vector<pair<double, double>> curveTogrid(vector<double> &y);
};

extern Curves *curves;

#endif /* CURVES_H */

