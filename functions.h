#pragma once
#include <functional>
#include <vector>
#include <math.h>

#define ROUND_TO 5

using namespace std;


int rangeHash(float x0, float x, float y0, float y) {
    hash<float> hashing;
    return hashing(round(x0*10000)) + hashing(round(x*20000)) + hashing(round(y0*30000)) + hashing(round(y*40000));
}