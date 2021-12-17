#include "./Common/Methods.h"
#include "./Common/Tools.h"
#include "./LSH/LshHashing.h"
#include "./HyperCube/CubeHashing.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>


using namespace std;

int main(int argc, char** argv)
{
    string input_file = "";
    string query_file = "";
    string out_file = "";
    string algorithm = "";
    string metric = "";
    int k = 4;
    int L = 5;
    int M = 50;
    int probes = 20;
    double delta = 1;

    srand(time(NULL));
    
    if (argc > 15)
    {
        cout << "Error: Too many arguments" << endl;
        exit(1);
    }

    // Read the arguments
    for(int i = 0; i < argc; i++)
    {
        
        if(string(argv[i]) == "-i")
          input_file = argv[i+1];

        else if (string(argv[i]) == "-q")
            query_file = argv[i+1];

        else if (string(argv[i]) == "-k")
            k = atoi(argv[i+1]);

        else if (string(argv[i]) == "-L")
            L = atoi(argv[i+1]);

        else if (string(argv[i]) == "-M")
            M = atoi(argv[i + 1]);

        else if (string(argv[i]) == "-probes")
            probes = atoi(argv[i + 1]);

        else if(string(argv[i]) == "-o")
          out_file = argv[i+1];

        else if (string(argv[i]) == "-algorithm")
            algorithm = argv[i + 1];

        else if (string(argv[i]) == "-metric")
            metric = argv[i + 1];

        else if (string(argv[i]) == "-delta")
            delta = atoi(argv[i + 1]);
        
    }

    if (input_file == "" || query_file == "" || out_file == "")
    {
        cout << "Error: Some file names were not given" << endl;
        exit(1);
    }

    if (algorithm == "Frechet" && metric == "")
    {
        cout << "Error: Metric for Frechet was not given" << endl;
        exit(1);
    }

    cout << "inputFile: " << input_file << endl;
    cout << "outputFile: " << out_file << endl;
    cout << "queryFile: " << query_file << endl;

    if (algorithm == "LSH")
    {
        cout << "Running LSH" << endl;
        init_hashing_lsh(k, L, dimension(input_file), count_file_lines(input_file)/8);
        LSH_pre_process(input_file, L);
        lsh(query_file, out_file, 1);
        DeallocateMemory();
    }
    else if (algorithm == "Hypercube")
    {
        cout << "Running Hypercube" << endl;
        init_hashing_cube(k, dimension(input_file), pow(2,k));
        Cube_pre_process(input_file, k);
        cube(query_file, out_file, 1, k, M, probes);
        DeallocateMemory();
    }
    else if (algorithm == "Frechet")
    {
        if(metric == "discrete")
        {
            cout << "Running Frechet Discrete" << endl;
            init_hashing_lsh(k, L, dimension(input_file), count_file_lines(input_file)/8, delta);
            CurvesLSH_pre_process(input_file, L);
            lshCurvesDiscrete( query_file, out_file, 1, 1.0);
            DeallocateMemory();
        }
        else if(metric == "continuous")
        {
            cout << "Running Frechet Continuous" << endl;
            init_hashing_lsh(k, 1, dimension(input_file), count_file_lines(input_file)/8, delta);
            CurvesLSH_pre_process(input_file, 1, false);
            lshCurvesContinuous( query_file, out_file, 1);
            DeallocateMemory();
        }
       
    }
    
    
	
    return 0;
}

