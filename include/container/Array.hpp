//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_ARRAY_HPP
#define ANNUM_ARRAY_HPP

#include <numeric_env/NumericConfig.h>
#include <limits>
#include <base/container/List/LinkedList.hpp>

namespace anarion {

    struct true_trait {};
    struct false_trait {};

#define default_numeric_limits_def(num_type) \
constexpr static const size_type size = sizeof(num_type);\
constexpr static const num_type max = std::numeric_limits<num_type>::max();\
constexpr static const num_type min = std::numeric_limits<num_type>::min();\
constexpr static const num_type epsilon = std::numeric_limits<num_type>::epsilon();\
constexpr static const num_type infinity = std::numeric_limits<num_type>::infinity();\
constexpr static const num_type nan = std::numeric_limits<num_type>::quiet_NaN();\

    template <typename num_type>
    struct num_trait {
        using isInteger = false_trait;
        using isPOD = false_type;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(num_type)
    };

#pragma region basic_types
    // integers

    // signed
    template <>
    struct num_trait<char> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(char);
    };
    template <>
    struct num_trait<short> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(short);
    };
    template <>
    struct num_trait<int> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(int);
    };
    template <>
    struct num_trait<long> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(long);
    };
    template <>
    struct num_trait<long long> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(long long );
    };

    // unsigned

    template <>
    struct num_trait<unsigned char> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = true_trait;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned char);
    };
    template <>
    struct num_trait<unsigned short> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = true_trait;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned short);
    };
    template <>
    struct num_trait<unsigned int> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = true_trait;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned int);
    };
    template <>
    struct num_trait<unsigned long> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = true_trait;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned long);
    };
    template <>
    struct num_trait<unsigned long long> {
        using isInteger = true_trait;
        using isPOD = true_trait;
        using isUnsigned = true_trait;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned long long);
    };

    // floating

    template <>
    struct num_trait<float> {
        using isInteger = false_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(float);
    };
    template <>
    struct num_trait<double> {
        using isInteger = false_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(double);
    };
    template <>
    struct num_trait<long double> {
        using isInteger = false_trait;
        using isPOD = true_trait;
        using isUnsigned = false_trait;
        using isMovable = false_type;

        default_numeric_limits_def(long double);
    };
#pragma endregion

    template <typename num_type, size_type segLen = 1024>
    class Array {
    private:

        // copy array
        // considering dst as raw memory space
        void copy_numtype_impl(num_type *dst, num_type *src, size_type length, true_trait) {
            size_type ensize = num_trait<num_type>::size;
            if (ensize == 0) { return; }
            memcpy(dst, src, length * ensize);
        }
        void copy_numtype_impl(num_type *dst, num_type *src, size_type length, false_trait) {
            for (size_type index = 0; index < length; ++index) {
                new (&dst[index]) num_type(src[index]);
            }
        }
        void move_numtype_impl(num_type *dst, num_type *src, size_type length, true_type) {
            for (size_type index = 0; index < length; ++index) {
                new (&dst[index]) num_type(move(src[index]));
            }
        }
        void move_numtype_impl(num_type *dst, num_type *src, size_type length, false_type) {
            copy_numtype(dst, src, length);
        }

        // allocate
        num_type *allocate_nums_impl(size_type nums, true_type) {
            // is pod
            size_type length = nums * num_trait<num_type>::size;
            num_type *ptr = (num_type*)operator new (length);
            memset(ptr, 0, length);
        }
        num_type *allocate_nums_impl(size_type nums, false_type) {
            // is not pod
            size_type length = nums * num_trait<num_type>::size;
            num_type *ptr = (num_type*)operator new (length);
            for (size_type index = 0; index < length; ++index) {
                new (&ptr[index]) num_type;
            }
        }
    protected:
        LinkedList<num_type*> segments;
        using link_iterator = typename LinkedList<num_type*>::iterator;

        void copy_numtype(num_type *dst, num_type *src, size_type length) {
            copy_numtype_impl(dst, src, length, num_trait<num_type>());
        }
        void move_numtype(num_type *dst, num_type *src, size_type length) {
            move_numtype_impl(dst, src, length, num_trait<num_type>::isMovable);
        }
        num_type *allocate_nums(size_type nums) {
            return allocate_nums_impl(nums, num_trait<num_type>::isPOD);
        }

        link_iterator append_new_segment() {
            num_type *arr = allocate_nums(segLen);
            segments.push_back(arr);
            return segments.end_iterator() - 1;
        }

        // mark the end
        link_iterator endIterator;
        size_type endOffset = 0;

    public:
        Array() = default;
        Array(const Array<num_type, segLen> &rhs) : endOffset(rhs.endOffset) {
            // copy all segments

        }

        // sizes
        constexpr size_type capacity() const { return segments.size() * segLen; }
        constexpr size_type size() const { return (segments.size() - 1) * segLen + endOffset; }
        constexpr bool empty() const { return !size(); }

        // structural

        class iterator {
            link_iterator linkIterator;
            size_type offset = 0;
            Array<num_type, segLen> *_this;
        public:
            explicit iterator(link_iterator linkIterator, Array<num_type, segLen> *_this)
                : linkIterator(linkIterator), _this(_this) {}
            iterator(const iterator &rhs)
                : linkIterator(rhs.linkIterator), offset(rhs.offset), _this(rhs._this) {}
            iterator &operator=(const iterator &rhs) {
                if (this == &rhs) { return *this; }
                linkIterator = rhs.linkIterator;
                offset = rhs.offset;
                _this = rhs._this;
                return *this;
            }

            iterator &operator+(size_type steps) {

            }
            iterator &operator-(size_type steps) {

            }
            iterator &operator++() {

            }
            iterator &operator++(int) {

            }
            iterator &operator--() {

            }
            iterator &operator--(int) {

            }
        };
        // iterator support
    protected:
        void iterator_forward(iterator &it, size_type steps) {
            size_type &offset = it.offset;
            link_iterator &linkIterator = it.linkIterator;
            size_type newOffset = offset + steps;
            if (newOffset > segLen) {
                ++linkIterator;
                offset = newOffset - (segLen - offset);
            }
        }
    public:
    };
}

#endif //ANNUM_ARRAY_HPP
