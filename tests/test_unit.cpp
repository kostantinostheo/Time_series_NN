#include <iostream>
#include "lib/acutest.hpp"
#include "../src/Common/Tools.h"
#include "../src/Common/Euclidean.h"
#include "../src/Common/Frechet.h"

using namespace std;

void testFileDimension(void){
    TEST_CHECK_(dimension("../../dir/nasd_input.csv") < 200, "Expected file dimension less than %d", 200);
}

void countFileLines(void){
    TEST_CHECK_(count_file_lines("../../dir/nasd_input.csv") < 200, "Expected file lines to be less than %d" , 200);
}

void testEuclideanDist(void) {
    vector<double> p{2.0, 6.0, 12.0};
    vector<double> q{3.0, 4.0, 10.0};

    TEST_CHECK_(euclidean_distance(p, q) == 3.0, "Expected %f, got %f", 3.0, euclidean_distance(p, q));
}

void testFrechetDist(void) {
    vector<double> p{3.0, 4.0};
    vector<double> q{5.0, 8.0};
    int freq = 1;

    TEST_CHECK_(FrechetDistance(p, q, freq) == 4.0, "Expected %f, got %f", 4.0, FrechetDistance(p, q, freq));
}

TEST_LIST = {
    {"'Input file dimensions must be less than 200'", testFileDimension},
    {"'Input file should have less than 200 lines'", countFileLines},
    {"'Euclidean distance of points (2, 6, 12) and (3, 4, 12) should be 3'", testEuclideanDist},
    {"'Frechet distance of curves ((0,3), (1,4)) and ((0,5), (1,8)) should be 4'", testFrechetDist},
    {0}
};