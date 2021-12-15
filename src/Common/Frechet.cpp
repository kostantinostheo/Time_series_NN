#include "Frechet.h"
#include "Euclidean.h"

#include <cmath>

using namespace std;

// Function that calculates the Frechet distance between two curves
double FrechetDistance(const vector<double> & P, const vector<double> & Q, double freq)
{
    size_t m1 = P.size();
    size_t m2 = Q.size();
    
    vector<vector<double>> dp_matrix;
    
    dp_matrix.resize(m1);
    for (int i = 0; i < dp_matrix.size(); i++) {
        
        dp_matrix[i].resize(m2);
    }
    
    dp_matrix[0][0] = euclidean_distance(P[0], Q[0], 0, 0, freq);
    
    // Fill the matrix with the right values by following the steps of the algorithm
    for (size_t i = 0; i < m1; i++)
    {
        for (size_t j = 0; j < m2; j++)
        {
            if (i == 0 && j > 0)
                dp_matrix[0][j] = max(dp_matrix[0][j-1], euclidean_distance(P[0], Q[j], 0, j, freq));
            else if (i > 0 && j == 0)
                dp_matrix[i][0] = max(dp_matrix[i-1][0], euclidean_distance(P[i], Q[0], i, 0, freq));
            else if (i > 0 && j > 0)
                dp_matrix[i][j] = max(min(min(dp_matrix[i-1][j], dp_matrix[i-1][j-1]), dp_matrix[i][j-1]), euclidean_distance(P[i], Q[j], i, j, freq));
        }
    }
    
    return dp_matrix[m1-1][m2-1];
}

vector<pair<int, int>> OptimalTraversal(const vector<double>& P, const vector<double>& Q, double freq)
{
    size_t m1 = P.size();
    size_t m2 = Q.size();

    vector<vector<pair<double, pair<int, int>>>> dp_matrix;
    vector<pair<int, int>> traversal;

    dp_matrix.resize(m1);
    for (int i = 0; i < dp_matrix.size(); i++) {

        dp_matrix[i].resize(m2);
    }

    dp_matrix[0][0].first = euclidean_distance(P[0], Q[0], 0, 0, freq);

    // Fill the matrix with the right values by following the steps of the algorithm
    for (size_t i = 0; i < m1; i++)
    {
        for (size_t j = 0; j < m2; j++)
        {
            if (i == 0 && j > 0)
            {
                dp_matrix[0][j].first = max(dp_matrix[0][j - 1].first, euclidean_distance(P[0], Q[j], 0, j, freq));
                dp_matrix[0][j].second = make_pair(-1, -1);
            }
            else if (i > 0 && j == 0)
            {
                dp_matrix[i][0].first = (dp_matrix[i - 1][0].first, euclidean_distance(P[i], Q[0], i, 0, freq));
                dp_matrix[i][0].second = make_pair(-1, -1);
            }
            else if (i > 0 && j > 0)
            {
                if (dp_matrix[i - 1][j].first <= dp_matrix[i - 1][j - 1].first && dp_matrix[i - 1][j].first <= dp_matrix[i][j - 1].first)
                {
                    dp_matrix[i][j].first = max(dp_matrix[i - 1][j].first, euclidean_distance(P[i], Q[j], i, j, freq));
                    dp_matrix[i][j].second = make_pair(i - 1, j);
                }
                else if (dp_matrix[i - 1][j - 1].first <= dp_matrix[i - 1][j].first && dp_matrix[i - 1][j - 1].first <= dp_matrix[i][j - 1].first)
                {
                    dp_matrix[i][j].first = max(dp_matrix[i - 1][j - 1].first, euclidean_distance(P[i], Q[j], i, j, freq));
                    dp_matrix[i][j].second = make_pair(i - 1, j - 1);
                }
                else
                {
                    dp_matrix[i][j].first = max(dp_matrix[i][j - 1].first, euclidean_distance(P[i], Q[j], i, j, freq));
                    dp_matrix[i][j].second = make_pair(i, j - 1);
                }
            }
        }
    }
    
    
}
