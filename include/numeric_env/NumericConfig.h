//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_NUMERICCONFIG_H
#define ANNUM_NUMERICCONFIG_H

/*
 * The configuration class
 * specifying a series of configurations ere running
 */

namespace anarion {

    typedef unsigned long size_type;

    class NumericConfig {
    protected:
        size_type maxIteration = 10000;
        double epsilon = 1e-6;

    public:
        constexpr size_type getMaxIteration() const { return maxIteration; }
        constexpr double getEpsilon() const { return epsilon; }
    };
}

#endif //ANNUM_NUMERICCONFIG_H
