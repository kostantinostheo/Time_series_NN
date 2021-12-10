#include <cmath>
#include <algorithm>
#include <limits>
#include "Tools.h"
#include "Curves.h"
#include "Frechet.h"


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

pair<string, vector<double>> * Curves::insert(string id, vector<double> & v)
{
    curves.push_back(make_pair(id, v));  // Insert the 'item_id' of point 'p' and its coordinates
    
    // Get the item that was just inserted in the list
    pair< string, vector<double>> &p = curves.back();
    
    return &p;
    
//    vector<pair<double, double>> processedCurve = curveTogrid(v);
    
//    gridCurves.push_back(make_pair(p, processedCurve));  //list<pair< pair<string, vector<double>> &, vector<pair<double, double>>>> 
}
        
pair<string, vector<double>> & Curves::getCurve(unsigned int i)
{
    auto f = curves.begin();

    std::advance(f, i);
    
    return *f;
    
}

vector<double>  Curves::gridCurveToVector(vector<pair<double, double>> &f)
{
    vector<double> concatVector;
    
    for (auto xy : f) {

        concatVector.push_back(xy.first);
        concatVector.push_back(xy.second);
    }

    return concatVector;
}


vector<pair<double, double>> Curves::curveTogrid(vector<double> & y, int j)
{
    vector<pair<double, double>> proccesed; //[[1,2][3,5][4,1]]
    
    {
        double tx = tGrid[j].first;
        double ty = tGrid[j].second;

        double xi = floor(abs(0*frequency-tx)/delta + 0.5)*delta + tx;
        double yi = floor(abs(y[0]-ty)/delta +0.5)*delta + ty;

        proccesed.push_back( make_pair( xi, yi ) );
    }
    
    //TODO: Maybe this should change. 
    for (int i = 1; i < y.size(); i++) 
    {
        double tx = tGrid[j].first;
        double ty = tGrid[j].second;

        double xi = floor(abs(i*frequency-tx)/delta +0.5)*delta + tx;
        double yi = floor(abs(y[i]-ty)/delta +0.5)*delta + ty;

        if( proccesed.back().first != xi || proccesed.back().second != yi )
            proccesed.push_back( make_pair( xi, yi ) );
    }
    
    
    return proccesed;
}


void Curves::padVector(vector<double> &v)
{
    int m = (curves.begin())->second.size() * 2;
    int n = v.size();
    
    
    v.resize(m);
    
    for (int i = n; i < m; i++) {
        
        v[i] = numeric_limits<double>::max();
    }

}

vector<pair<string, double>> Curves::findRealDistBruteForce( vector<double> &q, int N , double freq)
{
    vector<pair<string, double>> b;
    
    // For every point 'p' calculate its distance from 'q'
    for(auto candidate : curves)
    {
        vector<double> &p = candidate.second;
        b.push_back( make_pair(candidate.first, FrechetDistance(p, q, freq)) );
    }
    
    // Sort the vector 'b' to find the shortest distances
    sort(b.begin(), b.end(), sortbyDist);
    
    // Only keep the N shortest distances
    b.resize(N);
    
    return b;
}