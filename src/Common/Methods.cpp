#include "Methods.h"
#include "../LSH/LshHashing.h"
#include "../HyperCube/CubeHashing.h"
#include "VectorData.h"
#include "Curves.h"

// Function that reads all the points from the input file and saves them in the appropriate data structures (LSH)
void LSH_pre_process(string filename, int L)
{
    vector<double> p;
    
    // Open the input file
    ifstream inputFile(filename);
    if (inputFile)
    {
        string line;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;  // This is the 'item_id' of the point

            token = strtok(NULL," \t");

            // Read all the coordinates of the point and store them in vector 'p'
            while (token != NULL)
            {
                p.push_back(atof(token));
                token = strtok (NULL, " \t");
            }

            // Insert the 'item_id' of the point and its coordinates in the 'vectorData' list
            // The 'VectorData::insert' function returns the address of the pair (id, p) that was just inserted in the list
            pair<string, vector<double>> * vectorDataPointer =  vectorData->insert(id, p);
            
                        
            // Insert the point in every hash table
            for (int i = 0; i < L; i++) {

                LSH_hashTables->LSH_insert(i, p, vectorDataPointer);
            }
            
            delete[] buff;
            p.clear();
        }

        inputFile.close();
    }
}


// Function that reads all the points from the input file and saves them in the appropriate data structures (Hypercube)
void Cube_pre_process(string filename, int k)
{    
    vector<double> p;
    
    // Open the input file
    ifstream inputFile(filename);

    if (inputFile)
    {
        string line;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;  // This is the 'item_id' of the point

            token = strtok(NULL," \t");

            // Read all the coordinates of the point and store them in vector 'p'
            while (token != NULL)
            {
                p.push_back(atof(token));
                token = strtok (NULL, " \t");
            }

            // Insert the 'item_id' of the point and its coordinates in the 'vectorData' list
            // The 'VectorData::insert' function returns the address of the pair (id, p) that was just inserted in the list
            pair<string, vector<double>> * vectorDataPointer =  vectorData->insert(id, p);
            
            // Insert the point in the hash table
            C_hashTables->Cube_insert(p, vectorDataPointer, k);

            delete[] buff;
            p.clear();
        }

        inputFile.close();
    }
}


// Function that reads all the query points from the query file and executes the ANN and Range Search algorithms (LSH)
// It also generates the output file with the results
void lsh(string input, string output, int N)
{
    vector<double> q;
    
    // Open the input and output files
    ifstream inputFile(input);
    
    ofstream outputFile(output);

    if (inputFile)
    {
        string line;
        auto tApproximateAverage = 0;
        auto tTrueAverage = 0;
        int counter = 0;
        double statistics = 0;
        double maf=0.0;
        double maxmaf=-1.0;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;

            token = strtok(NULL," \t");

            // Read all the coordinates of the query and store them in vector 'q'
            while (token != NULL)
            {
                q.push_back(atof(token));
                token = strtok (NULL, " \t");
            }

            // Find the N nearest neighbors using the ANN algorithm, their distance from 'q' and the time it takes to find them
            auto startLSH = chrono::steady_clock::now();
            vector<pair<string, double>> nn = LSH_hashTables->LSH_findNN(q, N);
            auto endLSH = chrono::steady_clock::now();

            // Find the real distances of the N nearest neighbors from 'q' and the time it takes to find them
            auto startRealDist = chrono::steady_clock::now();
            vector<pair<string, double>> bf =  vectorData->findRealDistBruteForce(q, N);
            auto endRealDist = chrono::steady_clock::now();

            tApproximateAverage += chrono::duration_cast<chrono::microseconds>(endLSH - startLSH).count();
            tTrueAverage += chrono::duration_cast<chrono::microseconds>(endRealDist - startRealDist).count();

            counter++;

            outputFile << "Query: " << id << endl;
            outputFile << "Algorithm: LSH_Vector" << endl;
            
            // Write all the results in the output file
            if(!nn.empty())
                outputFile << "Approximate Nearest neighbor: " << nn[0].first << endl;
            else
                outputFile << "Approximate Nearest neighbor: null" << endl;
            
            outputFile << "True Nearest neighbor: "<< bf[0].first << endl;
            
            if(!nn.empty()){
                outputFile << "distanceApproximate: "<< nn[0].second << endl;
                maf = nn[0].second / bf[0].second;
                if( maf > maxmaf ) {
                    
                    maxmaf = maf;
                }
            }
            else{
                outputFile << "distanceApproximate: null" << endl;
            }

            outputFile << "distanceTrue: "<< bf[0].second << endl;
            outputFile << endl;
            
            if(!nn.empty())
                statistics += nn[0].second - bf[0].second;

            delete[] buff;
            q.clear();
        }
        outputFile << "tApproximateAverage: " << tApproximateAverage / counter << " microseconds" << endl;
        outputFile << "tTrueAverage: " <<  tTrueAverage / counter << " microseconds" << endl;
        outputFile << "MAF: " << maxmaf << endl;

        cout << "Average distance variation: " << statistics/counter << endl;

            
        inputFile.close();
        outputFile.close();
    }
}


