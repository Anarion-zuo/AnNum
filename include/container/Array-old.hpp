//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_ARRAY_OLD_HPP
#define ANNUM_ARRAY_OLD_HPP

#include <numeric_env/NumericConfig.h>
#include <limits>
#include <base/container/List/Vector.hpp>

namespace anarion {

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

#define default_numeric_limits_def(num_type) \
constexpr static const size_type size = sizeof(num_type);\
constexpr static const num_type max = std::numeric_limits<num_type>::max();\
constexpr static const num_type min = std::numeric_limits<num_type>::min();\
constexpr static const num_type epsilon = std::numeric_limits<num_type>::epsilon();\
constexpr static const num_type infinity = std::numeric_limits<num_type>::infinity();\
constexpr static const num_type nan = std::numeric_limits<num_type>::quiet_NaN();\
constexpr static const num_type zero = 0;\

    template <typename num_type>
    struct num_trait {
        using isInteger = false_type;
        using isPOD = false_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(num_type)
    };

#pragma region basic_types
    // integers

    // signed
    template <>
    struct num_trait<char> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(char);
    };
    template <>
    struct num_trait<short> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(short);
    };
    template <>
    struct num_trait<int> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(int);
    };
    template <>
    struct num_trait<long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(long);
    };
    template <>
    struct num_trait<long long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(long long );
    };

    // unsigned

    template <>
    struct num_trait<unsigned char> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned char);
    };
    template <>
    struct num_trait<unsigned short> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned short);
    };
    template <>
    struct num_trait<unsigned int> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned int);
    };
    template <>
    struct num_trait<unsigned long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned long);
    };
    template <>
    struct num_trait<unsigned long long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned long long);
    };

    // floating

    template <>
    struct num_trait<float> {
        using isInteger = false_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(float);
    };
    template <>
    struct num_trait<double> {
        using isInteger = false_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(double);
    };
    template <>
    struct num_trait<long double> {
        using isInteger = false_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(long double);
    };
