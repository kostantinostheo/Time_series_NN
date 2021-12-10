#include "Frechet.h"
#include "Euclidean.h"

#include <cmath>

using namespace std;

double FrechetDistance(const vector<double> & P, const vector<double> & Q, double C)
{
    size_t m1 = P.size();
    size_t m2 = P.size();
    
    vector<vector<double>> dp_matrix;
    
    dp_matrix.resize( m1 );       // m1 rows
    for (int i = 0; i < dp_matrix.size(); i++) {
        
        dp_matrix[i].resize( m2 );        // m2 columns
    }

    
    dp_matrix[0][0] = euclidean_distance( P[0] , Q[0], 0, 0, C);
    
    for (size_t i = 0; i < m1; i++)
    {
        for (size_t j = 0; j < m2; j++)
        {
            if (i == 0 && j > 0)
                dp_matrix[0][j] = max( dp_matrix[0][j-1], euclidean_distance( P[0], Q[j], 0, j, C) );
            else if (i > 0 && j == 0)
                dp_matrix[i][0] = max( dp_matrix[i-1][0], euclidean_distance( P[i], Q[0], i, 0, C) );
            else if (i > 0 && j > 0)
                dp_matrix[i][j] = max( min( min(dp_matrix[i-1][j], dp_matrix[i-1][j-1]), dp_matrix[i][j-1] ), euclidean_distance( P[i], Q[j], i, j, C) );
        }
    }
    
    // ???
    return dp_matrix[m1-1][m2-1];
}
