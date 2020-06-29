#include <iostream>
#include <model/BatchLinearRegression.h>
#include <data/Series.h>

using namespace std;
using namespace anarion;

int main() {

    Series series = Series::createByType<float32>();
    series.insertNumber(0, 1.5f);
    float32 &number = series.getNumber<float32>(0);
    number = 2.4;
    float32 nn = series.getNumber<float32>(0);

    return 0;
}
