#ifndef FRECHET_H
#define FRECHET_H

#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

double FrechetDistance(const vector<double> & P, const vector<double> & Q, double freq);
double FrechetDistance(const vector<pair<double, double>>& P, const vector<pair<double, double>>& Q);
vector<pair<int, int>> OptimalTraversal(const vector<pair<double, double>>& P, const vector<pair<double, double>>& Q);

#endif /* FRECHET_H */