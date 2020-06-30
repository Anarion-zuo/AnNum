//
// Created by anarion on 6/29/20.
//

#ifndef ANNUM_SERIES_H
#define ANNUM_SERIES_H

#include <container/List/Vector.hpp>
#include <limits>
#include <cmath>

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long;
using float32 = float;
using float64 = double;
using float80 = long double;

namespace anarion {
    enum Dtype {
        Int8, Int16, Int32, Int64,
        Float32, Float64
    };



    template <typename num_type>
    struct dtype_trait { const static Dtype dtype = Dtype::Float32; };
    template <> struct dtype_trait<int8> {
        const static Dtype dtype = Dtype::Int8;
    };
    template <> struct dtype_trait<int16> {
        const static Dtype dtype = Dtype::Int16;
    };
    template <>
    struct dtype_trait<int32> {
        const static Dtype dtype = Dtype::Int64;
    };
    template <>
    struct dtype_trait<int64> {
        const static Dtype dtype = Dtype::Int64;
    };
    template <>
    struct dtype_trait<float32> {
        const static Dtype dtype = Dtype::Float32;
    };
    template <>
    struct dtype_trait<float64> {
        const static Dtype dtype = Dtype::Float64;
    };

    template<typename num_type> Dtype getDtypeByTraints() { return dtype_trait<num_type>::dtype; }

    struct InvalidDtype : public std::exception {};

    struct SeriesException : public std::exception {};
    struct DtypeNotMatch : public SeriesException {};
    struct SeriesLengthNotMatch : public SeriesException {};
    /*
     * @Class: Series
     * @Description: static polymorphism on number types
     */

    class Series : public Vector<char> {
        friend class Matrix;
        using parent_type = Vector<char>;
    protected:
        Dtype dtype;   // showing the storage type of this array
        uint8 numberLength;   // the length of a number in bytes, computed with dtype
        static uint8 computeNumberLengthByDtype(Dtype dtype);
        void setNumberImpl(size_type index, const void *number);
        void insertNumberImpl(size_type index, const void *number);

        template <typename num_type>
        void throwDtypeNotMatch() const {
            if (getDtypeByTraints<num_type>() != dtype) {
                throw DtypeNotMatch();
            }
        }

        /*
         * Operations
         */
#define arithmetic_operators_series(op) \
    size_type length = size();\
    if (length != rhs.size()) {\
        throw SeriesLengthNotMatch();\
    }\
    \
    for (size_type index = 0; index < length; ++index) {\
        getNumber<num_type>(index) op rhs.getNumber<num_type>(index);\
    }

#define arithmetic_operators_numbers(op) \
    size_type length = size();\
    if (length != rhs.size()) {\
        throw SeriesLengthNotMatch();\
    }\
    for (size_type index = 0; index < length; ++index) {\
        getNumber<num_type>(index) op rhs;\
    }

        template<typename num_type>
        void addImpl(const Series &rhs) { arithmetic_operators_series(+=) }
        template<typename num_type>
        void subImpl(const Series &rhs) { arithmetic_operators_series(-=) }
        template<typename num_type>
        void mulImpl(const Series &rhs) { arithmetic_operators_series(*=) }
        template<typename num_type>
        void divImpl(const Series &rhs) { arithmetic_operators_series(/=) }

        template<typename num_type>
        num_type sumImpl() const {
            size_type length = size();
            num_type res { 0 };
            for (size_type index = 0; index < length; ++index) {
                res += getNumber<num_type>(index);
            }
            return res;
        }
        template<typename num_type>
        num_type prodImpl() const {
            size_type length = size();
            num_type res { 1 };
            for (size_type index = 0; index < length; ++index) {
                res *= getNumber<num_type>(index);
            }
            return res;
        }
        template<typename num_type>
        num_type meanImpl() const {
            return sumImpl<num_type>() / size();
        }
        template<typename num_type>
        float64 varianceImpl() const {
            num_type meanVal = meanImpl<num_type>();
            float64 var = 0;
            for (size_type index = 0; index < size(); ++index) {
                num_type diff = getNumber<num_type>(index) - meanVal;
                var += diff * diff;
            }
            return var / (size() - 1);
        }
        template<typename num_type>
        void standardize() {
            num_type meanVal = mean();
            float64 var = 0;
            for (size_type index = 0; index < size(); ++index) {
                num_type &ref = getNumber<num_type>(index);
                num_type diff = ref - meanVal;
                var += diff * diff;
                ref = diff;
            }
            var /= (size() - 1);
            var = sqrt(var);
            div(var);
        }

#define convert_known_type(type) \
    converted = (num_type)*(type*)buf;\
    newSeries.insertNumber(index, converted);\
    break;

