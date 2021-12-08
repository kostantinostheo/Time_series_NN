#define _CRT_SECURE_NO_WARNINGS
#include "VectorData.h"
#include "Euclidean.h"
#include "Tools.h"

#include <string>
#include <algorithm>


// Function that is used to insert each point in the list
pair<string, vector<double>> * VectorData::insert(string id, const vector<double> &v)
{
    vectors.push_back(make_pair(id, v));  // Insert the 'item_id' of point 'p' and its coordinates
    
    // Get the item that was just inserted in the list
    pair< string, vector<double>>& p = vectors.back();
    
    // Return the item's address
    return &p;
}

// Function that returns the size of the list
unsigned int VectorData::size()
{
    return vectors.size();
}

// Function that returns a reference to a specific iten in the list
pair<string, vector<double>> & VectorData::get(unsigned int i)
{
    auto f = vectors.begin();

    std::advance(f, i);
    
    return *f;
}

// Function that returns a reference to the start of the list
list<pair<string, vector<double>>> & VectorData::getBegin()
{
    return vectors;
}

// Function that finds the real distances between query point 'q' and its N nearest neighbors using exhaustive search
vector<pair<string, double>> VectorData::findRealDistBruteForce( vector<double> &q, int N )
{
    vector<pair<string, double>> b;
    
    // For every point 'p' calculate its distance from 'q'
    for(auto candidate : vectors)
    {
        vector<double> &p = candidate.second;
        b.push_back( make_pair(candidate.first, euclidean_distance(p, q)) );
    }
    
	// Sort the vector 'b' to find the shortest distances
    sort(b.begin(), b.end(), sortbyDist);
    
	// Only keep the N shortest distances
    b.resize(N);
    
    return b;
}