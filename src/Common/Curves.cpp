#include <cmath>
#include "Curves.h"

Curves::Curves(double d, double freq, int L) {
    delta = d;
    frequency = freq;

    double tx, ty;
    
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> distribution(0, delta);
    // Initialize all the random 't' numbers that will be used by the hash functions 'h(p)'
    for (int i = 0; i < L; i++)
    {
        tx=distribution(generator);
        ty=distribution(generator);
        tGrid.push_back(make_pair(tx,ty));
    }
}

void Curves::insert(string id, vector<double> & v)
{
    curves.push_back(make_pair(id, v));  // Insert the 'item_id' of point 'p' and its coordinates
    
    // Get the item that was just inserted in the list
    pair< string, vector<double>> &p = curves.back();
    
    vector<pair<double, double>> processedCurve = curveTogrid(v);
    
    gridCurves.push_back(make_pair(p, processedCurve));  //list<pair< pair<string, vector<double>> &, vector<pair<double, double>>>> 
}
        
pair<string, vector<double>> & Curves::getCurve(unsigned int i)
{
    auto f = curves.begin();

    std::advance(f, i);
    
    return *f;
    
}

pair<vector<double>, pair<string, vector<double>> *>  Curves::gridCurveToVector(unsigned int i)
{
    // Pairnoume to epexergasmeno
    auto f = gridCurves.begin();

    std::advance(f, i);
    
    vector<pair<double,double>> &changed = f->second;
    
    // Pairnoume deikti apo to Curves
    auto g = curves.begin();

    std::advance(g, i);
    pair<string, vector<double>> *p = &(*g);
    
    
    vector<double> concatVector;
    
    for (auto xy : changed) {

        concatVector.push_back(xy.first);
        concatVector.push_back(xy.second);
    }

    return make_pair( concatVector, p );
}


vector<pair<double, double>> Curves::curveTogrid(vector<double> & y)
{
    vector<pair<double, double>> proccesed; //[[1,2][3,5][4,1]]
    
    //TODO: Maybe this should change. 
    for (int j = 0; j < tGrid.size(); j++) 
    {
        for (int i = 0; i < y.size(); i++) 
        {
            double tx = tGrid[j].first;
            double ty = tGrid[j].second;

            double xi = floor((i*frequency-tx)/delta +0.5)*delta + tx;
            double yi = floor((y[i]-ty)/delta +0.5)*delta + ty;
            
            if( proccesed.back().first != xi || proccesed.back().second != yi )
                proccesed.push_back( make_pair( xi, yi ) );
        }
    }
    return proccesed;
}