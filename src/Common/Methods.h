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
void CurvesLSH_pre_process(string filename, int L, bool discrete=true);

void lsh(string input, string output, int N);
void cube(string input, string output, int N, int k, int maxPoints, int probes);
void lshCurvesDiscrete(string input, string output, int N, double freq);
void lshCurvesContinuous(string input, string output, int N);

#endif

