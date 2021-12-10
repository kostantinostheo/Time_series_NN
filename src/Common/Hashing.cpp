#define _CRT_SECURE_NO_WARNINGS
#include "../LSH/LshHashing.h"
#include "../HyperCube/CubeHashing.h"
#include "../Common/VectorData.h"
#include "../Common/Curves.h"
#include "Euclidean.h"
#include "Tools.h"
#include "Frechet.h"

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#define M 4294967291   // M = 2^32 - 5

int window;   // 'w' variable that is used by the h(p) hash functions
vector<int> r;
vector<vector<int>> g;
vector<double> t;
vector<vector<double>> v;

// Each h(p) function will have a map and each map will have a key value of h(p) and a mapped value of 0 or 1
vector< map<int, bool> > cubeMap;

LSHHashTable *LSH_hashTables = NULL;
CubeHashTable *C_hashTables = NULL;
VectorData *vectorData = NULL;
Curves *curves = NULL;

using namespace std;

// Function that is used to initialize all the necessary variables and data structures in order to use the hash functions and the hash tables (LSH)
void init_hashing_lsh(int k, int L, int d, unsigned int TableSize, double delta)
{
    srand(time(NULL));
    
    window = 400;
    int C = 1;
    
    LSH_hashTables = new LSHHashTable(L, TableSize);
    curves = new Curves(delta, C, L);

    // Initialize the 'r' vector that will be used by every amplified hash function 'g(p)'
    {
        default_random_engine generator(time(NULL));
        uniform_int_distribution<int> distribution(1, INT32_MAX);

        for (int i = 0; i < k; i++) {

            r.push_back(distribution(generator));
        }
    }

    // Initialize all the random 't' numbers that will be used by the hash functions 'h(p)'
    {
        default_random_engine generator(time(NULL));
        uniform_real_distribution<double> distribution(0, window);

        for (int i = 0; i < k*L; i++) {

            t.push_back(distribution(generator));
        }
    }
    
    // Initialize the all the 'v' vectors that will be used by the hash functions 'h(p)'
	// We need k*L vectors 'v', one for each hash function
    {
        default_random_engine generator(time(NULL));
        normal_distribution<double> distribution(0.0, 1.0);

        v.resize(k*L);

        for (int i = 0; i < k*L; i++) {

            for (int j = 0; j < d; j++) {
                
                v[i].push_back(distribution(generator));
            }
        }
    }

    // Initialize the 'g' vectors
	// There will be L 'g' vectors in total and each one stores the number 'i' of the hi(p) functions that will be multiplied with the 'ri' numbers
	// E.g k=4, L=5, g[0]=[0,1,3,2] then g1(p)=((r1*h1(p)+r2*h2(p)+r3*h4(p)+r4*h3(p)) mod M) mod TableSize
    g.resize(L);
    for (int i = 0; i < L; i++) {
        
        for (int j = 0; j < k; j++) {
            
            g[i].push_back(j + i*k);  // Insert that number in g[i]
        }
    }
}
void init_hashing_lsh(int k, int L, int d, unsigned int TableSize)
{
    srand(time(NULL));
    
    window = 400;
    
    LSH_hashTables = new LSHHashTable(L, TableSize);
    vectorData = new VectorData();

    // Initialize the 'r' vector that will be used by every amplified hash function 'g(p)'
    {
        default_random_engine generator(time(NULL));
        uniform_int_distribution<int> distribution(1, INT32_MAX);

        for (int i = 0; i < k; i++) {

            r.push_back(distribution(generator));
        }
    }

    // Initialize all the random 't' numbers that will be used by the hash functions 'h(p)'
    {
        default_random_engine generator(time(NULL));
        uniform_real_distribution<double> distribution(0, window);

        for (int i = 0; i < k*L; i++) {

            t.push_back(distribution(generator));
        }
    }
    
    // Initialize the all the 'v' vectors that will be used by the hash functions 'h(p)'
	// We need k*L vectors 'v', one for each hash function
    {
        default_random_engine generator(time(NULL));
        normal_distribution<double> distribution(0.0, 1.0);

        v.resize(k*L);

        for (int i = 0; i < k*L; i++) {

            for (int j = 0; j < d; j++) {
                
                v[i].push_back(distribution(generator));
            }
        }
    }

    // Initialize the 'g' vectors
	// There will be L 'g' vectors in total and each one stores the number 'i' of the hi(p) functions that will be multiplied with the 'ri' numbers
	// E.g k=4, L=5, g[0]=[0,1,3,2] then g1(p)=((r1*h1(p)+r2*h2(p)+r3*h4(p)+r4*h3(p)) mod M) mod TableSize
    g.resize(L);
    for (int i = 0; i < L; i++) {
        
        for (int j = 0; j < k; j++) {
            
            g[i].push_back(j + i*k);  // Insert that number in g[i]
        }
    }
}

