#define _CRT_SECURE_NO_WARNINGS
#include "Kmeans.h"
#include "../Common/VectorData.h"
#include "../Common/Euclidean.h"

#include <iostream>
#include <string.h>

using namespace std;

#define STOP 20

Clusters *clusters;

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
    srand(time(NULL));
    
    vectorData = new VectorData();
}

// Function that creates the data structure that stores all the clusters and centroids
void init_clusters(int k)
{
    clusters = new Clusters(k);
}

Clusters::Clusters(int k)
{
    this->k = k;
    cout << "Number of clusters: " << k << endl;
    clusters.resize(k);
    
    unsigned int random_point = rand() % vectorData->size();
    
    vector<double> c = vectorData->get(random_point).second;
    
    centroids.push_back(c);
}

void Clusters::KMeans()
{
    for (int i = 0; i < k-1; i++) {
        
        updateClusters();
        chooseNewCentroid();
    }
}

void Clusters::updateClusters()
{
    clusters.clear();
    clusters.resize(k);
    
    for(auto v : vectorData->getBegin())
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
        clusters[newC].push_back(&(v));
    }
}

void Clusters::chooseNewCentroid()
{
    vector<double> sums;
    vector<vector<double>> nonCentroidPoints;
    sums.push_back(0);
    int count = 0;
    
    for(auto v : vectorData->getBegin())
    {
        count++;
        double dist = 0;
        double max = 0;
        double min = INT32_MAX;
        
        for (int c = 0; c < centroids.size(); c++) {

            dist = euclidean_distance(v.second, centroids[c]);
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
        
        double di = pow(min / max, 2.0);
        sums.push_back(sums.back() + di);
        nonCentroidPoints.push_back(v.second);
    }
    
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> distribution(0, sums.back());
    double x = distribution(generator);
    
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
}

void Clusters::Lloyd()
{
    bool b;

    do {

        updateClusters();
        b = updateCentroids();
        
    } while(!b);
}

bool Clusters::updateCentroids()
{
    vector<double> newDist;

    for (int i = 0; i < centroids.size(); i++) {
        vector<double> m = mean(i);
        double dist = euclidean_distance(centroids[i], m);
        centroids[i] = m;
        newDist.push_back(dist);
    }
    
    for (auto d : newDist) {

        if(d > STOP)
            return false;
    }

    return true;
}

vector<double> Clusters::mean(int c)
{    
    vector<double> m = centroids[c];
    vector<double> temp;

    for (int i = 0; i < m.size(); i++) {
        
        m[i] = 0.0;
        
        for( auto vec : clusters[c] ) {
            temp = vec->second;
            if (temp.empty() == true)
                cout << "1" << endl;
            m[i] = m[i] + temp[i];
        }
        
        if(clusters[c].empty())
            m[i] = 0.0;
        else
            m[i] = m[i] / clusters[c].size();
    }
    return m;
}

// Function that returns the second closest centroid of a point
int Clusters::getSecondClosestCentroid(vector<double>& p)
{
    double min = INT32_MAX, dist = 0;
    int c = 0;

    for (int i = 0; i < centroids.size(); i++)
    {
        dist = euclidean_distance(p, centroids[i]);

        if (dist == 0)
            continue;

        if (dist < min)
        {
            min = dist;
            c = i;
        }
    }

    return c;
}

// Function that finds the average distance of a point to points in the same cluster or another cluster
double Clusters::avgDistanceBetweenPoints(vector<double>& p, int c)
{
    double dist = 0;
    for (int i = 0; i < clusters[c].size(); i++)
    {
        dist += euclidean_distance(p, clusters[c][i]->second);
    }

    return (dist / (clusters[c].size()-1));
}

void Clusters::Silhouette(string filename, bool complete)
{
    double ai = 0.0, bi = 0.0;
    unsigned int allPoints = 0;
    double temp = 0.0, sil = 0.0, result = 0.0;
    vector<double> si;
    
    for (int i = 0; i < this->k; i++)
    {
        sil = 0.0;
        for (int j = 0; j < clusters[i].size(); j++)
        {
            ai = avgDistanceBetweenPoints(clusters[i][j]->second, i);
            int secondClosest = getSecondClosestCentroid(clusters[i][j]->second);
            bi = avgDistanceBetweenPoints(clusters[i][j]->second, secondClosest);
            sil += (bi - ai) / max(ai, bi);
        }

        if (clusters[i].size() != 0)
        {
            sil = sil / clusters[i].size();
            si.push_back(sil);
            result += sil;
            allPoints++;
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

        for (int j = 0; j < centroids[i].size(); j++)
        {
            if (j < centroids[i].size() - 1)
                outputFile << centroids[i][j] << " ";
            else
                outputFile << centroids[i][j];
        }

        outputFile << "]}" << endl;
    }

    outputFile << "Silhouette: [";
    for (int i = 0; i < this->k; i++)
    {
        if (i < this->k - 1)
            outputFile << si[i] << ", ";
        else
            outputFile << si[i];
    }
    outputFile << "]" << endl;

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
void cluster(string output, bool complete)
{
    vector<double> si;

    // Find the first 'k' centroids with KMeans++
    clusters->KMeans();

    clusters->printClusters();

    // Using Lloyd's algorithm keep updating the centroids and assign the points to their nearest centroid
    //clusters->Lloyd();

    // Create the output file and write all the results inside
    //clusters->Silhouette(output, complete);
}

void Clusters::printClusters()
{
    ofstream out("clust.txt");
    cout << centroids.size() << endl;

    for (int i = 0; i < centroids.size(); i++)
    {
        out << "CLUSTER-" << i + 1 << " {";
        cout << clusters[i].size() << endl;

        for (int j = 0; j < clusters[i].size(); j++)
        {
            if (j < clusters[i].size() - 1)
                cout << clusters[i][j]->first << ", ";
            else
                cout << clusters[i][j]->first;
        }

        out << "}" << endl;
    }

    out.close();
}

void DeallocateMemoryClusters()
{
    delete vectorData;
    delete clusters;
}