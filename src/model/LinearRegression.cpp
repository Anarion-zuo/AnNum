//
// Created by 左钰 on 2020/5/20.
//

#include "model/LinearRegression.h"
#include <container/Array.hpp>

void anarion::LinearRegression::fit(anarion::Matrix<anarion::float64> &X, anarion::Array<anarion::float64> &y) {
    size_type rowCount = X.getHeight();
    if (rowCount != y.size()) {
        throw TrainDataShapeDifferent();
    }
    size_type colCount = X.getWidth();
    size_type iter = 0;
    initTargets(colCount);
    while (iter < maxIteration) {
        randGen.setSeed(time(nullptr));
        size_type rowIndex = randGen.getInt(0ul, rowCount);
        auto row = X.getRow(rowIndex);
        bool shouldBreak = true;
        Array<float64> newThetas(X.getWidth(), 0);
        for (size_type colIndex = 0; colIndex < colCount; ++colIndex) {
            float64 theta = thetas[colIndex];
            float64 yhat = (row * thetas).sum() + bias;
            float64 nval = learningRate * row[colIndex] * (y[rowIndex] - yhat);
            if (shouldBreak) {
                shouldBreak = shouldBreak && isPrecise(nval);
            }
            theta += nval;
            newThetas[colIndex] = (theta);
        }
        thetas = newThetas;
        if (shouldBreak) {
            break;
        }
        ++iter;
    }
}

anarion::LinearRegression::LinearRegression() {

}

anarion::float64 anarion::LinearRegression::evaluate(anarion::Array<anarion::float64> &x) {
    auto product = thetas * x;
    auto dotp = product.sum();
    return bias + dotp;
}

void anarion::LinearRegression::initTargets(anarion::size_type num) {
    thetas = Array<float64>(num, 0);
    bias = 0;
}