// Function that is used to initialize all the necessary variables and data structures in order to use the hash functions and the hash table (Hypercube)
void init_hashing_cube(int k, int d, unsigned int TableSize)
{
    srand(time(NULL));
    
    window = 600;
    
    C_hashTables = new CubeHashTable(1, TableSize);
    vectorData = new VectorData();

    // Initialize all the random 't' numbers that will be used by the hash functions 'h(p)'
    {
        default_random_engine generator(time(NULL));
        uniform_real_distribution<double> distribution(0, window);

        for (int i = 0; i < k; i++) {

            t.push_back(distribution(generator));
        }
    }
    
    // Initialize the all the 'v' vectors that will be used by the hash functions 'h(p)'
    // We need k(=d') vectors 'v', one for each hash function
    {
        default_random_engine generator(time(NULL));
        normal_distribution<double> distribution(0.0, 1.0);

        v.resize(k);

        for (int i = 0; i < k; i++) {

            for (int j = 0; j < d; j++) {
                
                v[i].push_back(distribution(generator));
            }
        }
    }

    // k maps are needed, one for each h(p) function
    cubeMap.resize(k);
}

// This is the h(p) hash function
int h_func(const vector<double> &p, int i)
{
    double dot_product=0.0;
    
    // Calculate the dot product p*v
    for(int j=0; j<p.size(); j++)
    {
        cout << "skata" << endl;
        dot_product += p[j] * v[i][j];
    }
    
    return (int) floor((dot_product + t[i]) / window);
}

// This is the amplified hash function g(p)
unsigned int g_func(const vector<double> &p, int i)
{
    unsigned long sum=0;
    
	// Calculate the sum r1*h1(p) + r2*h2(p) +...
    for(int j=0; j<g[0].size(); j++)
    {
        sum = (sum % M) + euclidean_mod(r[j] * h_func(p, g[i][j]), M) ;
    }
    
    return euclidean_mod(sum, M);
}

// Constructor of LSHHashTable class
LSHHashTable::LSHHashTable(int L, unsigned int TableSize)
{
    this->L = L;
    this->TableSize = TableSize;

    // L hash tables are needed in total
    LSH_hashTables.resize(L);

    // Each hash table has 'TableSize' buckets
    for (int i = 0; i < L; i++) {
        LSH_hashTables[i].resize(TableSize);
    }
}

// Constructor of CubeHashTable class
CubeHashTable::CubeHashTable(int L, unsigned int TableSize)
{
    this->L = L;  // For the hypercube L will always be equal to 1 because we only need one hash table
    this->TableSize = TableSize;

    // L hash tables are needed in total
    C_hashTables.resize(L);

    // The hash table has 'TableSize' buckets
    for (int i = 0; i < L; i++) {
        C_hashTables[i].resize(TableSize);
    }
}


// Function that inserts an item in one of the hash tables
void LSHHashTable::LSH_insert(int i, vector<double> &p, pair<string, vector<double>> * vectorPointer)
{   
    // Argument 'i' needs to be smaller than 'L' because the amplified hash function gi(p), 0<= i <=L, will be called
    if(i < this->L)
    {   
        unsigned int hashValue = g_func(p, i);

        LSH_hashTables[i][hashValue % TableSize].push_back(make_pair(hashValue, vectorPointer));
    }
}

// Function that inserts an item in the hash table if the hypercube is used
void CubeHashTable::Cube_insert(vector<double> &p, pair<string, vector<double>> * vectorPointer, int k)
{
    unsigned int bucket = 0;
    
    // Get the result of every fi(hi(p)) function and store it in the map 'cubeMap'
    // Every different value of hi(p) corresponds to 0 or 1 in the map
    for (int i = 0; i < k; i++) {
        
        // Get hi(p)
        int hi = h_func(p, i);
        
        // If there is no key value in the map that is equal to hi(p) then hi(p) is inserted and is given a mapped value of 0 or 1 randomly
        if(cubeMap[i].find(hi) == cubeMap[i].end())
            cubeMap[i][hi] = rand() % 2;

        
	// Perform bitwise operations to concatenate all the bits that we will get from this loop
        bucket |= cubeMap[i][hi];
        if (i < k - 1)
            bucket <<= 1;
    }
	
	// Insert the item in the correct bucket
    C_hashTables[0][bucket].push_back(vectorPointer);
}


