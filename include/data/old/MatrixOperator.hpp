//
// Created by anarion on 6/30/20.
//

#ifndef ANNUM_MATRIXOPERATOR_HPP
#define ANNUM_MATRIXOPERATOR_HPP

#include "Matrix.h"
#include "SeriesOperator.h"

namespace anarion {
    class MatrixOperator {
    public:
        Matrix sumColumns(const Matrix &matrix) {
            size_type colCount = matrix.getWidth();
            Matrix res = matrix.cloneType(1);
            for (size_type index = 0; index < colCount; ++index) {

            }

        }
    };
}

#endif //ANNUM_MATRIXOPERATOR_HPP
