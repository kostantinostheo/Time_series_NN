#define _CRT_SECURE_NO_WARNINGS
#include "Kmeans.h"
#include "../Common/VectorData.h"
#include "../Common/Euclidean.h"
#include "../Common/Curves.h"
#include "../Common/Frechet.h"

#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

#define STOP 75

Clusters *clust;

// Function that reads the configuration file
void readConfig(string filename, int& num_clusters, int& L, int& num_hash, int& M, int& cube_dim, int& probes)
{
    // Open the configuration file
    ifstream confFile(filename);

    if (confFile)
    {
        string line;

        // Read every line of the file
        while (getline(confFile, line))
        {
            char* buff = new char[line.length() + 1];
            strcpy(buff, line.c_str());

            char* type = strtok(buff, " ");
            char* val = strtok(NULL, " ");

            if (string(type) == "number_of_clusters:")
                num_clusters = atoi(val);
            else if (string(type) == "number_of_vector_hash_tables:")
                L = atoi(val);
            else if (string(type) == "number_of_vector_hash_functions:")
                num_hash = atoi(val);
            else if (string(type) == "max_number_M_hypercube:")
                M = atoi(val);
            else if (string(type) == "number_of_hypercube_dimensions:")
                cube_dim = atoi(val);
            else if (string(type) == "number_of_probes:")
                probes = atoi(val);

            delete[] buff;
        }

        confFile.close();
    }
}

// Function that initializes creates the data structure that stores all the points
void init_vectorData()
{
    curves = nullptr;
    vectorData = new VectorData();
}

// Function that creates the data structure that stores all the clusters and centroids
void init_clusters(int k, bool frechetOption)
{
    clust = new Clusters(k, frechetOption);
}

// Constructor of class 'Clusters'
Clusters::Clusters(int k, bool frechetOption)
{
    this->k = k;
    cout << "Number of clusters: " << k << endl;
    clusters.resize(k);
    
    if( !frechetOption ) {
        unsigned int random_point = rand() % vectorData->size();

        // Get a random point from the list of points
        vector<double> c = vectorData->get(random_point).second;
        
        // This random point will be the first centroid
        centroids.push_back(c);
    }
    else {
        
        unsigned int random_point = rand() % curves->size();

        // Get a random point from the list of points
        vector<double> c = curves->getCurve(random_point).second;
        
        // This random point will be the first centroid
        vector<pair<double, double>> e = curves->convertToYX(c);

        centroids.push_back(c);
        centroidsXY.push_back(e);
    }
    
}

// Function that performs KMeans++ initialization
void Clusters::KMeans(bool frechetOption)
{
    // Find the initial centroids
    if(!frechetOption){
    
        for (int i = 0; i < k-1; i++) {

            updateClusters();
            chooseNewCentroid();
        }
    }
    else {
        for (int i = 0; i < k-1; i++) {

            updateClustersFrechet(true);
            chooseNewCentroid(true);
        }
    }
}

// Function that finds the closest centroid for every point in the dataset
// and assigns the points to the right cluster
void Clusters::updateClusters()
{
    clusters.clear();
    clusters.resize(k);
    int i = 0;
    
    // For every point find the closest centroid
    for(auto &v : vectorData->getBegin())
    {
        int newC = -1;
        double min = INT32_MAX;
        
        for (int c = 0; c < centroids.size() ; c++) {

            double dist = euclidean_distance(v.second, centroids[c]);
            
            if(dist < min) {
                min = dist;
                newC = c;
            }
        }
        
        // Assign the point to this cluster
        clusters[newC].push_back(&(v));
    }
}

