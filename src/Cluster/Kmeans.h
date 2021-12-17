#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <utility>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

class Clusters
{
    private:
        int k;  // Number of clusters
        vector<vector<double>> centroids;    // Coordiantes of all centroids
        vector<vector<pair<string, vector<double>> *>> clusters;  // k vectors that contain the points that belong in each cluster
    
    public:
        
        Clusters(int k);
        void KMeans(bool frechetOption=false);
        void updateClusters();
        void updateClustersFrechet();
        void chooseNewCentroid(bool frechetOption=false);
        void Lloyd(bool frechetOption=false);
        vector<double> mean(int c);
        vector<double> meanCurve(int c);
        bool updateCentroids();
        int getSecondClosestCentroid(vector<double>& p, int c);
        double avgDistanceBetweenPoints(vector<double>& p, int c);
        void Silhouette(string filename, bool complete, bool silhouette);
        void printClusters();
};

extern Clusters *clust;

void readConfig(string filename, int& num_clusters, int& L, int& num_hash, int& M, int& cube_dim, int& probes);
void init_vectorData();
void init_clusters(int k);
void Cluster_pre_process(string filename, bool frechetOption=false);
void DeallocateMemoryClusters();
void cluster(string output, bool complete, bool silhouette);

#endif