// Function that reads all the query points from the query file and executes the ANN and Range Search algorithms (Hypercube)
// It also generates the output file with the results
void cube(string input, string output, int N, int k, int maxPoints, int probes)
{
    vector<double> q;

    // Open the input and output files
    ifstream inputFile(input);

    ofstream outputFile(output);

    if (inputFile)
    {
        string line;
        auto tApproximateAverage = 0;
        auto tTrueAverage = 0;
        int counter = 0;
        double statistics = 0;
        double maf=0.0;
        double maxmaf=-1.0;

        // Read every line of the file
        while (getline(inputFile, line))
        {
   
            char* buff = new char[line.length() + 1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff, " \t");

            char* id = token;

            token = strtok(NULL, " \t");

            // Read all the coordinates of the query and store them in vector 'q'
            while (token != NULL)
            {
                q.push_back(atof(token));
                token = strtok(NULL, " \t");
            }

            // Find the N nearest neighbors using the ANN algorithm, their distance from 'q' and the time it takes to find them
            auto startCube = chrono::steady_clock::now();
            vector<pair<string, double>> nn = C_hashTables->Cube_findNN(q, N, k, maxPoints, probes);
            auto endCube = chrono::steady_clock::now();

            // Find the real distances of the N nearest neighbors from 'q' and the time it takes to find them
            auto startRealDist = chrono::steady_clock::now();
            vector<pair<string, double>> bf = vectorData->findRealDistBruteForce(q, N);
            auto endRealDist = chrono::steady_clock::now();


            tApproximateAverage += chrono::duration_cast<chrono::microseconds>(endCube - startCube).count();
            tTrueAverage += chrono::duration_cast<chrono::microseconds>(endRealDist - startRealDist).count();

            counter++;

            outputFile << "Query: " << id << endl;
            outputFile << "Algorithm: Hypercube" << endl;
            
            // Write all the results in the output file
            if(!nn.empty())
                outputFile << "Approximate Nearest neighbor: " << nn[0].first << endl;
            else
                outputFile << "Approximate Nearest neighbor: null" << endl;
            
            outputFile << "True Nearest neighbor: "<< bf[0].first << endl;
            
            if(!nn.empty()){
                outputFile << "distanceApproximate: "<< nn[0].second << endl;
                maf = nn[0].second / bf[0].second;

                if(maf > maxmaf)
                    maxmaf = maf;
            }
            else{
                outputFile << "distanceApproximate: null" << endl;
            }

            outputFile << "distanceTrue: "<< bf[0].second << endl;
            outputFile << endl;
            
            if(!nn.empty())
                statistics += nn[0].second - bf[0].second;

            delete[] buff;
            q.clear();
        }
        
        outputFile << "tApproximateAverage: " << tApproximateAverage / counter << " microseconds" << endl;
        outputFile << "tTrueAverage: " <<  tTrueAverage / counter << " microseconds" << endl;
        outputFile << "MAF: " <<  maxmaf << endl;

        cout << "Average distance variation: " << statistics/counter << endl;

        inputFile.close();
        outputFile.close();
    }
}


