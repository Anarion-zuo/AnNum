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
        BigFloatArray thetas;   // target parameters
        float64 bias;
        void initTargets(size_type num);  // give thetas dimension

    public:
        LinearRegression();

        void fit(Matrix &X, ArrayInterface &y);
        float64 evaluate(const ArrayInterface &x) const ;
        anarion::float64 evaluate(anarion::Matrix &x, size_type rowIndex);

        float64 getBias() const { return bias; }
        const BigFloatArray &getThetas() const { return thetas; }
    };
}

#endif //ANNUM_LINEARREGRESSION_H
