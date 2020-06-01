#include <iostream>
#include <model/LinearRegression.h>
#include <base_data/operators/ArrayOperator.h>

using namespace std;
using namespace anarion;

int main() {
    ArrayInterface *array = new BigFloatArray;
    for (float64 x = 0; x < 10000; x += .001) {
        array->push_back(x);
    }
    ArrayInterface *stdArray = ArrayOperator().standardize(*array);
    Matrix X {stdArray};
    ArrayInterface *y = ArrayOperator().mul(*array, *new IntegerArray(stdArray->length(), 2));
    LinearRegression lr;
    lr.fit(X, *y);
    auto thetas = lr.getThetas();
    float64 theta = thetas.getFloat(0), bias = lr.getBias();
    return 0;
}