// Function that reads all the points from the input file and saves them in the appropriate data structures (Cluster)
void Cluster_pre_process(string filename, bool frechetOption)
{    
    vector<double> p;
    
    // Open the input file
    ifstream inputFile(filename);

    if (inputFile)
    {
        string line;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;  // This is the 'item_id' of the point

            token = strtok(NULL," \t");

            // Read all the coordinates of the point and store them in vector 'p'
            while (token != NULL)
            {
                p.push_back(atof(token));
                token = strtok (NULL, " \t");
            }
			
            p.pop_back();

            if(!frechetOption){
                // Insert the 'item_id' of the point and its coordinates in the 'vectorData' list
                // The 'VectorData::insert' function returns the address of the pair (id, p) that was just inserted in the list
                pair<string, vector<double>> * vectorDataPointer =  vectorData->insert(id, p);
            }
            else {
                // Insert the 'item_id' of the curve and its coordinates in the 'curves' list
                pair<string, vector<double>> * vectorDataPointer = curves->insert(id, p);
            }
            
            delete[] buff;
            p.clear();
        }

        inputFile.close();
    }
}

// Function that reads all the points from the input file and saves them in the appropriate data structures (LSH_Frechet)
void CurvesLSH_pre_process(string filename, int L, bool discrete)
{
    vector<double> p;
    
    // Open the input file
    ifstream inputFile(filename);

    if (inputFile)
    {
        string line;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;  // This is the 'item_id' of the point

            token = strtok(NULL," \t");

            // Read all the coordinates of the curve and store them in vector 'p'
            while (token != NULL)
            {
                p.push_back(atof(token));
                token = strtok (NULL, " \t");
            }

            // Insert the 'item_id' of the curve and its coordinates in the 'curves' object
            pair<string, vector<double>> * curvePtr = curves->insert(id, p);
            
            if(discrete){
            
                for (int i = 0; i < L; i++) 
                {
                    // Map the curve to a grid and get the new grid curve
                    vector<pair<double, double>> processed = curves->snappingDiscrete(curvePtr->second, i);

                    // Concatenate the coordinates of the grid curve and get the vector
                    vector<double> concatVec = curves->vectorization(processed);

                    // Pad the vector
                    curves->padVector(concatVec);

                    // Insert the vector in one of the LSH hashtables
                    LSH_hashTables->LSH_insert(i, concatVec, curvePtr);
                }
            }
            else {
                
                // Filter the curve
                vector<double> filtered = curves->filtering(p);
                
                // Map the curve to a grid and get the new grid curve
                vector<double> snapped = curves->snappingContinuous( filtered );

                // Keep only the minima and maxima of the curve
                vector<double> minimaxed = curves->minimaxima( snapped );
                
                // Pad the vector
                curves->padVector( minimaxed, false );

                // Insert the vector in one of the LSH hashtables
                LSH_hashTables->LSH_insert(0, minimaxed, curvePtr);
            }
            
            delete[] buff;
            p.clear();
        }

        inputFile.close();
    }
}

// Function that reads all the query points from the query file and executes the ANN algorithm (LSH_Frechet Discrete)
// It also generates the output file with the results
void lshCurvesDiscrete(string input, string output, int N, double freq)
{
    vector<double> q;
    
    // Open the input and output files
    ifstream inputFile(input);
    
    ofstream outputFile(output);

    if (inputFile)
    {
        string line;
        auto tApproximateAverage = 0;
        auto tTrueAverage = 0;
        int counter = 0;
        double statistics = 0;
        double maf=0.0;
        double maxmaf=-1.0;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;

            token = strtok(NULL," \t");

            // Read all the coordinates of the query and store them in vector 'q'
            while (token != NULL)
            {
                q.push_back(atof(token));
                token = strtok (NULL, " \t");
            }

            // Find the N nearest neighbors using the ANN algorithm, their Frechet distance from 'q' and the time it takes to find them
            auto startLSH = chrono::steady_clock::now();
            vector<pair<string, double>> nn = LSH_hashTables->LSH_findCurvesNN_Discrete (q, N, freq);
            auto endLSH = chrono::steady_clock::now();

            // Find the real Frechet distances of the N nearest neighbors from 'q' and the time it takes to find them
            auto startRealDist = chrono::steady_clock::now();
            vector<pair<string, double>> bf =  curves->findRealDistBruteForce_Discrete(q, N, freq);
            auto endRealDist = chrono::steady_clock::now();

            tApproximateAverage += chrono::duration_cast<chrono::microseconds>(endLSH - startLSH).count();
            tTrueAverage += chrono::duration_cast<chrono::microseconds>(endRealDist - startRealDist).count();

            counter++;

            outputFile << "Query: " << id << endl;
            outputFile << "Algorithm: LSH_Frechet_Discrete" << endl;
            
            // Write all the results in the output file
            if(!nn.empty())
                outputFile << "Approximate Nearest neighbor: " << nn[0].first << endl;
            else
                outputFile << "Approximate Nearest neighbor: null" << endl;
            
            outputFile << "True Nearest neighbor: "<< bf[0].first << endl;
            
            if(!nn.empty()){
                outputFile << "distanceApproximate: "<< nn[0].second << endl;
                maf = nn[0].second / bf[0].second;

                if(maf > maxmaf)
                    maxmaf = maf;
            }
            else{
                outputFile << "distanceApproximate: null" << endl;
            }

            outputFile << "distanceTrue: "<< bf[0].second << endl;
            outputFile << endl;
            
            if(!nn.empty())
                statistics += nn[0].second - bf[0].second;

            delete[] buff;
            q.clear();
        }
        outputFile << "tApproximateAverage: " << tApproximateAverage / counter << " microseconds" << endl;
        outputFile << "tTrueAverage: " <<  tTrueAverage / counter << " microseconds" << endl;
        outputFile << "MAF: " << maxmaf << endl;

        cout << "Average distance variation: " << statistics/counter << endl;

            
        inputFile.close();
        outputFile.close();
    }
}


