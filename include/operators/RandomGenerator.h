//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_RANDOMGENERATOR_H
#define ANNUM_RANDOMGENERATOR_H

#include <cstdlib>

namespace anarion {
    class RandomGenerator {
    protected:
        using seed_type = unsigned int;

        seed_type seed = 1;
        void setSysSeed();
        long getSysRandom();

    public:
        RandomGenerator() = default;
        RandomGenerator(const RandomGenerator &rhs) = default;
        explicit RandomGenerator(seed_type seed) : seed(seed) {}
        virtual ~RandomGenerator() = default;

        void setSeed(seed_type newSeed);
        template <typename int_type>
        int_type getInt(int_type lower, int_type upper) {
            int_type result = int_type(getSysRandom());
            result %= upper - lower;
            result += lower;
            return result;
        }

        template <typename float_type>
        float_type getFloat(float_type lower, float_type upper) {
            float_type result = float_type(getSysRandom());
            result /= upper - lower;
            result += lower;
            return result;
        }
    };
}

#endif //ANNUM_RANDOMGENERATOR_H
