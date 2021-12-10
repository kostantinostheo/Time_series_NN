#ifndef HASHING_H
#define HASHING_H

#include <vector>
#include <set>
#include <utility>
#include <string>

using namespace std;


// Class that contains all the hash tables that will be used, as well as the number ('L') of hash tables and the size ('TableSize') of each one
class LSHHashTable
{
    private:
        int L;  // Number of hash tables
        unsigned int TableSize;  // Number of buckets for each hash table
        
		// These are all the hash tables that will be used
		// Every bucket of each hash table contains an ID (ID(p) = r1*h1(p)+r2*h2(p)+... mod M) for the item that will be inserted
		// Each bucket also contains a pointer to a pair (item_id, vector 'p') from the list of vectors in class 'VectorData'
        vector<vector<vector< pair<unsigned int, pair<string, vector<double>> * >>>> LSH_hashTables;
    
    public:
        LSHHashTable(int L, unsigned int TableSize);
        void LSH_insert(int l, vector<double> &p, pair<string, vector<double>> * vectorPointer);
        vector<pair<string, double>> LSH_findNN(vector<double> &q, int N);
        vector<pair<string, double>> LSH_findCurvedNN(vector<double> &q, int N, double freq);
        set<string> LSH_rangeSearch(vector<double> &q, double R);
        void printHash();
};


extern LSHHashTable *LSH_hashTables;
void init_hashing_lsh(int k, int L, int d, unsigned int TableSize, double delta);
void init_hashing_lsh(int k, int L, int d, unsigned int TableSize);
#endif

