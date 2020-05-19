//
// Created by 左钰 on 2020/5/20.
//

#ifndef ANNUM_NUMERICCOMPONENT_H
#define ANNUM_NUMERICCOMPONENT_H

#include "NumericContext.h"

namespace anarion {
    class NumericComponent {
    protected:
        NumericContext *context;
    public:
        explicit NumericComponent(NumericContext *context) : context(context) {}
        virtual ~NumericComponent() = default;

        constexpr NumericContext &getContext() { return *context; }
    };
}

#endif //ANNUM_NUMERICCOMPONENT_H
