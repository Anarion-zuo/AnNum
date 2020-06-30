//
// Created by anarion on 6/30/20.
//

#include "data/Matrix.h"

void anarion::Matrix::clear() {
    columns.clear();
}

anarion::Matrix::~Matrix() {
    clear();
}

anarion::Matrix::Matrix(const anarion::Matrix &rhs)
: columns(rhs.columns) {

}

anarion::Matrix::Matrix(anarion::Matrix &&rhs) noexcept
: columns(move(rhs.columns)) {

}

anarion::Matrix &anarion::Matrix::operator=(const anarion::Matrix &rhs) {
    if (&rhs == this) { return *this; }
    columns = rhs.columns;
    return *this;
}

anarion::Matrix &anarion::Matrix::operator=(anarion::Matrix &&rhs) noexcept {
    columns = move(rhs.columns);
    return *this;
}

anarion::Series &anarion::Matrix::getColumn(anarion::size_type colIndex) {
    return columns.get(colIndex);
}

const anarion::Series &anarion::Matrix::getColumn(anarion::size_type colIndex) const {
    return columns.get(colIndex);
}

anarion::Dtype anarion::Matrix::generalType() const {
    size_type numberLength = 0;
    bool isInteger = true;
    Dtype dtype = Dtype::Float32;
    for (auto it = columns.begin_iterator(); it != columns.end_iterator(); ++it) {
        if (isInteger) {
            if (!it->isInteger()) {
                isInteger = false;
                numberLength = it->getNumberLength();
                dtype = it->getDtype();
            } else {
                if (it->getNumberLength() > numberLength) {
                    numberLength = it->getNumberLength();
                    dtype = it->getDtype();
                }
            }
        } else {
            if (it->getNumberLength() > numberLength) {
                numberLength = it->getNumberLength();
                dtype = it->getDtype();
            }
        }
    }
    return dtype;
}

anarion::Matrix anarion::Matrix::cloneType(size_type initColCount) const {
    Vector<Series> newCols(columns.size());
    for (auto it = columns.begin_iterator(); it != columns.end_iterator(); ++it) {
        newCols.push_back(Series(it->getDtype(), initColCount));
    }
    return Matrix(move(newCols));
}

anarion::Matrix::Matrix(anarion::Vector<anarion::Series> &&columns)
: columns(forward<Vector<Series>>(columns)) {

}

anarion::Matrix::Matrix(
        Dtype dtype,
        anarion::size_type initRowCount,
        anarion::size_type initColCount
        )
: columns(initRowCount, Series(dtype, initColCount)) {

}

anarion::Series anarion::Matrix::columnsSum() const {
    Series res (Dtype::Float64, getWidth());
    for (size_type index = 0; index < getWidth(); ++index) {
        res.append(getColumn(index).sum());
    }
    return move(res);
}

anarion::Series anarion::Matrix::columnsVariance() const {
    Series res (Dtype::Float64, getWidth());
    for (size_type index = 0; index < getWidth(); ++index) {
        res.append(getColumn(index).variance());
    }
    return move(res);
}

anarion::Matrix anarion::Matrix::transpose() const {
    Matrix matrix(Dtype::Float64, getWidth(), getHeight());
    for (size_type colIndex = 0; colIndex < getWidth(); ++colIndex) {
        const Series *col = &getColumn(colIndex);
        Series back(Dtype::Float64);
        // convert if not match
        if (col->getDtype() != Dtype::Float64) {
            back = col->convertType(Dtype::Float64);
            col = &back;
        }
        for (size_type rowIndex = 0; rowIndex < getHeight(); ++rowIndex) {
            matrix.getColumn(rowIndex).append(col->getNumber<float64>(rowIndex));
        }
    }
    return move(matrix);
}

void anarion::Matrix::add(const anarion::Matrix &rhs) {
    size_type rowCount = getHeight(), colCount = getWidth();
    if (rhs.getHeight() != rowCount || rhs.getWidth() != colCount) {
        throw MatrixShapeNotMatch();
    }
    for (size_type index = 0; index < colCount; ++index) {
        columns.get(index).add(rhs.columns.get(index));
    }
}

void anarion::Matrix::sub(const anarion::Matrix &rhs) {
    size_type rowCount = getHeight(), colCount = getWidth();
    if (rhs.getHeight() != rowCount || rhs.getWidth() != colCount) {
        throw MatrixShapeNotMatch();
    }
    for (size_type index = 0; index < colCount; ++index) {
        columns.get(index).sub(rhs.columns.get(index));
    }
}

anarion::Matrix anarion::Matrix::mul(const anarion::Matrix &rhs) const {
    // check for shape
    size_type newHeight = getHeight(), newWidth = rhs.getWidth();
    if (getWidth() != rhs.getHeight()) {
        throw MatrixShapeNotMatch();
    }
    Matrix left = *this, right = rhs;
    left.convertType(Dtype::Float64);
    right.convertType(Dtype::Float64);
    Matrix newMat(Dtype::Float64, newHeight, newWidth);
    for (size_type rowIndex = 0; rowIndex < newHeight; ++rowIndex) {
        for (size_type colIndex = 0; colIndex < newWidth; ++colIndex) {

        }
    }
}

void anarion::Matrix::convertType(anarion::Dtype dtype) {
    for (size_type index = 0; index < getWidth(); ++index) {
        Series &series = getColumn(index);
        if (series.getDtype() != dtype) {
            series = series.convertType(dtype);
        }
    }
}

anarion::Series anarion::Matrix::mul(const anarion::Series &rhs) const {
    size_type length = rhs.size();
    if (getWidth() != length) {
        throw MatrixShapeNotMatch();
    }

}