// Function that finds the N approximate nearest neighbors for LSH
vector<pair<string, double>> LSHHashTable::LSH_findNN(vector<double> &q, int N)
{
    map<string, double> b;
    
    // For each hash table
    for (int i = 0; i < L; i++) {
        
		// Get the bucket that query 'q' belongs in
        unsigned int hashValue = g_func(q, i);
        
        // For each item in the bucket
        for(auto candidate : LSH_hashTables[i][hashValue % TableSize])
        {
            // If ID(p) = ID(q)
            if (candidate.first == hashValue)
            {
                string id = candidate.second->first;  // Get the 'item_id' of the point
                vector<double>& p = candidate.second->second;  // Get the coordinates of the point

                // If the 'item_id' of that point is not already in the map then insert the distance
                if(b.find(id) == b.end())
                    b[id] = euclidean_distance(p, q);
            }
        }
    }
    
    // Store all the values of the map 'b' in vector 'vb'
    vector<pair<string, double>> vb;
    for(auto x : b)
    {
        vb.push_back(make_pair(x.first, x.second));
    }
    
    // Sort the vector 'vb' to find the shortest distances
    sort(vb.begin(), vb.end(), sortbyDist);
    
    // Only keep the N shortest distances
    if(vb.size() > N)
        vb.resize(N);
    
    return vb;
}

vector<pair<string, double>> LSHHashTable::LSH_findCurvedNN(vector<double> &query, int N, double freq)
{
    map<string, double> b;
   
    
    // For each hash table
    for (int i = 0; i < L; i++) {
        
        // Process query curve
        vector<pair<double, double>> processedQ = curves->curveTogrid( query, i);

        vector<double> concatVec = curves->gridCurveToVector( processedQ );

        curves->padVector( concatVec );
        
        
        // Get the bucket that query 'concatVec' belongs in
        unsigned int hashValue = g_func(concatVec, i);
        
        // For each item in the bucket
        for(auto candidate : LSH_hashTables[i][hashValue % TableSize])
        {
            // If ID(p) = ID(q)
            if (candidate.first == hashValue)
            {
                string id = candidate.second->first;  // Get the 'item_id' of the point
                vector<double>& p = candidate.second->second;  // Get the coordinates of the point

                // If the 'item_id' of that point is not already in the map then insert the distance
                if(b.find(id) == b.end())
                    b[id] = FrechetDistance (p, query, freq);
            }
        }
    }
    
    // Store all the values of the map 'b' in vector 'vb'
    vector<pair<string, double>> vb;
    for(auto x : b)
    {
        vb.push_back(make_pair(x.first, x.second));
    }
    
    // Sort the vector 'vb' to find the shortest distances
    sort(vb.begin(), vb.end(), sortbyDist);
    
    // Only keep the N shortest distances
    if(vb.size() > N)
        vb.resize(N);
    
    return vb;
}

// Function that finds the N approximate nearest neighbors for Hypercube
vector<pair<string, double>> CubeHashTable::Cube_findNN(vector<double> &q, int N, int k, int maxPoints, int probes)
{
    vector<pair<string, double>> b;
    vector<vector<int>> hamming;
    unsigned int bucket = 0;
    int points = 0;  // The number of points that will be checked. Must be <= 'maxPoints'
    int vertices = 0;  // The number of vertices that will be checked. Must be <= 'probes'

    // Get the bucket that query 'q' belongs in
    for (int i = 0; i < k; i++) {

        // Get hi(p)
        int hi = h_func(q, i);

        // If there is no key value in the map that is equal to hi(p) then hi(p) is inserted and is given a mapped value of 0 or 1 randomly
        if (cubeMap[i].find(hi) == cubeMap[i].end())
            cubeMap[i][hi] = rand() % 2;

        
        // Perform bitwise operations to concatenate all the bits that we will get from this loop
        bucket |= cubeMap[i][hi];
        if (i < k - 1)
            bucket <<= 1;
    }

    // Find the hamming distance between 'bucket' and every other bucket of the hash table
    hamming.resize(k+1); // Max hamming distance is k+1 if we count 0 as well
    for (unsigned int i = 0; i < this->TableSize; i++)
    {
        // Get the hamming distance between the two buckets
        unsigned int dist = HammingDistance(bucket, i);

        // The hamming[dist] vector contains all the buckets with hamming distance 'dist'
        hamming[dist].push_back(i);
    }
    

    int dist = 0;
    while (points < maxPoints && vertices < probes && dist <= k)
    {
        // Get the bucket that must be checked with hamming distance 'dist'
        for (auto buck : hamming[dist])
        {
            // For each item in the bucket
            for (auto candidate : C_hashTables[0][buck])
            {
                string id = candidate->first;  // Get the 'item_id' of the point
                vector<double>& p = candidate->second;  // Get the coordinates of the point

                b.push_back(make_pair(id, euclidean_distance(p, q)));  // Make the 'item_id' and the coordinates of the point a pair and insert it

                points++;
                if (points >= maxPoints)
                    break;
            }

            vertices++;
            if (vertices >= probes || points >= maxPoints)
                break;
        }
        dist++;
    }
    
    // Sort the vector 'b' to find the shortest distances
    sort(b.begin(), b.end(), sortbyDist);

    // Only keep the N shortest distances
    if (b.size() > N)
        b.resize(N);

    return b;
}

