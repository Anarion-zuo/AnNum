//
// Created by anarion on 6/29/20.
//

#ifndef ANNUM_SERIES_H
#define ANNUM_SERIES_H

#include <container/List/Vector.hpp>
#include <limits>

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
        Int32, Int64,
        Float32, Float64
    };



    template <typename num_type>
    struct dtype_trait { const static Dtype dtype = Dtype::Float32; };
    template <>
    struct dtype_trait<int32> { const static Dtype dtype = Dtype::Int64; };
    template <>
    struct dtype_trait<int64> { const static Dtype dtype = Dtype::Int64; };
    template <>
    struct dtype_trait<float32> { const static Dtype dtype = Dtype::Float32; };
    template <>
    struct dtype_trait<float64> { const static Dtype dtype = Dtype::Float64; };

    template<typename num_type> Dtype getDtype() { return dtype_trait<num_type>::dtype; }

struct InvalidDtype : public std::exception {};

    struct SeriesException : public std::exception {};
    struct DtypeNotMatch : public SeriesException {};
    /*
     * @Class: Series
     * @Description: static polymorphism on number types
     */

    class Series : public Vector<char> {
        using parent_type = Vector<char>;
    protected:
        Dtype dtype;   // showing the storage type of this array
        uint8 numberLength;   // the length of a number in bytes, computed with dtype
        uint8 computeNumberLengthByDtype();
        void setNumberImpl(size_type index, const void *number);
        void insertNumberImpl(size_type index, const void *number);

        template <typename num_type>
        void throwDtypeNotMatch() const {
            if (getDtype<num_type>() != dtype) {
                throw DtypeNotMatch();
            }
        }

    public:

        ~Series() override = default;

        explicit Series(Dtype dtype) : dtype(dtype), numberLength(computeNumberLengthByDtype()) {}
/*
        template<typename num_type>
        Series() : dtype(getDtype<num_type>()) {
            setNumberLengthByDtype();
        }
*/
        template <typename num_type>
        static Series createByType() {
            return Series(getDtype<num_type>());
        }
        template <typename num_type, typename ...Args>
        static Series createByType(Args &&...args) {
            return Series(getDtype<num_type>(), forward<Args...>(args...));
        }

        Series(Dtype dtype, size_type initSize);
        Series(const Series &rhs);
        Series(Series &&rhs) noexcept ;
        Series &operator=(const Series &rhs);
        Series &operator=(Series &&rhs) noexcept ;

        static bool isDtypeInteger(Dtype dtype);

        constexpr size_type size() const { return parent_type::size() / numberLength; }
        constexpr size_type capacity() const { return parent_type::capacity() / numberLength; }

        template <typename num_type>
        num_type &getNumber(size_type index) {
            throwDtypeNotMatch<num_type>();
            return *(num_type*)(this->begin + index * numberLength);
        }

        template <typename num_type>
        const num_type &getNumber(size_type index) const {
            throwDtypeNotMatch<num_type>();
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
    };
}

#endif //ANNUM_SERIES_H
