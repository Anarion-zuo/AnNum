//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_LINEARREGRESSION_H
#define ANNUM_LINEARREGRESSION_H

#include "Model.h"

namespace anarion {
    struct LinearModelException : public ModelException {};

    class LinearRegression : public Model {
    protected:
        Array<float64> thetas;   // target parameters
        float64 bias;
        void initTargets(size_type num);  // give thetas dimension

        float64 evaluate(Array<float64> &x);
    public:
        LinearRegression();

        void fit(Matrix<float64> &X, Array<float64> &y);
    };
}

#endif //ANNUM_LINEARREGRESSION_H