// Function that reads all the query points from the query file and executes the ANN algorithm (LSH_Frechet Continuous)
// It also generates the output file with the results
void lshCurvesContinuous(string input, string output, int N)
{
    vector<double> q;
    
    // Open the input and output files
    ifstream inputFile(input);
    
    ofstream outputFile(output);

    if (inputFile)
    {
        string line;
        auto tApproximateAverage = 0;
        auto tTrueAverage = 0;
        int counter = 0;
        double statistics = 0;
        double maf=0.0;
        double maxmaf=-1.0;

        // Read every line of the file
        while (getline(inputFile, line))
        {
            char* buff = new char[line.length()+1];
            strcpy(buff, line.c_str());
            char* token = strtok(buff," \t");

            char *id = token;

            token = strtok(NULL," \t");

            // Read all the coordinates of the query and store them in vector 'q'
            while (token != NULL)
            {
                q.push_back(atof(token));
                token = strtok (NULL, " \t");
            }

            // Find the N nearest neighbors using the ANN algorithm, their Frechet distance from 'q' and the time it takes to find them
            auto startLSH = chrono::steady_clock::now();
            vector<pair<string, double>> nn = LSH_hashTables->LSH_findCurvedNN_Continuous (q, N);
            auto endLSH = chrono::steady_clock::now();

            // Find the real Frechet distances of the N nearest neighbors from 'q' and the time it takes to find them
            auto startRealDist = chrono::steady_clock::now();
            vector<pair<string, double>> bf =  curves->findRealDistBruteForce_Continuous(q, N);
            auto endRealDist = chrono::steady_clock::now();

            tApproximateAverage += chrono::duration_cast<chrono::seconds>(endLSH - startLSH).count();
            tTrueAverage += chrono::duration_cast<chrono::seconds>(endRealDist - startRealDist).count();

            counter++;

            outputFile << "Query: " << id << endl;
            outputFile << "Algorithm: LSH_Frechet_Continuous" << endl;
            
            // Write all the results in the output file
            if(!nn.empty())
                outputFile << "Approximate Nearest neighbor: " << nn[0].first << endl;
            else
                outputFile << "Approximate Nearest neighbor: null" << endl;
            
            outputFile << "True Nearest neighbor: "<< bf[0].first << endl;
            
            if(!nn.empty()){
                outputFile << "distanceApproximate: "<< nn[0].second << endl;
                maf = nn[0].second / bf[0].second;

                if(maf > maxmaf)
                    maxmaf = maf;
            }
            else{
                outputFile << "distanceApproximate: null" << endl;
            }

            outputFile << "distanceTrue: "<< bf[0].second << endl;
            outputFile << endl;
            
            if(!nn.empty())
                statistics += nn[0].second - bf[0].second;

            delete[] buff;
            q.clear();
        }
        outputFile << "tApproximateAverage: " << tApproximateAverage / counter << " seconds" << endl;
        outputFile << "tTrueAverage: " <<  tTrueAverage / counter << " seconds" << endl;
        outputFile << "MAF: " << maxmaf << endl;

        cout << "Average distance variation: " << statistics/counter << endl;

            
        inputFile.close();
        outputFile.close();
    }
}