#define _CRT_SECURE_NO_WARNINGS
#include "Tools.h"

// Function that is given as an argument to the 'sort' function in order to sort a vector that contains pairs
bool sortbyDist(const pair<string, double> &a, const pair<string, double> &b)
{
    return a.second < b.second;
}

// Function that calculates the hamming distance between two integers
unsigned int HammingDistance(unsigned int a, unsigned int b)
{
    // Get the XOR of the two numbers
    unsigned int Xor = a ^ b;

    // The bits of the XOR between a and b that are 1
    unsigned int bits = 0;

    // Count the numbers of bits that are 1
    while (Xor > 0)
    {
        // If the rightmost bit of 'Xor' is 1 then Xor & 1 = 1
        bits += Xor & 1;

        // Perform right shift on 'Xor' to get the next bits
        Xor >>= 1;
    }

    return bits;
}



// Function that finds the number of coordinates that a point has
int dimension(string filename)
{
    int dim = 0; 

    ifstream infile(filename);
    string  line; 

    getline(infile, line);

    dim = count(line.begin(), line.end(), ' ')-1;

    infile.close();

    return dim;
}

// Function that counts the number of lines in a file
int count_file_lines(string filename)
{
    int count = 0;
    ifstream infile(filename);
    if (infile)
    {
        string line;
        while (getline(infile, line))
        {
            count++;
        }
    }
    infile.close();
    return count;
}