//
// Created by anarion on 6/30/20.
//

#ifndef ANNUM_MATRIX_H
#define ANNUM_MATRIX_H

#include "Series.h"

namespace anarion {

    struct MatrixException : public std::exception {};
    struct MatrixShapeNotMatch : public MatrixException {};

    class Matrix {
    protected:
        Vector<Series> columns;

        void getNumberImpl(size_type rowIndex, size_type colIndex, void *out);

    public:

        Matrix() = default;
        ~Matrix();
        void clear();
        Matrix(const Matrix &rhs);
        Matrix(Matrix &&rhs) noexcept ;
        Matrix &operator=(const Matrix &rhs);
        Matrix &operator=(Matrix &&rhs) noexcept ;

        explicit Matrix(Vector<Series> &&columns);
        Matrix(Dtype dtype, anarion::size_type initRowCount, anarion::size_type initColCount);

        constexpr size_type getWidth() const { return columns.size(); }
        constexpr size_type getHeight() const {
            if (columns.empty()) { return 0; }
            return columns.get(0).size();
        }

        Series &getColumn(size_type colIndex);
        const Series &getColumn(size_type colIndex) const ;

        Dtype generalType() const ;
        Matrix cloneType(size_type initColCount = 0) const ;

        template <typename num_type>
        num_type &getNumber(size_type rowIndex, size_type colIndex) {
            return getColumn(colIndex).getNumber<num_type>(rowIndex);
        }

        template <typename num_type>
        num_type &getNumber(size_type rowIndex, size_type colIndex) const {
            return getColumn(colIndex).getNumber<num_type>(rowIndex);
        }

        template <typename num_type>
        Series getRow(size_type rowIndex) const {
            size_type width = getWidth();
            Series row { Series::createByType<num_type>(width) };
            for (size_type index = 0; index < width; ++width) {
                row.append(getNumber<num_type>(rowIndex, index));
            }
            return move(row);
        }

        void convertType(Dtype dtype);

        // operations
        void add(const Matrix &rhs);
        void sub(const Matrix &rhs);
        Matrix mul(const Matrix &rhs) const ;  // simple matrix multiplication without any optimization
        Series mul(const Series &rhs) const ;  // simple matrix&vector multiplication

        Series columnsSum() const ;
        Series columnsVariance() const ;
        Matrix transpose() const ;
        Matrix solveLinear(const Series &rhs);   // solve linear equation
        Matrix inverse() const ;
    };

}

#endif //ANNUM_MATRIX_H
