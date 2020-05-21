//
// Created by 左钰 on 2020/5/20.
//

#include "operators/RandomGenerator.h"

long anarion::RandomGenerator::getSysRandom() {
    setSysSeed();
    return random();
}

void anarion::RandomGenerator::setSeed(unsigned int newSeed) {
    seed = newSeed;
}

void anarion::RandomGenerator::setSysSeed() {
    srandom(seed);
}
