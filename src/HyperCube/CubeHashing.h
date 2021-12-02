#ifndef CUBE_HASHING_H
#define CUBE_HASHING_H

#include <vector>
#include <utility>
#include <string>
#include <fstream>

using namespace std;


// Class that contains the hash tables that will be used, as well as the number ('L') of hash tables and the size ('TableSize') of each one
class CubeHashTable
{
    private:
        int L;  // Number of hash tables. Will always be 1 when using the hypercube
        unsigned int TableSize;  // Number of buckets for each hash table
        
		// These are all the hash tables that will be used
		// Each bucket contains a pointer to a pair (item_id, vector 'p') from the list of vectors in class 'VectorData'
        vector<vector<vector< pair<string, vector<double>> * >>> C_hashTables; 

    public:
    
        CubeHashTable(int L, unsigned int TableSize);
        void Cube_insert(vector<double> &p, pair<string, vector<double>> * vectorPointer, int k);
        vector<pair<string, double>> Cube_findNN(vector<double> &q, int N, int k, int maxPoints, int probes);
        vector<string> Cube_rangeSearch(vector<double> &q, int k, double R, int maxPoints, int probes);

        
        void printHash( )
        {
            ofstream out("ids.txt");

            for (int i = 0; i < L; i++)
            {
                out << "HashTable " << i << endl;
                for (int j = 0; j < TableSize; j++)
                {
                    out << "Bucket " << j;
                    for (int k = 0; k < C_hashTables[i][j].size(); k++)
                    {
                        out << " | Item_id: " << C_hashTables[i][j][k]->first;
                    }
                    out << endl;
                }
                out << "\n\n";
            }

            out.close();
        }
};


extern CubeHashTable *C_hashTables;
void init_hashing_cube(int k, int d, unsigned int TableSize);

#endif
