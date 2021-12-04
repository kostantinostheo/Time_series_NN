#ifndef EUCLIDEAN_H
#define EUCLIDEAN_H

#include <vector>

using namespace std;

double euclidean_distance( const vector<double> &p, const vector<double> &q );

double euclidean_distance(double py, double qy, int i, int j, double C);

unsigned int euclidean_mod(long a, long b);

#endif

