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

double FrechetDistance(const vector<pair<double, double>>& P, const vector<pair<double, double>>& Q)
{
    size_t m1 = P.size();
    size_t m2 = Q.size();

    vector<vector<double>> dp_matrix;

    dp_matrix.resize(m1);
    for (int i = 0; i < dp_matrix.size(); i++) {

        dp_matrix[i].resize(m2);
    }

    dp_matrix[0][0] = euclidean_distance(P, Q);

    // Fill the matrix with the right values by following the steps of the algorithm
    for (size_t i = 0; i < m1; i++)
    {
        for (size_t j = 0; j < m2; j++)
        {
            if (i == 0 && j > 0)
                dp_matrix[0][j] = max(dp_matrix[0][j - 1], euclidean_distance(P[0], Q[j]));
            else if (i > 0 && j == 0)
                dp_matrix[i][0] = max(dp_matrix[i - 1][0], euclidean_distance(P[i], Q[0]));
            else if (i > 0 && j > 0)
                dp_matrix[i][j] = max(min(min(dp_matrix[i - 1][j], dp_matrix[i - 1][j - 1]), dp_matrix[i][j - 1]), euclidean_distance(P[i], Q[j]));
        }
    }
    
    return dp_matrix[m1-1][m2-1];
}

// Function that finds an optimal traversal of two curves
vector<pair<int, int>> OptimalTraversal(const vector<pair<double, double>>& P, const vector<pair<double, double>>& Q)
{
    size_t m1 = P.size();
    size_t m2 = Q.size();

    vector<vector<double>> dp_matrix;
    vector<pair<int, int>> traversal;
    int pi, qj;

    dp_matrix.resize(m1);
    for (int i = 0; i < dp_matrix.size(); i++) {

        dp_matrix[i].resize(m2);
    }

    dp_matrix[0][0] = euclidean_distance(P, Q);

    // Fill the matrix with the right values by following the steps of the algorithm
    for (size_t i = 0; i < m1; i++)
    {
        for (size_t j = 0; j < m2; j++)
        {
            if (i == 0 && j > 0)
                dp_matrix[0][j] = max(dp_matrix[0][j - 1], euclidean_distance(P[0], Q[j]));
            else if (i > 0 && j == 0)
                dp_matrix[i][0] = max(dp_matrix[i - 1][0], euclidean_distance(P[i], Q[0]));
            else if (i > 0 && j > 0)
                dp_matrix[i][j] = max(min(min(dp_matrix[i - 1][j], dp_matrix[i - 1][j - 1]), dp_matrix[i][j - 1]), euclidean_distance(P[i], Q[j]));
        }
    }
    
    pi = m1 - 1;
    qj = m2 - 1;
    traversal.push_back(make_pair(pi, qj));

    // Pick the right indexes by comparing three different values from the matrix in every iteration and picking the lowest one
    // Then insert the position of that value in the vector 'traversal'
    while (pi != 0 || qj != 0)
    {
        if (pi == 0)
        {
            while (qj != 0)
            {
                traversal.push_back(make_pair(pi, --qj));
            }
        }
        else if (qj == 0)
        {
            while (pi != 0)
            {
                traversal.push_back(make_pair(--pi, qj));
            }
        }
        else if (dp_matrix[pi - 1][qj] <= dp_matrix[pi][qj - 1] && dp_matrix[pi - 1][qj] <= dp_matrix[pi - 1][qj - 1])
        {
            traversal.push_back(make_pair(--pi, qj));
        }
        else if (dp_matrix[pi][qj - 1] <= dp_matrix[pi - 1][qj] && dp_matrix[pi][qj - 1] <= dp_matrix[pi - 1][qj - 1])
        {
            traversal.push_back(make_pair(pi, --qj));
        }
        else
        {
            traversal.push_back(make_pair(--pi, --qj));
        }
    }

    reverse(traversal.begin(), traversal.end());

    return traversal;
}
