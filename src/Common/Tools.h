#ifndef TOOLS_H
#define TOOLS_H

#include <utility>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

bool sortbyDist(const pair<string, double> &a, const pair<string, double> &b);

unsigned int HammingDistance(unsigned int a, unsigned int b);

void DeallocateMemory();

int dimension(string filename);

int count_file_lines(string filename);

/* h and g functions for LSH and hypercube calculations */
int h_func(const vector<double> &p, int i);
unsigned int g_func(const vector<double> &p, int i);

#endif