// Function that finds all the points within a certain radius 'R' of query 'q' for LSH
set<string> LSHHashTable::LSH_rangeSearch(vector<double> &q, double R)
{
    set<string> b;
    
    // For each hash table
    for (int i = 0; i < L; i++) {
        
		// Get the bucket that query 'q' belongs in
        unsigned int hashValue = g_func(q, i);
        
        // For each item in the bucket
        for(auto candidate : LSH_hashTables[i][hashValue % TableSize])
        {
            string id = candidate.second->first;  // Get the 'item_id' of the point
            vector<double> &p = candidate.second->second;  // Get the coordinates of the point
            
            // If the euclidean distance of 'p' from 'q' is smaller than radius 'R' then 'p' is within that radius
            // Then the 'item_id' of 'p' is inserted in the vector 'b'
            if(euclidean_distance(p, q) < R)
                b.insert(id);
        }
    }
    
    return b;
}

// Function that finds all the points within a certain radius 'R' of query 'q' for Hypercube
vector<string> CubeHashTable::Cube_rangeSearch(vector<double> &q, int k, double R, int maxPoints, int probes)
{
    vector<string> b;
    vector<vector<int>> hamming;
    unsigned int bucket = 0;
    int allBuckets = pow(2, k);
    int points = 0;  // The number of points that will be checked. Must be <= 'maxPoints'
    int vertices = 0;  // The number of vertices that will be checked. Must be <= 'probes'

    // Get the bucket that query 'q' belongs in
    for (int i = 0; i < k; i++) {

        // Get hi(p)
        int hi = h_func(q, i);

        // If there is no key value in the map that is equal to hi(p) then hi(p) is inserted and is given a mapped value of 0 or 1 randomly
        if (cubeMap[i].find(hi) == cubeMap[i].end())
            cubeMap[i][hi] = rand() % 2;

        // Perform bitwise operations to concatenate all the bits that we will get from this loop
        bucket |= cubeMap[i][hi];
        if (i < k - 1)
            bucket <<= 1;
    }

    // Find the hamming distance between 'bucket' and every other bucket of the hash table
    hamming.resize(k + 1); // Max hamming distance is k+1 if we count 0 as well
    for (int i = 0; i < allBuckets; i++)
    {
        // Get the hamming distance between the two buckets
        int dist = HammingDistance(bucket, i);

        // The hamming[dist] vector contains all the buckets with hamming distance 'dist'
        hamming[dist].push_back(i);
    }

    int dist = 0;
    while (points <= maxPoints && vertices <= probes && dist <= k)
    {
        // Get the bucket that must be checked with hamming distance 'dist'
        for (auto bucket : hamming[dist])
        {
            // For each item in the bucket
            for (auto candidate : C_hashTables[0][bucket])
            {
                string id = candidate->first;  // Get the 'item_id' of the point
                vector<double>& p = candidate->second;  // Get the coordinates of the point

                // If the euclidean distance of 'p' from 'q' is smaller than radius 'R' then 'p' is within that radius
                // Then the 'item_id' of 'p' is inserted in the vector 'b'
                if (euclidean_distance(p, q) < R)
                    b.push_back(id);

                points++;
                if (points > maxPoints)
                    break;
            }

            vertices++;
            if (vertices > probes || points > maxPoints)
                break;
        }
        dist++;
    }

    return b;
}

// Function that frees the memory that was allocated for the data structures
void DeallocateMemory()
{
    delete vectorData;
    delete LSH_hashTables;
    delete C_hashTables;
    delete curves;
}

//For Debug only
// void LSHHashTable::printHash( )
// {
//     ofstream out("ids.txt");
    
//     for (int i = 0; i < L; i++)
//     {
//         out << "HashTable " << i << endl;
//         for (int j = 0; j < TableSize; j++)
//         {
//             out << "Bucket " << j;
//             for (int k = 0; k < LSH_hashTables[i][j].size(); k++)
//             {
//                 out << " | ID: " << LSH_hashTables[i][j][k].first << ", Item_id: " << LSH_hashTables[i][j][k].second->first;
//             }
//             out << endl;
//         }
//         out << "\n\n";
//     }
//     out.close();
// }