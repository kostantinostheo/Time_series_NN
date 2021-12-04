#include "Euclidean.h"

#include <cmath>

using namespace std;

// Function that calculates the euclidean distance between two points
double euclidean_distance(const vector<double> &p, const vector<double> &q)
{
    double sum = 0.0;
    
    for(int i = 0; i < p.size(); i++)
    {
        sum = sum + pow(abs( p[i] - q[i]), 2.0);
    }
    
    return sqrt(sum);
}

// Function that performs the euclidean modulo operation
unsigned int euclidean_mod(long x, long y)
{
    if(y != 0)
    {
        int r = x % y;

        return r >= 0 ? r : r + abs(y);
    }
    
    return 0;
}

double euclidean_distance(double py, double qy, int i, int j, double C)
{
    return sqrt( pow(abs(py - qy), 2.0)+pow(abs( i*C - j*C), 2.0) );
}