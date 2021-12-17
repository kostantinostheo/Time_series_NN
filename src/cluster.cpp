#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

#include "./Common/Methods.h"
#include "./Cluster/Kmeans.h"
#include "./Common/Curves.h"

using namespace std;


int main(int argc, char** argv)
{
    string input_file = "";
    string conf_file = "";
    string out_file = "";
    string update = "";
    string assignment = "";
    bool complete = false;
    bool silhouette = false;
    int num_clusters = 2;
    int L = 3;
    int num_hash = 4;
    int M = 10;
    int cube_dim = 3;
    int probes = 2;

    srand(time(NULL));

    if (argc > 15)
    {
        cout << "Error: Too many arguments" << endl;
        exit(1);
    }

    // Read the arguments
    for (int i = 0; i < argc; i++)
    {

        if (string(argv[i]) == "-i")
            input_file = argv[i + 1];

        else if (string(argv[i]) == "-c")
            conf_file = argv[i + 1];

        else if (string(argv[i]) == "-o")
            out_file = argv[i + 1];
		
		else if (string(argv[i]) == "-update")
            update = argv[i + 1];
		
		else if (string(argv[i]) == "-assignment")
            assignment = argv[i + 1];

        else if (string(argv[i]) == "-complete")
            complete = true;

        else if (string(argv[i]) == "-silhouette")
            silhouette = true;
    }

    if (input_file == "" || conf_file == "" || out_file == "")
    {
        cout << "Error: Some file names were not given" << endl;
        exit(1);
    }

    readConfig(conf_file, num_clusters, L, num_hash, M, cube_dim, probes);

    cout << "inputFile: " << input_file << endl;
    cout << "outputFile: " << out_file << endl;
    cout << "confFile: " << conf_file << endl;

    if (update == "Mean_Vector")
    {
        if (assignment == "Classic")
        {
            init_vectorData();

            Cluster_pre_process(input_file);

            init_clusters(num_clusters);
            
            cluster(out_file, complete, silhouette);

            DeallocateMemoryClusters();
        }
        else if (assignment == "LSH")
        {

        }
        else if (assignment == "Hypercube")
        {

        }
    }
    else if (update == "Mean_Frechet")
    {
        if (assignment == "Classic")
        {
            //init_vectorData();
            curves = new TimeSeries(1.0, 1.0, 0.0);

            Cluster_pre_process(input_file, true);

            init_clusters(num_clusters, true);

            cluster(out_file, complete, silhouette, true);

            DeallocateMemoryClusters();
        }
        else if (assignment == "LSH_Frechet")
        {

        }
    }

    return 0;
}


