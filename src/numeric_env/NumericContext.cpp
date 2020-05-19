//
// Created by 左钰 on 2020/5/20.
//

#include "numeric_env/NumericContext.h"

anarion::NumericContext::NumericContext(anarion::NumericConfig *config)
    : config(config)
{

}

anarion::NumericContext::~NumericContext() {
    delete config;
}
