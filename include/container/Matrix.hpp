//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_MATRIX_HPP
#define ANNUM_MATRIX_HPP

//#include "Array-old.hpp"
#include "Array.hpp"

namespace anarion {
    /*
     * first colIndex for a column
     */
    template<typename num_type>
    class Matrix : public Array<Array<num_type>> {
        using parent_type = Array<Array<num_type>>;
        using this_type = Matrix<num_type>;
        using less_type = Array<num_type>;
    public:
        Matrix() = default;
        Matrix(const this_type &) = default;
        Matrix(this_type &&) noexcept = default;
        virtual ~Matrix() = default;
        this_type &operator=(const this_type &) = default;
        this_type &operator=(this_type &&) noexcept = default;

        Matrix(size_type width, size_type height, const num_type &number) : parent_type(width, Array<num_type>(height, number)) {}

        /*
        Matrix(std::initializer_list<less_type> &&initList) {
            size_type rowCount = initList.size();
            if (rowCount == 0) { return; }
            size_type colCount = initList[0].getDimension();
            for (size_type colIndex = 0; colIndex < colCount; ++colIndex) {
                for (size_type rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
                    initList[rowIndex][colIndex];
                }
            }
        }
         */

        constexpr size_type getWidth() const {
            return this->getDimension();
        }

        size_type getHeight() const {
            if (this->empty()) {
                return 0;
            }
            return this->get(0).getDimension();
        }

        constexpr Array<num_type> &getColumn(size_type index) {
            return this->begin[index];
        }

        constexpr const Array<num_type> &getColumn(size_type index) const {
            return this->begin[index];
        }

        constexpr num_type &getEntry(size_type rowIndex, size_type colIndex) {
            return getColumn(colIndex).getEntry(rowIndex);
        }

        constexpr const num_type &getEntry(size_type rowIndex, size_type colIndex) const {
            return getColumn(colIndex).getEntry(rowIndex);
        }

        less_type getRow(size_type rowIndex) const {
            less_type row(getWidth(), 0);
            for (size_type colIndex = 0; colIndex < getWidth(); ++colIndex) {
                row.getEntry(colIndex) = getEntry(colIndex, rowIndex);
            }
            return move(row);
        }

        less_type sum(size_type beginIndex, size_type endIndex) {
            if (getWidth() == 0) {
                return less_type();
            }
            less_type ret { getWidth(), 0 };
            for (size_type index = 0; index < getWidth(); ++index) {
                ret[index] = getColumn(index).sum(beginIndex, endIndex);
            }
            return move(ret);
        }

        Array<num_type> sum() {
            return sum(0, getHeight());
        }


    };
}

#endif //ANNUM_MATRIX_HPP
