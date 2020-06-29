//
// Created by anarion on 6/29/20.
//

#ifndef ANNUM_SERIESOPERATOR_H
#define ANNUM_SERIESOPERATOR_H

#include "Series.h"
#include <cmath>

namespace anarion {

    struct SeriesOperationException : public SeriesException {};
    struct SeriesLengthNotMatch : public SeriesOperationException {};

    template<typename num_type>
    class SeriesOperator {
    public:
/*
 * Operations between Series
 */
#define multiple_operator(character) \
size_type length = list.begin()->size();\
if (length == 0) { return Series::createByType<num_type>(); }\
for (auto &item : list) {\
    if (item.size() != length) {\
        throw SeriesLengthNotMatch();\
    }\
}\
Series series {*list.begin()};\
for (auto resIt = list.begin() + 1; resIt != list.end(); ++resIt) {\
    for (size_type index = 0; index < length; ++index) {\
        series.getNumber<num_type>(index) character resIt->getNumber<num_type>(index);\
    }\
}\
return move(series);

        Series sum(std::initializer_list<Series> &&list) {
            multiple_operator(+=)
        }

        Series prod(std::initializer_list<Series> &&list) {
            multiple_operator(*=)
        }

        /*
         * Operations between Series and numbers
         */
#define number_operator(character) \
        for (size_type index = 0; index < series.size(); ++index) {\
            series.getNumber<num_type>(index) character number;\
        }

        void add(Series &series, const num_type &number) {
            number_operator(+=)
        }
        void sub(Series &series, const num_type &number) {
            number_operator(-=)
        }
        void mul(Series &series, const num_type &number) {
            number_operator(*=)
        }
        void div(Series &series, const num_type &number) {
            number_operator(/=)
        }

        /*
         * Operations inside Series
         */

        num_type sum(const Series &series) {
            size_type length = series.size();
            num_type res { 0 };
            for (size_type index = 0; index < length; ++index) {
                res += series.getNumber<num_type>(index);
            }
            return res;
        }

        num_type prod(const Series &series) {
            size_type length = series.size();
            num_type res { 1 };
            for (size_type index = 0; index < length; ++index) {
                res *= series.getNumber<num_type>(index);
            }
            return res;
        }

        num_type mean(const Series &series) {
            return sum(series) / series.size();
        }

        float64 variance(const Series &series) {
            num_type meanVal = mean(series);
            float64 var = 0;
            for (size_type index = 0; index < series.size(); ++index) {
                num_type diff = series.getNumber<num_type>(index) - meanVal;
                var += diff * diff;
            }
            return var / (series.size() - 1);
        }

        float64 stdVariance(const Series &series) {
            return sqrt(variance(series));
        }

        void standardize(Series &series) {
            num_type meanVal = mean(series);
            float64 var = 0;
            for (size_type index = 0; index < series.size(); ++index) {
                num_type &ref = series.getNumber<num_type>(index);
                num_type diff = ref - meanVal;
                var += diff * diff;
                ref = diff;
            }
            var /= (series.size() - 1);
            var = sqrt(var);
            div(series, var);
        }
    };
}

#endif //ANNUM_SERIESOPERATOR_H
