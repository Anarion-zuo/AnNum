//
// Created by anarion on 7/1/20.
//

#ifndef ANNUM_ABSTRACTARRAY_H
#define ANNUM_ABSTRACTARRAY_H

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
    using size_type = unsigned long;
    /*
     * This is an interface
     * It pains me to produce this design, when I found I had no choice but to adopt C++ polymorphism if the numeric storage type should be dynamic, i.e. not deterministic at compile time. This interface is to manage the complexity introduced by the different storage type of the data. Arrays could have int or long as integer, and float or double as floating point. By adopting this interface, all integers must look like long, whilst all floating points must look like double. Thus hides the different storage types underneath each array.
     * One may argue that the virtual methods are not as efficient as non-virtual ones, while a numeric library must be efficient by all means. If one should make an attempt such that the storage type of an array is run-time determined, one must adopt some kind of dynamic binding mechanism, allowing the client algorithms handling each type as it seems fit. C++ provides good virtual function mechanism and polymorphism mechanism, better at least than if/else or switch, which must be managed manually by the programmer. Therefore, I must adopt this well-practised software development technique.
     * With the help of this interface, arrays are divided into 2 different classes, integers and floating points. The algorithms must not trouble themselves considering whether this array is stored float or double or even long double, unless their life is at peril. The algorithms could only make requests on the "representation" of the requested number, i.e. integer or floating point. Integers can be converted to floating point numbers almost without a scratch, while floating points must be rounded to become an integer.
     * Nevertheless, it is sometimes necessary to check the actual storage type of the array, therefore the interface has the desired method.
     */
    class AbstractArray {
    public:
        virtual float64 getFloat(size_type index) = 0;
        virtual int64 getInteger(size_type index) = 0;

        virtual bool isInteger() const = 0;
    };
}

#endif //ANNUM_ABSTRACTARRAY_H
