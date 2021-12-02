#ifndef VECTOR_H
#define VECTOR_H

#include <list>
#include <vector>
#include <utility>
#include <string>

using namespace std;

// Class that is used to store all the points 'p' from the input file
class VectorData
{
    private:

    list<pair<string, vector<double>>> vectors;  // Each node of this list contains the 'item_id' of each point 'p' and its coordinates
    
    public:
        pair<string, vector<double>> * insert(string id, const vector<double> &v);
        unsigned int size();
        vector<pair<string, double>> findRealDistBruteForce(vector<double> &q, int N);
        pair<string, vector<double>> & get(unsigned int i);
        list<pair<string, vector<double>>> & getBegin();
};

extern VectorData *vectorData;

#endif