// Function that finds the closest centroid for every curve in the dataset
// and assigns the curves to the right cluster
void Clusters::updateClustersFrechet(bool isKmeans)
{
    clusters.clear();
    clusters.resize(k);
    
    double freq = curves->getFreq();
    
    if(isKmeans) {
        
        // For every curve find the closest centroid
        for(auto &v : curves->getBegin())
        {
            int newC = -1;
            double min = INT32_MAX;

            for (int c = 0; c < centroids.size() ; c++) {

                double dist = FrechetDistance( v.second, centroids[c], freq );

                if(dist < min) {
                    min = dist;
                    newC = c;
                }
            }

            // Assign the curve to this cluster
            clusters[newC].push_back(&(v));
        }
    }
    else {
        
        // For every curve find the closest centroid
        for(auto &v : curves->getBegin())
        {
            int newC = -1;
            double min = INT32_MAX;


            for (int c = 0; c < centroidsXY.size() ; c++) {

                double dist = FrechetDistance( curves->convertToYX(v.second), centroidsXY[c] );

                if(dist < min) {
                    min = dist;
                    newC = c;
                }
            }

            // Assign the curve to this cluster
            clusters[newC].push_back(&(v));
        }
    }
}

// Function that finds a new centroid during KMeans++
void Clusters::chooseNewCentroid(bool frechetOption)
{
    vector<double> sums;
    vector<vector<double>> nonCentroidPoints;
    sums.push_back(0);  // Partial sums P(r)
    int count = 0;
    double freq;
    
    list<pair<string, vector<double>>> * collection;
    
    if(!frechetOption)
        collection = &(vectorData->getBegin());
    else{
        collection = &(curves->getBegin());
        freq = curves->getFreq();
    }
    
    // For every non-centroid point
    for(auto v : *collection)
    {
        count++;
        double dist = 0;
        double max = 0;
        double min = INT32_MAX;
        
        // Find the minimum and maximum distance to some centroids
        for (int c = 0; c < centroids.size(); c++) {

            if(!frechetOption)
                dist = euclidean_distance(v.second, centroids[c]);
            else
                dist = FrechetDistance(v.second, centroids[c], freq);
            
            if(dist == 0.0)
                break;
                    
            if(dist < min)
                min = dist;
            
            if(dist > max)
                max = dist;
        }

        if (dist == 0.0)
            continue;

        if(min == INT32_MAX)
            continue;
        
        // Get the normalized D(i)^2
        double di = pow(min / max, 2.0);

        // Add D(i) and the previous partial sum and store it
        sums.push_back(sums.back() + di);

        nonCentroidPoints.push_back(v.second);
    }
    
    // Find a uniformly distributed float x in [0, P(n-t)]
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> distribution(0, sums.back());
    double x = distribution(generator);
    
    // Find the right index 'r' that satisfies the conditions below
    unsigned int r = 0;
    while(r < sums.size()){
        
        if(sums[r] < x && x <= sums[r+1]){
            r++;
            break;
        }
        
        r++;
    }

    //vector<double> c = vectorData->get(r).second;
    vector<double> c = nonCentroidPoints[r-1];
    centroids.push_back(c);
    
    if(frechetOption){
        
        int i=0;
        freq = curves->getFreq();
        vector<pair<double, double>> e;
        for (auto y : c) {
            e.push_back( make_pair( i*freq, y ) );
        }
        centroidsXY.push_back(e);
    }


}

// Function that performs Lloyd's assignment
void Clusters::Lloyd(bool frechetOption)
{
    bool b;

    if( !frechetOption ){
        do {

            updateClusters();
            b = updateCentroids();

        } while(!b);
    }
    else {
        do {

            updateClustersFrechet();
            b = updateCentroids(true);
            cout << "updated center" << endl;

        } while(!b);
    }
}

// Function that finds better centroids by calculating a mean vector per cluster
bool Clusters::updateCentroids(bool frechetOption)
{
    vector<double> newDist;

    if(!frechetOption ){
        // For every cluster find the mean vector and make that the new centroid
        for (int i = 0; i < centroids.size(); i++) {
            vector<double> m = mean(i);
            double dist = euclidean_distance(centroids[i], m);
            centroids[i] = m;
            newDist.push_back(dist);
        }
    }
    else{
        
        // For every cluster find the mean vector and make that the new centroid
        for (int i = 0; i < centroidsXY.size(); i++) {
            vector<pair<double, double>> m = meanCurve(i);
            double dist = FrechetDistance(centroidsXY[i], m );
            centroidsXY[i] = m;
            newDist.push_back(dist);
        }
    }
    
    // If every old centroid has a distance less than 'STOP' from the new centroids then Lloyd's assignment ends
    for (auto d : newDist) {

        if(d > STOP)
            return false;
    }

    return true;
}

