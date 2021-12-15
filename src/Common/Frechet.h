#ifndef FRECHET_H
#define FRECHET_H

#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

double FrechetDistance(const vector<double> & P, const vector<double> & Q, double freq);

vector<pair<int, int>> OptimalTraversal(const vector<double>& P, const vector<double>& Q, double freq);

#endif /* FRECHET_H */