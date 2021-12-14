#include <iostream>
#include "lib/acutest.hpp"
#include "../src/Common/Tools.h"

using namespace std;

void testFileDimension(void){
    TEST_CHECK_(dimension("../../dir/input_small_id") < 200, "dimension(%s)==%d" , "../../dir/input_small_id" , 200);
}

void countFileLines(void){
    TEST_CHECK_(count_file_lines("../../dir/input_small_id") < 500, "count_file_lines(%s)==%d" , "../../dir/input_small_id" , 500);
}
TEST_LIST = {
    {"'Input file dimensions must be less than 200'", testFileDimension},
    {"'Input file should have less than 500 lines'", countFileLines},
    {0}
};