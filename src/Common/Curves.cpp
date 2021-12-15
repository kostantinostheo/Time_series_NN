#include <cmath>
#include <algorithm>
#include <limits>
#include "Tools.h"
#include "Curves.h"
#include "Frechet.h"
#include "../fred/include/frechet.hpp"

double filterThreshold = 0.5;

// Constructor of class 'TimeSeries'
TimeSeries::TimeSeries(double d, double freq, int L) {
    delta = d;
    frequency = freq;

    double tx, ty;
    
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> distribution(0, delta);

    // Initialize all the random 't' numbers that will be used by the grids
    for (int i = 0; i < L; i++)
    {
        tx=distribution(generator);
        ty=distribution(generator);
        tGrid.push_back(make_pair(tx,ty));
    }
}

// Function that inserts a curve into a list of curves
pair<string, vector<double>> * TimeSeries::insert(string id, vector<double> & v)
{
    // Insert the 'item_id' of curve 'p' and its coordinates
    curves.push_back(make_pair(id, v));
    
    // Get the item that was just inserted in the list
    pair< string, vector<double>> &p = curves.back();
    
    return &p; 
}

// Returns a specific curve from the list of curves
pair<string, vector<double>> & TimeSeries::getCurve(unsigned int i)
{
    auto f = curves.begin();

    std::advance(f, i);
    
    return *f;
}

// Function that concatenates all the points of a grid curve and creates a vector
vector<double>  TimeSeries::vectorization(vector<pair<double, double>> &f)
{
    vector<double> concatVector;
    
    for (auto xy : f) {

        concatVector.push_back(xy.first);
        concatVector.push_back(xy.second);
    }

    return concatVector;
}

// Function that filters curves (Continuous Frechet)
vector<double> TimeSeries::filtering(vector<double> & timeSerie)
{
    vector<double> filtered;
    
    for (int i = 0; i < timeSerie.size(); i++)
    {
        if (i == timeSerie.size()-1 || i == timeSerie.size()-2)
        {
            filtered.push_back(timeSerie[i]);
        }
        else
        {
            if(abs(timeSerie[i] - timeSerie[i+1]) <= filterThreshold && abs(timeSerie[i+1] - timeSerie[i+2]) <= filterThreshold)
            {
                filtered.push_back(timeSerie[i]);
                i++;
            }
            else
                filtered.push_back(timeSerie[i]);
        }
    }

    return filtered;
}

// Function that only keeps the minima and maxima of a curve and discards all other points
vector<double> TimeSeries::minimaxima(vector<double> & timeSerie)
{
    vector<double> super_filtered;
    double min, max = 0.0;

    super_filtered.push_back(timeSerie[0]); 

    for (int i = 1; i < timeSerie.size()-1; i++)
    {
        min = timeSerie[i-1];
        max = timeSerie[i+1];
        if(timeSerie[i-1] > timeSerie[i+1])
        {
            max = timeSerie[i-1];
            min = timeSerie[i+1];
        }
        
        if(timeSerie[i] >= min && timeSerie[i] <= max)
            continue;
        else   
            super_filtered.push_back(timeSerie[i]);
    }

    super_filtered.push_back(timeSerie[timeSerie.size()-1]);

    return super_filtered;
}

// Function that maps a curve to a grid
vector<pair<double, double>> TimeSeries::snappingDiscrete(vector<double> & y, int j)
{
    vector<pair<double, double>> proccesed;
    
    // Find the coordinates of the grid curve by using a formula
    {
        double tx = tGrid[j].first;
        double ty = tGrid[j].second;

        double xi = floor((tx)/delta +0.5)*delta + tx;
        double yi = floor(abs(y[0]-ty)/delta +0.5)*delta + ty;

        proccesed.push_back(make_pair(xi, yi));
    }
    
    for (int i = 1; i < y.size(); i++) 
    {
        double tx = tGrid[j].first;
        double ty = tGrid[j].second;

        double xi = floor(abs(i*frequency-tx)/delta +0.5)*delta + tx;
        double yi = floor(abs(y[i]-ty)/delta +0.5)*delta + ty;

        if(proccesed.back().first != xi || proccesed.back().second != yi)
            proccesed.push_back(make_pair(xi, yi));
    }
    
    return proccesed;
}

// Function that maps a curve to a grid
vector<double> TimeSeries::snappingContinuous(vector<double> & y)
{
    vector<double> proccesed;
    
    // Find the coordinates of the grid curve by using a formula
    {
        double ty = tGrid[0].second;

        double yi = floor((y[0]+ty)/delta)*delta;

        proccesed.push_back(yi);
    }
    
    for (int i = 1; i < y.size(); i++) 
    {
        double ty = tGrid[0].second;

        double yi = floor((y[i]+ty)/delta)*delta;

        if(proccesed.back() != yi)
            proccesed.push_back(yi);
    }
    
    return proccesed;
}


// Function that pads a vector with a large number if it's shorter than expected
void TimeSeries::padVector(vector<double> &v, bool discrete)
{
    int m = (curves.begin())->second.size() * ( discrete ? 2 : 1); // New size of vector
    int n = v.size();  // Current size of vector
    
    v.resize(m);
    
    for (int i = n; i < m; i++) {
        
        v[i] = numeric_limits<double>::max();
    }

}

// Function that finds the real Frechet distances between curve'q' and its N nearest neighbors using exhaustive search
vector<pair<string, double>> TimeSeries::findRealDistBruteForce_Discrete(vector<double> &q, int N , double freq)
{
    vector<pair<string, double>> b;
    
    // For every curve 'p' calculate its distance from 'q'
    for(auto candidate : curves)
    {
        vector<double> &p = candidate.second;
        b.push_back(make_pair(candidate.first, FrechetDistance(p, q, freq)));
    }
    
    // Sort the vector 'b' to find the shortest distances
    sort(b.begin(), b.end(), sortbyDist);
    
    // Only keep the N shortest distances
    b.resize(N);
    
    return b;
}

// Function that finds the real Frechet distances between curve'q' and its N nearest neighbors using exhaustive search
vector<pair<string, double>> TimeSeries::findRealDistBruteForce_Continuous(vector<double> &q, int N)
{
    vector<pair<string, double>> b;
    
    // For every curve 'p' calculate its distance from 'q'
    for(auto candidate : curves)
    {
        vector<double> &p = candidate.second;
        
        Curve curve1 = transformer( p );
        Curve curve2 = transformer( q );

        Frechet::Continuous::Distance dist = Frechet::Continuous::distance( curve1, curve2 );
        
        b.push_back(make_pair(candidate.first, dist.value));
    }
    
    // Sort the vector 'b' to find the shortest distances
    sort(b.begin(), b.end(), sortbyDist);
    
    // Only keep the N shortest distances
    b.resize(N);
    
    return b;
}

Curve TimeSeries::transformer( const vector<double> &v )
{
    Curve fredCurve ( 1 );
    
    for (int i = 0; i < v.size(); i++) {
        
        Point p(1);
        p.set(0, v[i]);
        fredCurve.push_back( p );
    }

    return fredCurve;
}