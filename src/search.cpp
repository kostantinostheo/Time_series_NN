#include "./Common/Methods.h"
#include "./Common/Tools.h"
#include "./LSH/LshHashing.h"

#include <cstdlib>
#include <iostream>
#include <string>

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
    int M = 10;
    int probes = 2;
    double delta = 0;

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

    
	
    return 0;
}

