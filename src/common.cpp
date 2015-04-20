#include "common.h"
#include <random>
#include <cstdlib>
#include <iostream>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> uniform01_dis(0,1);

float uniform01() {
    return uniform01_dis(gen);
}

int uniformInt(int a, int b) {
    std::uniform_int_distribution<int> dis(a,b);
    return dis(gen);
}



