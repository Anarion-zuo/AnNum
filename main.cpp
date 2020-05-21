#include <iostream>
#include <container/number_trait.h>
#include <container/Array.hpp>
#include <container/Matrix.hpp>
#include <model/LinearRegression.h>

using namespace std;
using namespace anarion;

int main() {
    Array<float64> a;
    for (float64 x = 0; x < 10000; x += 1) {
        a.push_back(x);
    }
    Matrix<float64> X;
    X.push_back(a);
    Array<float64> y = a * 2.0;
    LinearRegression lr;
    lr.fit(X, y);
    return 0;
}
