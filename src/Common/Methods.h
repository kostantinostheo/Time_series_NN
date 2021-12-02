#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>


using namespace std;

void LSH_pre_process(string filename, int L);
void Cube_pre_process(string filename, int k);

void lsh(string input, string output, int N, double R);
void cube(string input, string output, int N, int k, double R, int maxPoints, int probes);

#endif