// Function that finds the mean vector of a specific cluster
vector<double> Clusters::mean(int c)
{    
    vector<double> m = centroids[c];
    vector<double> temp;

    for (int i = 0; i < m.size(); i++) {
        
        m[i] = 0.0;
        
        for( auto vec : clusters[c] ) {
            temp = vec->second;
            m[i] = m[i] + temp[i];
        }
        
        if(clusters[c].empty())
            m[i] = 0.0;
        else
            m[i] = m[i] / clusters[c].size();
        
    }
    return m;
}

vector<pair<double, double>> Clusters::meanCurve(int c)
{    
    if(clusters[c].size() == 0){
        
        return centroidsXY[c];
    }
    
    double freq = curves->getFreq();

    vector<pair<string, vector<double>> *> & cus = clusters[c];
    vector<vector<pair<double, double>> > temp;
    for (auto elem : cus) {

        int i=0;
        vector<pair<double, double>> x;
        for( auto inner : elem->second )
            x.push_back( make_pair( (i++)*freq, inner ) );
        
        temp.push_back(x);
    }

    shuffle( temp.begin(), temp.end(), std::default_random_engine(time(NULL)) );
    
    int numCurves = temp.size();
    int height = ceil( log2(numCurves) );
    unsigned int dummiesCount = pow( 2, height ) - numCurves;
    

      
    for(int h=0; h<height; h++){
    
        vector<vector<pair<double,double> >> temp2;
        
        while( temp.size() != 0 )
        {
            if( temp.size() >= 2 )
            {
                vector<pair<double,double>> p = temp.front();        temp.erase( temp.begin() );
                vector<pair<double,double>> q = temp.front();        temp.erase( temp.begin() );

                vector<pair<int, int>> indexes = OptimalTraversal( p, q );

                vector<pair<double,double>> x;

                for (int i = 0; i < indexes.size(); i++) {

                    x.push_back( make_pair ( (p[indexes[i].first].first+q[indexes[i].second].first)/2.0, (p[indexes[i].first].second+q[indexes[i].second].second)/2.0)  );
                }

                // eisagwgi ston panw epipedo
                temp2.push_back(x);
            }
            else {

                vector<pair<double,double>> p = temp.front();        temp.erase( temp.begin() );
                dummiesCount--;
                
                // eisagwgi ston panw epipedo
                temp2.push_back(p);
            }
        }
        
        dummiesCount /= 2;
        
        temp = temp2;
    }
    
    return temp[0];
}

// Function that returns the second closest centroid of a point
int Clusters::getSecondClosestCentroid(vector<double>& p, int c, bool frechetOption)
{
    double min = INT32_MAX, dist = 0;
    int secondClosest = 0;

    if(!frechetOption){
        for (int i = 0; i < centroids.size(); i++)
        {
            // If this is the closest centroid then skip it
            // We need the second closest centroid
            if (i == c)
                continue;

            dist = euclidean_distance(p, centroids[i]);

            if (dist == 0)
                continue;

            if (dist < min)
            {
                min = dist;
                secondClosest = i;
            }
        }
    }
    else {
        double freq = curves->getFreq();
        for (int i = 0; i < centroids.size(); i++)
        {
            // If this is the closest centroid then skip it
            // We need the second closest centroid
            if (i == c)
                continue;

            dist = FrechetDistance(p, centroids[i], freq);

            if (dist == 0)
                continue;

            if (dist < min)
            {
                min = dist;
                secondClosest = i;
            }
        }
    }

    return secondClosest;
}

// Function that finds the average distance of a point to points in the same cluster or another cluster
double Clusters::avgDistanceBetweenPoints(vector<double>& p, int c, bool frechetOption)
{
    double dist = 0;
    
    if( !frechetOption ){
    
        for (int i = 0; i < clusters[c].size(); i++)
        {
            dist += euclidean_distance(p, clusters[c][i]->second);
        }
    }
    else {
        
        double freq = curves->getFreq();
        
        for (int i = 0; i < clusters[c].size(); i++)
        {
            dist += FrechetDistance(p, clusters[c][i]->second, freq);
        }
    }

    if(clusters[c].size() != 0)
        return (dist / clusters[c].size());
    else
        return 0.0;
}

