//
// Created by 左钰 on 2020/5/20.
//

#include "model/LinearRegression.h"
#include <series/Array.h>
#include <operators/ArrayOperator.h>

void anarion::LinearRegression::fit(anarion::Matrix &X, anarion::ArrayInterface &y) {
    size_type rowCount = X.getHeight();
    if (rowCount != y.length()) {
        throw TrainDataShapeDifferent();
    }
    size_type colCount = X.getWidth();
    size_type iter = 0;
    initTargets(colCount);
    while (iter < maxIteration) {
        bool shouldBreak = true;
        BigFloatArray newThetas(X.getWidth(), 0);
        float64 batchVal = 0;
        for (size_type colIndex = 0; colIndex < colCount; ++colIndex) {
            float64 theta = thetas[colIndex];
            batchVal = 0;
            for (size_type rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
                batchVal += X.getFloat(rowIndex, colIndex) * (y.getFloat(rowIndex) - evaluate(X, rowIndex));
            }
            batchVal /= rowCount;
            float64 nval = learningRate * batchVal;
            if (shouldBreak) {
                shouldBreak = shouldBreak && isPrecise(nval);
            }
            theta += nval;
            newThetas[colIndex] = (theta);
        }
        float64 tbias = bias;
        batchVal = 0;
        for (size_type rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
            batchVal += (y.getFloat(rowIndex) - evaluate(X, rowIndex));
        }
        batchVal /= rowCount;
        tbias = learningRate * batchVal;

        thetas = newThetas;
        bias = tbias;
        if (shouldBreak) {
            break;
        }
        ++iter;
    }
}

anarion::LinearRegression::LinearRegression() {

}

anarion::float64 anarion::LinearRegression::evaluate(const ArrayInterface &x) const {
    auto product = ArrayOperator().mul(thetas, x);
    float64 dotp = ArrayOperator().sum(*product);
    delete product;
    return bias + dotp;
}

void anarion::LinearRegression::initTargets(anarion::size_type num) {
    float64 randVal = randGen.getFloat<float64>(0.0f, 2.0f);
    thetas = BigFloatArray(num, randVal);
    bias = randVal;
}

anarion::float64 anarion::LinearRegression::evaluate(anarion::Matrix &x, size_type rowIndex) {
    float64 product = 0;
    for (size_type colIndex = 0; colIndex < x.getWidth(); ++colIndex) {
        product += x.getFloat(rowIndex, colIndex) * thetas[colIndex];
    }
    return bias + product;
}
