//
// Created by 左钰 on 2020/5/21.
//

#ifndef ANNUM_ARRAY_HPP
#define ANNUM_ARRAY_HPP

#include <container/List/Vector.hpp>
#include "number_trait.h"

namespace anarion {

    struct ArrayException : std::exception {};
    struct ArrayIndexOutOfRange : public ArrayException {
        const char *what() const noexcept override {
            return "Array-old colIndex out of bound";
        }
    };
    struct ArrayNotMyIterator : public ArrayException {
        const char *what() const noexcept override {
            return "Operating on an iterator of a different Array-old";
        }
    };
    struct ArrayOperationShapeDifferent : public ArrayException {
        const char *what() const noexcept override {
            return "Operation on 2 arrays with different size/shape";
        }
    };

    template <typename num_type>
    class Array : public Vector<num_type> {
        using this_type = Array<num_type>;

    public:
        Array() = default;
        Array(const this_type &) = default;
        Array(this_type &&) noexcept = default;
        virtual ~Array() = default;
        this_type &operator=(const this_type &) = default;
        this_type &operator=(this_type &&) noexcept = default;

        Array(size_type initSize, const num_type &number) : Vector<num_type>(initSize, number) {}

        Array(std::initializer_list<num_type> &&initList) : Vector<num_type>(anarion::forward<std::initializer_list<num_type>>(initList)) {}

        this_type &operator=(const num_type &number) {
            for (size_type index = 0; index < this->size(); ++index) {
                this->begin[index] = number;
            }
            return *this;
        }

        constexpr size_type getDimension() const {
            return this->size();
        }

        constexpr num_type &getEntry(size_type index) {
            return this->begin[index];
        }

        constexpr const num_type &getEntry(size_type index) const {
            return this->begin[index];
        }

        num_type sum(size_type beginIndex, size_type endIndex) const {
            size_type length = endIndex - beginIndex;
            if (length > this->size() - beginIndex) {
                throw IndexOutOfRange();
            }
            num_type *arr = this->begin + beginIndex;
            num_type ret = num_trait<num_type>::zero;
            for (size_type index = 0; index < length; ++index) {
                ret += arr[index];
            }
            return move(ret);
        }

        num_type sum() const {
            this->sum(0, this->size());
        }

        num_type mean(size_type beginIndex, size_type endIndex) const {
            size_type length = endIndex - beginIndex;
            if (length > this->size() - beginIndex) {
                throw IndexOutOfRange();
            }
            num_type *arr = this->begin + beginIndex;
            num_type ret = num_trait<num_type>::zero;
            for (size_type index = 0; index < length; ++index) {
                ret = (ret * index + arr[index]) / (index + 1);
                ++index;
            }
            return move(ret);
        }

        num_type mean() const {
            mean(0, this->size());
        }

#pragma region operators
#define arithmetic_operator_array(character) \
        size_type length = this->size();\
        if (length != rhs.size()) {\
            throw ArrayOperationShapeDifferent();\
        }\
        this_type ret(length, num_trait<num_type>::zero);\
        for (size_type index = 0; index < length; ++index) {\
            ret.begin[index] = this->begin[index] character rhs.begin[index];\
        }\
        return move(ret);
#define arithmetic_operator_number(character) \
        size_type length = this->size();\
        this_type ret(length, num_trait<num_type>::zero);\
        for (size_type index = 0; index < length; ++index) {\
            ret.begin[index] = this->begin[index] character number;\
        }\
        return move(ret);

        this_type operator+(const this_type &rhs) const {
            arithmetic_operator_array(+)
        }

        this_type operator-(const this_type &rhs) const {
            arithmetic_operator_array(-)
        }

        this_type operator*(const this_type &rhs) const {
            arithmetic_operator_array(*)
        }

        this_type operator/(const this_type &rhs) const {
            arithmetic_operator_array(/)
        }

        this_type operator-() const {
            this_type ret(this->size());
            for (size_type index = 0; index < this->size(); ++index) {
                ret.begin[index] = -this->begin[index];
            }
            return move(ret);
        }

        this_type operator+(const num_type &number) {
            arithmetic_operator_number(+)
        }

        this_type operator-(const num_type &number) {
            arithmetic_operator_number(-)
        }

        this_type operator*(const num_type &number) {
            arithmetic_operator_number(*)
        }

        this_type operator/(const num_type &number) {
            arithmetic_operator_number(/)
        }

#define arithmetic_assign_operator_number(character) \
        size_type length = this->size();\
        if (length != rhs.size()) {\
            throw ArrayOperationShapeDifferent();\
        }\
        for (size_type index = 0; index < length; ++index) {\
            this->begin[index] character rhs.begin[index];\
        }\
        return *this;

        this_type &operator+=(const this_type &rhs) {
            arithmetic_assign_operator_number(+=)
        }

        this_type &operator-=(const this_type &rhs) {
            arithmetic_assign_operator_number(-=)
        }

        this_type &operator*=(const this_type &rhs) {
            arithmetic_assign_operator_number(*=)
        }

        this_type &operator/=(const this_type &rhs) {
            arithmetic_assign_operator_number(/=)
        }

        bool operator==(const this_type &rhs) const {
            size_type length = this->size();
            if (length != rhs.size()) {
                throw ArrayOperationShapeDifferent();
            }
            for (size_type index = 0; index < length; ++index) {
                if (this->begin[index] != rhs.begin[index]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const this_type &rhs) const {
            return !(rhs == *this);
        }

        bool operator==(const num_type &number) const {
            for (size_type index = 0; index < this->size(); ++index) {
                if (this->begin[index] != number) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const num_type &number) const {
            return !(*this == number);
        }

#pragma endregion

        static num_type dot(const this_type &arr1, const this_type &arr2) {
            return (arr1 * arr2).sum();
        }
    };

    template<typename num_type>
    struct num_trait<Array<num_type>> {
        using this_type = Array<num_type>;
        using number_type = num_type;
        using isInteger = false_type;
        using isPOD = false_type;
        using isUnsigned = false_type;
        using isMovable = true_type;

        constexpr static const size_type size = sizeof(this_type);
    };
}

#endif //ANNUM_ARRAY_HPP
