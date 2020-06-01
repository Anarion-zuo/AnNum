//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_MODEL_H
#define ANNUM_MODEL_H

#include <series/Matrix.h>
#include <operators/RandomGenerator.h>

namespace anarion {
    struct ModelException : public std::exception {};

    struct TrainDataShapeDifferent: public ModelException {
        const char *what() const noexcept override {
            return "X and y have different number of data";
        }
    };

    class Model {
    protected:
        float64 learningRate = 1e-3;
        uint64 maxIteration = 1000;
        float64 epsilon = 1e-5;
        float64 precision = 1e-4;

        // operators
        RandomGenerator randGen;

        constexpr bool isPrecise(float64 val) {
            return (val < precision) && (val > (-precision));
        }
    };
}

#endif //ANNUM_MODEL_H