// Function that writes all the results in a file
void Clusters::Silhouette(string filename, bool complete, bool silhouette, bool frechetOption)
{
    double ai = 0.0, bi = 0.0;
    unsigned int allPoints = 0;
    double sil = 0.0, result = 0.0;
    vector<double> si;
    
    for (int i = 0; i < this->k; i++)
    {
        sil = 0.0;
        for (int j = 0; j < clusters[i].size(); j++)
        {
            ai = avgDistanceBetweenPoints(clusters[i][j]->second, i, frechetOption);
            int secondClosest = getSecondClosestCentroid(clusters[i][j]->second, i, frechetOption);
            bi = avgDistanceBetweenPoints(clusters[i][j]->second, secondClosest, frechetOption);
            sil += (bi - ai) / max(ai, bi);
        }

        if (clusters[i].size() != 0)
        {
            sil = sil / clusters[i].size();
            si.push_back(sil);
            result += sil;
            allPoints++;
        }
        else {
            si.push_back(-2.0);
        }
    }

    result = result / allPoints;
    si.push_back(result);

    // Open the output file
    ofstream outputFile(filename);
    
    outputFile << "Algorithm: Lloyds" << endl;

    for (int i = 0; i < this->k; i++)
    {
        outputFile << "CLUSTER-" << i + 1 << " {size: " << clusters[i].size() << ", centroid: [";

        if(!frechetOption){
        
            for (int j = 0; j < centroids[i].size(); j++)
            {
                if (j < centroids[i].size() - 1)
                    outputFile << centroids[i][j] << " ";
                else
                    outputFile << centroids[i][j];
            }
        }
        else {
            for (int j = 0; j < centroidsXY[i].size(); j++)
            {
                if (j < centroidsXY[i].size() - 1)
                    outputFile << "(" << centroidsXY[i][j].first << ", "<< centroidsXY[i][j].second << ") ";
                else
                    outputFile << "(" << centroidsXY[i][j].first << ", "<< centroidsXY[i][j].second << ")";
            }
        }

        outputFile << "]}" << endl;
    }
    outputFile << endl;

    if (silhouette == true)
    {
        outputFile << "Silhouette: [";
        for (int i = 0; i < allPoints+1; i++)
        {
            if (i < allPoints)
                outputFile << si[i] << ", ";
            else
                outputFile << si[i];
        }
        outputFile << "]" << endl;
        outputFile << endl;
    }

    if (complete == true)
    {
        for (int i = 0; i < centroids.size(); i++)
        {
            outputFile << "CLUSTER-" << i + 1 << " {centroid, ";
            for (int j = 0; j < clusters[i].size(); j++)
            {
                if (j < clusters[i].size() - 1)
                    outputFile << clusters[i][j]->first << ", ";
                else
                    outputFile << clusters[i][j]->first;
            }

            outputFile << "}" << endl;
        }
    }

    outputFile.close();
}

// Function that creates 'k' clusters and assigns each point to a cluster. Lloyd's algorithm has been implemented
// It also generates the output file with the results
void cluster(string output, bool complete, bool silhouette, bool frechetOption)
{
    // Find the first 'k' centroids with KMeans++
    clust->KMeans(frechetOption);

    // Using Lloyd's algorithm keep updating the centroids and assign the points to their nearest centroid
    clust->Lloyd(frechetOption);

    // Create the output file and write all the results inside
    clust->Silhouette(output, complete, silhouette, frechetOption);
}

void Clusters::printClusters()
{
    ofstream out("clust.txt");

    for (int i = 0; i < centroids.size(); i++)
    {
        out << "CLUSTER-" << i + 1 << " {";

        for (int j = 0; j < clusters[i].size(); j++)
        {
            if (j < clusters[i].size() - 1)
                out << clusters[i][j]->first << ", ";
            else
                out << clusters[i][j]->first;
        }

        out << "}" << endl;
    }
    out << endl;

    out.close();
}

void DeallocateMemoryClusters()
{
    delete vectorData;
    delete clust;
    delete curves;
}