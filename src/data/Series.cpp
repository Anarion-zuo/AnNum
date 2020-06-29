//
// Created by anarion on 6/29/20.
//

#include <data/Series.h>

uint8 anarion::Series::computeNumberLengthByDtype() {
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

void anarion::Series::removeNumber(anarion::size_type index) {
    if (index >= size()) {
        throw IndexOutOfRange();
    }
    this->remove(this->begin + index * numberLength, numberLength);
}

bool anarion::Series::isDtypeInteger(anarion::Dtype dtype) {
    switch (dtype) {
        case Dtype::Int32:
        case Dtype::Int64:
            return true;
        default:
            return false;
    }
}

anarion::Series::Series(anarion::Dtype dtype, anarion::size_type initSize)
: parent_type(initSize), dtype(dtype), numberLength(computeNumberLengthByDtype()) {

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
