//
// Created by 左钰 on 2020/5/21.
//

#ifndef ANNUM_NUMBER_TRAIT_H
#define ANNUM_NUMBER_TRAIT_H

#include <container/base/container_utility.hpp>
#include <limits>

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
constexpr static const num_type zero = 0;

    template<typename num_type>
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
    template<>
    struct num_trait<char> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(char)
    };
    template<>
    struct num_trait<short> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(short)
    };
    template<>
    struct num_trait<int> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(int)
    };
    template<>
    struct num_trait<long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(long)
    };
    template<>
    struct num_trait<long long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(long long)
    };

// unsigned

    template<>
    struct num_trait<unsigned char> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned char)
    };
    template<>
    struct num_trait<unsigned short> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned short)
    };
    template<>
    struct num_trait<unsigned int> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned int)
    };
    template<>
    struct num_trait<unsigned long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned long)
    };
    template<>
    struct num_trait<unsigned long long> {
        using isInteger = true_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(unsigned long long)
    };

// floating

    template<>
    struct num_trait<float> {
        using isInteger = false_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(float)
    };
    template<>
    struct num_trait<double> {
        using isInteger = false_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(double)
    };
    template<>
    struct num_trait<long double> {
        using isInteger = false_type;
        using isPOD = true_type;
        using isUnsigned = false_type;
        using isMovable = false_type;

        default_numeric_limits_def(long double)
    };
#pragma endregion
}


#endif //ANNUM_NUMBER_TRAIT_H