        template <typename num_type>
        Series convertTypeImpl() const {
            char buf[numberLength];
            size_type length = size();
            Series newSeries = Series::createByType<num_type>(length);
            num_type converted;
            for (size_type index = 0; index < length; ++index) {
                getNumber(index, buf);
                switch (dtype) {
                    case Dtype::Int8:
                        convert_known_type(int8)
                    case Dtype::Int16:
                        convert_known_type(int16)
                    case Dtype::Int32:
                        convert_known_type(int32)
                    case Dtype::Int64:
                        convert_known_type(int64)
                    case Dtype::Float32:
                        convert_known_type(float32)
                    case Dtype::Float64:
                        convert_known_type(float64)
                    default:
                        throw InvalidDtype();
                }
            }
            return move(newSeries);
        }

    public:

        ~Series() override = default;

        explicit Series(Dtype dtype) : dtype(dtype), numberLength(computeNumberLengthByDtype(dtype)) {}
/*
        template<typename num_type>
        Series() : dtype(getDtype<num_type>()) {
            setNumberLengthByDtype();
        }
*/
        template <typename num_type>
        static Series createByType() {
            return Series(getDtypeByTraints<num_type>());
        }
        template <typename num_type, typename ...Args>
        static Series createByType(Args &&...args) {
            return Series(getDtypeByTraints<num_type>(), forward<Args...>(args...));
        }
        template<typename num_type>
        static Series createByType(size_type initSize, const num_type &number) {
            Series series { createByType<num_type>(initSize) };
            for (size_type index = 0; index < initSize; ++index) {
                series.append(number);
            }
            return move(series);
        }

        Series(Dtype dtype, size_type initSize);
        Series(const Series &rhs);
        Series(Series &&rhs) noexcept ;
        Series &operator=(const Series &rhs);
        Series &operator=(Series &&rhs) noexcept ;

        constexpr Dtype getDtype() const { return dtype; }
        bool isInteger() const { return isDtypeInteger(dtype); }
        constexpr size_type getNumberLength() const { return numberLength; }
        Series cloneType(size_type initSize = 0) const;   // create empty object with same type
        static bool isDtypeInteger(Dtype dtype);

        constexpr size_type size() const { return parent_type::size() / numberLength; }
        constexpr size_type capacity() const { return parent_type::capacity() / numberLength; }

        void getNumber(size_type index, void *out) const ;

        template <typename num_type>
        num_type &getNumber(size_type index) {
            if (index >= size()) {
                throw IndexOutOfRange();
            }
            return *(num_type*)(this->begin + index * numberLength);
        }

        template <typename num_type>
        const num_type &getNumber(size_type index) const {
            if (index >= size()) {
                throw IndexOutOfRange();
            }
            return *(num_type*)(this->begin + index * numberLength);
        }

        void removeNumber(size_type index);

        template<typename num_type>
        void setNumber(size_type index, const num_type &number) {
            throwDtypeNotMatch<num_type>();
            setNumberImpl(index, &number);
        }

        template<typename num_type>
        void insertNumber(size_type index, const num_type &number) {
            throwDtypeNotMatch<num_type>();
            insertNumberImpl(index, &number);
        }

        template<typename num_type>
        void append(const num_type &number) {
            insertNumber(size(), number);
        }

        Series convertType(Dtype dt) const ;

        // operators
        void add(const Series &rhs);
        void sub(const Series &rhs);
        void mul(const Series &rhs);
        void div(const Series &rhs);
        float64 dot(const Series &rhs);

        template<typename num_type>
        void add(const num_type &rhs) { arithmetic_operators_numbers(+=) }
        template<typename num_type>
        void sub(const num_type &rhs) { arithmetic_operators_numbers(-=) }
        template<typename num_type>
        void mul(const num_type &rhs) { arithmetic_operators_numbers(*=) }
        template<typename num_type>
        void div(const num_type &rhs) { arithmetic_operators_numbers(/=) }

        float64 sum() const ;
        float64 prod() const ;
        float64 mean() const ;
        float64 variance() const ;
    };
}

#endif //ANNUM_SERIES_H
