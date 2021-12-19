#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <utility>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;

class Clusters
{
    private:
        int k;  // Number of clusters
        vector<vector<double>> centroids;    // Coordinates of all centroids (1 dimension for each coordinate)
        vector<vector<pair<double, double> >> centroidsXY;    // Coordiantes of all centroids (2 dimensions for each coordinate)
        vector<vector<pair<string, vector<double>> *>> clusters;  // k vectors that contain the points that belong in each cluster
    
    public:
        
        Clusters(int k, bool frechetOption=false);
        void KMeans(bool frechetOption=false);
        void updateClusters();
        void updateClustersFrechet(bool frechetOption=false);
        void chooseNewCentroid(bool frechetOption=false);
        void Lloyd(bool frechetOption=false);
        vector<double> mean(int c);
        vector<pair<double, double>> meanCurve(int c);
        bool updateCentroids(bool frechetOption=false);
        int getSecondClosestCentroid(vector<double>& p, int c, bool frechetOption=false);
        double avgDistanceBetweenPoints(vector<double>& p, int c, bool frechetOption=false);
        void Silhouette(string filename, bool complete, bool silhouette, int clusterTime, bool frechetOption=false );
};

extern Clusters *clust;

void readConfig(string filename, int& num_clusters, int& L, int& num_hash, int& M, int& cube_dim, int& probes);
void init_vectorData();
void init_clusters(int k, bool frechetOption=false);
void Cluster_pre_process(string filename, bool frechetOption=false);
void DeallocateMemoryClusters();
void cluster(string output, bool complete, bool silhouette,bool frechetOption=false);

#endif

