//
// Created by anarion on 6/29/20.
//

#include <data/Series.h>

uint8 anarion::Series::computeNumberLengthByDtype(Dtype dtype) {
    switch (dtype) {
        case Dtype::Int32:
        case Dtype::Float32:
            return 4;
        case Dtype::Int64:
        case Dtype::Float64:
            return 8;
            break;
        default:
            throw InvalidDtype();
    }
}

void anarion::Series::setNumberImpl(anarion::size_type index, const void *number) {
    if (index >= size()) {
        throw IndexOutOfRange();
    }
    char *dst = this->begin + index * numberLength;
    memcpy(dst, number, numberLength);
}

void anarion::Series::insertNumberImpl(anarion::size_type index, const void *number) {
    if (index > size()) {
        throw IndexOutOfRange();
    }
    this->insert(this->begin + index * numberLength, (char*)number, numberLength);
}

void anarion::Series::getNumber(anarion::size_type index, void *out) const {
    size_type trueIndex = index * numberLength;
    if (trueIndex > cur - begin) {
        throw IndexOutOfRange();
    }
    memcpy(out, begin + index * numberLength, numberLength);
}

void anarion::Series::removeNumber(anarion::size_type index) {
    if (index >= size()) {
        throw IndexOutOfRange();
    }
    this->remove(this->begin + index * numberLength, numberLength);
}

bool anarion::Series::isDtypeInteger(anarion::Dtype dtype) {
    switch (dtype) {
        case Dtype::Int8:
        case Dtype::Int16:
        case Dtype::Int32:
        case Dtype::Int64:
            return true;
        default:
            return false;
    }
}

anarion::Series::Series(anarion::Dtype dtype, anarion::size_type initSize)
: parent_type(), dtype(dtype), numberLength(computeNumberLengthByDtype(dtype)) {
    resize(numberLength * initSize);
}

anarion::Series::Series(const anarion::Series &rhs)
: parent_type(rhs), dtype(rhs.dtype), numberLength(rhs.numberLength) {

}

anarion::Series::Series(anarion::Series &&rhs) noexcept
: parent_type(forward<Series>(rhs)), dtype(rhs.dtype), numberLength(rhs.numberLength) {

}

anarion::Series &anarion::Series::operator=(const anarion::Series &rhs) {
    if (&rhs == this) { return *this; }
    parent_type::operator=(rhs);
    dtype = rhs.dtype;
    numberLength = rhs.numberLength;
    return *this;
}

anarion::Series &anarion::Series::operator=(anarion::Series &&rhs) noexcept {
    parent_type::operator=(forward<Series>(rhs));
    dtype = rhs.dtype;
    numberLength = rhs.numberLength;
    return *this;
}

anarion::Series anarion::Series::cloneType(size_type initSize) const {
    return Series(dtype, initSize);
}

#define call_series_template_dtype(func_name) \
switch (dtype) {\
    case Dtype::Int8:\
        return func_name<int8>();\
    case Dtype::Int16:\
        return func_name<int16>();\
    case Dtype::Int32:\
        return func_name<int32>();\
    case Dtype::Int64:\
        return func_name<int64>();\
    case Dtype::Float32:\
        return func_name<float32>();\
    case Dtype::Float64:\
        return func_name<float64>();\
    default:\
        throw InvalidDtype();\
}

float64 anarion::Series::variance() const {
    call_series_template_dtype(varianceImpl)
}

float64 anarion::Series::sum() const {
    call_series_template_dtype(sumImpl)
}

float64 anarion::Series::mean() const {
    call_series_template_dtype(sumImpl)
}

float64 anarion::Series::prod() const {
    call_series_template_dtype(prodImpl)
}
#define call_series_template_dtype_rhs(func_name) \
switch (rhs.dtype) {\
    case Dtype::Int8:\
        func_name<int8>(rhs);\
    case Dtype::Int16:\
        func_name<int16>(rhs);\
    case Dtype::Int32:\
        func_name<int32>(rhs);\
    case Dtype::Int64:\
        func_name<int64>(rhs);\
    case Dtype::Float32:\
        func_name<float32>(rhs);\
    case Dtype::Float64:\
        func_name<float64>(rhs);\
    default:\
        throw InvalidDtype();\
}

void anarion::Series::add(const anarion::Series &rhs) {
    call_series_template_dtype_rhs(addImpl)
}

void anarion::Series::sub(const anarion::Series &rhs) {
    call_series_template_dtype_rhs(subImpl)
}

void anarion::Series::mul(const anarion::Series &rhs) {
    call_series_template_dtype_rhs(mulImpl)
}

void anarion::Series::div(const anarion::Series &rhs) {
    call_series_template_dtype_rhs(divImpl)
}

float64 anarion::Series::dot(const anarion::Series &rhs) {

}

anarion::Series anarion::Series::convertType(anarion::Dtype dt) const {
    switch (dt) {
        case Dtype::Int8:
            return convertTypeImpl<int8>();
        case Dtype::Int16:
            return convertTypeImpl<int16>();
        case Dtype::Int32:
            return convertTypeImpl<int32>();
        case Dtype::Int64:
            return convertTypeImpl<int64>();
        case Dtype::Float32:
            return convertTypeImpl<float32>();
        case Dtype::Float64:
            return convertTypeImpl<float64>();
        default:
            throw InvalidDtype();
    }
}

