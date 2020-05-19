//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_NUMERICCONTEXT_H
#define ANNUM_NUMERICCONTEXT_H

#include "NumericConfig.h"

namespace anarion {
    class NumericContext {
    protected:
        NumericConfig *config = nullptr;
    public:
        NumericContext() = default;
        NumericContext(NumericConfig *config);
        ~NumericContext();

        constexpr NumericConfig &getConfig() { return *config; }
    };
}

#endif //ANNUM_NUMERICCONTEXT_H