#pragma endregion

    struct ArrayException : std::exception {};
    struct ArrayIndexOutOfRange : public ArrayException {
        const char *what() const noexcept override {
            return "Array-old index out of bound";
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
    private:
        // copy array
        // considering dst as raw memory space
        void copy_numtype_impl(num_type *dst, const num_type *src, size_type length, true_type) {
            size_type ensize = num_trait<num_type>::size;
            if (ensize == 0) { return; }
            memcpy(dst, src, length * ensize);
        }
        void copy_numtype_impl(num_type *dst, const num_type *src, size_type length, false_type) {
            for (size_type index = 0; index < length; ++index) {
                new (&dst[index]) num_type(src[index]);
            }
        }
        void move_numtype_impl(num_type *dst, const num_type *src, size_type length, true_type) {
            for (size_type index = 0; index < length; ++index) {
                new (&dst[index]) num_type(move(src[index]));
            }
        }
        void move_numtype_impl(num_type *dst, const num_type *src, size_type length, false_type) {
            copy_numtype(dst, src, length);
        }

        // allocate
        num_type *allocate_nums_impl(size_type nums, true_type) {
            // is pod
            size_type length = nums * num_trait<num_type>::size;
            num_type *ptr = (num_type*)operator new (length);
            memset(ptr, 0, length);
            return ptr;
        }
        num_type *allocate_nums_impl(size_type nums, false_type) {
            // is not pod
            size_type length = nums * num_trait<num_type>::size;
            num_type *ptr = (num_type*)operator new (length);
            for (size_type index = 0; index < nums; ++index) {
                new (&ptr[index]) num_type;
            }
            return ptr;
        }
    protected:
        Vector<num_type*> segments;
        using vec_iterator = typename Vector<num_type*>::iterator;

        constexpr bool indexIsOutOfBound(size_type vecIndex, size_type segIndex) const {
            return !(vecIndex < segments.size() && segIndex < segLength);
        }
        constexpr bool indexIsOutOfBound(size_type index) const {
            size_type vecIndex = index / segLength, segIndex = index % segLength;
            if (segIndex == 0) {
                ++vecIndex;
            }
            return indexIsOutOfBound(vecIndex, segIndex);
        }

        void copy_numtype(num_type *dst, const num_type *src, size_type length) {
            copy_numtype_impl(dst, src, length, typename num_trait<num_type>::isPOD());
        }
        void move_numtype(num_type *dst, const num_type *src, size_type length) {
            move_numtype_impl(dst, src, length, typename num_trait<num_type>::isMovable());
        }
        num_type *allocate_nums(size_type nums) {
            return allocate_nums_impl(nums, typename num_trait<num_type>::isPOD());
        }

        vec_iterator append_new_segment() {
            num_type *arr = allocate_nums(segLength);
            segments.push_back(arr);
            return segments.end_iterator() - 1;
        }

    public:

        // iterator support
        void throwNotMyIterator(const iterator &it) const {
            if (it._this != this) {
                throw ArrayNotMyIterator();
            }
        }
        size_type iteratorIndex(const iterator &it) {
            throwNotMyIterator(it);
            return (it.vecIterator - segments.begin_iterator()) * segLength + it.offset;
        }
    protected:
        constexpr bool indexIsOutOfBound(const iterator &it) const {
            throwNotMyIterator(it);
            return indexIsOutOfBound(it.vecIterator - segments.begin_iterator(), it.offset);
        }

        void iterator_forward(iterator &it, size_type steps) {
            throwNotMyIterator(it);
            while (steps > segLength - it.offset) {
                ++it.vecIterator;
                steps -= segLength - it.offset;
                it.offset = 0;
            }
            it.offset += steps;
        }
        void iterator_backward(iterator &it, size_type steps) {
            throwNotMyIterator(it);
            if (steps > iteratorIndex(it)) {
                throw ArrayIndexOutOfRange();
            }
            while (steps > it.offset) {
                --it.vecIterator;
                steps -= it.offset;
                it.offset = segLength;
            }
            it.offset -= steps;
        }

        // mark the end
        iterator endIterator;

        void init_empty_iterator() {
            endIterator.vecIterator = segments.end_iterator();
            endIterator.offset = 0;
        }
    public:

        iterator begin_iterator() const { return iterator(segments.begin_iterator(), const_cast<Array<num_type, segLength>*>(this)); }
        constexpr const iterator &end_iterator() const { return endIterator; }

        Array() : endIterator(nullptr, this) {
            endIterator.vecIterator = append_new_segment();
        }
        explicit Array(size_type initSize) : endIterator(nullptr, this) {
            size_type segNum = initSize / segLength, segOffset = initSize % segLength;
            if (segOffset != 0) {
                ++segNum;
            }
            endIterator.vecIterator = append_new_segment();
            for (size_type i = 0; i < segNum - 1; ++i) {
                append_new_segment();
            }
        }
        explicit Array(size_type initSize, const num_type &num) : endIterator(nullptr, this) {
            size_type segNum = initSize / segLength, segOffset = initSize % segLength;
            if (segOffset != 0) {
                ++segNum;
            }
            if (segNum == 1) {
                endIterator.vecIterator = append_new_segment();
                endIterator.offset = num;
                return;
            }
            iterator it(nullptr, this);
            for (size_type i = 0; i < segNum - 1; ++i) {
                it.vecIterator = append_new_segment();
                num_type *p = *it.vecIterator;
                for (size_type index = 0; index < segLength; ++index) {
                    p[index] = num;
                }
            }
            endIterator.vecIterator = it.vecIterator;
            endIterator.offset += segOffset;
        }
        explicit Array(const Array &rhs) : endIterator(nullptr, this) {
            endIterator.vecIterator = append_new_segment();
            size_type rhsLength = rhs.size();
            append<segLength>(rhs, rhsLength, 0);
        }
        template<size_type rhsSegLength>
        explicit Array(const Array<num_type, rhsSegLength> &rhs) : endIterator(nullptr, this) {
            endIterator.vecIterator = append_new_segment();
            append<rhsSegLength>(rhs, rhs.size(), 0);
        }
        Array(Array &&rhs) noexcept : endIterator(rhs.endIterator), segments(move(rhs.segments)) {}
        ~Array() { clear(); }
        Array &operator=(const Array &rhs) {
            if (&rhs == this) { return *this; }
            clear();
            append(rhs, rhs.size(), 0);
            return *this;
        }
        Array &operator=(Array &&rhs)  noexcept {
            clear();
            segments = move(rhs.segments);
            endIterator = rhs.endIterator;
            return *this;
        }

        // sizes
        constexpr size_type capacity() const { return segments.size() * segLength; }
        constexpr size_type size() const { return (segments.size() - 1) * segLength + endIterator.offset; }
        constexpr bool empty() const { return !size(); }

        // structural
        void clear() {
            for (size_type index = 0; index < segments.size(); ++index) {
                operator delete (segments[index], segLength);
            }
            segments.clear();
            init_empty_iterator();
            endIterator.vecIterator = append_new_segment();
        }
        void append(const num_type *src, size_type length) {
            size_type endOffset = endIterator.offset;
            auto linkIterator = endIterator.vecIterator;
            while (true) {
                size_type tailLen = segLength - endOffset;
                num_type *begin = *linkIterator + endOffset;
                if (length <= tailLen) {
                    copy_numtype(begin, src, length);
                    endOffset += length;
                    break;
                }
                copy_numtype(begin, src, tailLen);
                src += tailLen;
                length -= tailLen;
                linkIterator = append_new_segment();
                endOffset = 0;
            }
            endIterator.offset = endOffset;
            endIterator.vecIterator = linkIterator;
        }
        void append(const num_type &src) {
            append(&src, 1);
        }
        template<size_type rhsSegLength>
        void append(const Array<num_type, rhsSegLength> &rhs, size_type length, size_type index) {
            if (length > rhs.size() - index) {
                throw ArrayIndexOutOfRange();
            }
            iterator rhsIt = rhs.begin_iterator() + index;
            while (length) {
                size_type vecIndex = rhsIt.vecIterator - rhs.segments.begin_iterator();
                size_type segIndex = rhsIt.offset;

                size_type moreRhs = rhsSegLength - segIndex, moreThis = segLength - endIterator.offset;
                num_type *begin = *endIterator.vecIterator + endIterator.offset;

                if (moreThis < moreRhs) {
                    if (length < moreThis) {
                        copy_numtype(begin, *rhsIt.vecIterator + rhsIt.offset, length);
                        endIterator.offset += length;
                        rhsIt.offset += length;
                        break;
                    }
                    copy_numtype(begin, *rhsIt.vecIterator + rhsIt.offset, moreThis);
                    endIterator.vecIterator = append_new_segment();
                    endIterator.offset = 0;
                    rhsIt.offset += moreThis;
                    length -= moreThis;
                } else {
                    if (length < moreRhs) {
                        copy_numtype(begin, *rhsIt.vecIterator + rhsIt.offset, length);
                        endIterator.offset += length;
                        rhsIt.offset += length;
                        break;
                    }
                    copy_numtype(begin, *rhsIt.vecIterator + rhsIt.offset, moreRhs);
                    endIterator.offset += moreRhs;
                    ++rhsIt.vecIterator;
                    rhsIt.offset = 0;
                    length -= moreRhs;
                }
            }
        }

        // index
        num_type &operator[](size_type index) {
            size_type vecIndex = index / segLength, segIndex = index % segLength;
            if (segIndex == 0 && vecIndex != 0) {
                ++vecIndex;
            }
            if (indexIsOutOfBound(vecIndex, segIndex)) {
                throw ArrayIndexOutOfRange();
            }
            return segments[vecIndex][segIndex];
        }

        // math
        num_type sum() {
            sum(0, size());
        }
        num_type sum(size_type beginIndex, size_type endIndex) {
            num_type ret = num_trait<num_type>::zero;
            auto beginIt = begin_iterator() + beginIndex;
            auto endIt = begin_iterator() + endIndex;
            for (auto it = beginIt; it != endIt; ++it) {
                ret += *it;
            }
            return move(ret);
        }
        num_type mean() {
            return mean(0, size());
        }
        num_type mean(size_type beginIndex, size_type endIndex) {
            num_type ret = num_trait<num_type>::zero;
            size_type index = 0;
            auto beginIt = begin_iterator() + beginIndex;
            auto endIt = begin_iterator() + endIndex;
            for (auto it = beginIt; it != endIt; ++it) {
                ret = (ret * index + *it) / (index + 1);
                ++index;
            }
            return move(ret);
        }

        template<size_type rhsSegLength>
        Array operator+(const Array<num_type, rhsSegLength> &rhs) {
            if (size() != rhs.size()) { throw ArrayOperationShapeDifferent(); }
            Array ret;
            iterator thisIt = begin_iterator(), rhsIt = rhs.begin_iterator();
            for (size_type i = 0; i < size(); ++i) {
                ret.append(*thisIt + *rhsIt);
                ++thisIt;
                ++rhsIt;
            }
            return move(ret);
        }
        template<size_type rhsSegLength>
        Array operator-(const Array<num_type, rhsSegLength> &rhs) {
            if (size() != rhs.size()) { throw ArrayOperationShapeDifferent(); }
            Array ret;
            iterator thisIt = begin_iterator(), rhsIt = rhs.begin_iterator();
            for (size_type i = 0; i < size(); ++i) {
                ret.append(*thisIt - *rhsIt);
                ++thisIt;
                ++rhsIt;
            }
            return move(ret);
        }
        template<size_type rhsSegLength>
        Array operator*(const Array<num_type, rhsSegLength> &rhs) {
            if (size() != rhs.size()) { throw ArrayOperationShapeDifferent(); }
            Array ret;
            iterator thisIt = begin_iterator(), rhsIt = rhs.begin_iterator();
            for (size_type i = 0; i < size(); ++i) {
                ret.append(*thisIt * *rhsIt);
                ++thisIt;
                ++rhsIt;
            }
            return move(ret);
        }
        template<size_type rhsSegLength>
        Array operator/(const Array<num_type, rhsSegLength> &rhs) {
            if (size() != rhs.size()) { throw ArrayOperationShapeDifferent(); }
            Array ret;
            iterator thisIt = begin_iterator(), rhsIt = rhs.begin_iterator();
            for (size_type i = 0; i < size(); ++i) {
                ret.append(*thisIt / *rhsIt);
                ++thisIt;
                ++rhsIt;
            }
            return move(ret);
        }
        Array operator-() {
            Array ret;
            iterator thisIt = begin_iterator();
            for (size_type i = 0; i < size(); ++i) {
                ret.append(-*thisIt);
                ++thisIt;
            }
            return move(ret);
        }

        static Array dot(const Array<num_type> &a1, const Array<num_type> &a2) {

        }

    };

    template<typename num_type, size_type segLength>
    struct num_trait<Array<num_type, segLength>> {
        using this_type = Array<num_type, segLength>;
        using isInteger = false_type;
        using isPOD = false_type;
        using isUnsigned = false_type;
        using isMovable = true_type;

        constexpr static const size_type size = sizeof(this_type);
    };
}

#endif //ANNUM_ARRAY_OLD_HPP
