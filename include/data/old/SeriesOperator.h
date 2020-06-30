//
// Created by anarion on 6/29/20.
//

#ifndef ANNUM_SERIESOPERATOR_H
#define ANNUM_SERIESOPERATOR_H

#include "Series.h"

namespace anarion {

    struct SeriesOperationException : public SeriesException {};

    template<typename num_type>
    class SeriesOperator {
    public:

        /*
         * Operations between Series
         */
#define multiple_operator(character) \
size_type length = (*list[0]).size();\
if (length == 0) { return Series::createByType<num_type>(); }\
for (auto it = list.begin_iterator(); it != list.end_iterator(); ++it) {\
    if ((*it)->size() != length) {\
        throw SeriesLengthNotMatch();\
    }\
}\
Series series {**list.begin_iterator()};\
for (auto resIt = list.begin_iterator() + 1; resIt != list.end_iterator(); ++resIt) {\
    for (size_type index = 0; index < length; ++index) {\
        series.getNumber<num_type>(index) character (*resIt)->getNumber<num_type>(index);\
    }\
}\
return move(series);

        Series sum(const Vector<Series*> &list) {
            multiple_operator(+=)
        }

        Series prod(const Vector<Series*> &list) {
            multiple_operator(*=)
        }

        num_type dot(const Series &x, const Series &y) {
            size_type length = x.size();
            if (y.size() != length) {
                throw SeriesLengthNotMatch();
            }
            num_type res = 0;
            for (size_type index = 0; index < length; ++index) {
                res += x.getNumber<num_type>(index) * y.getNumber<num_type>(index);
            }
            return res;
        }

        Series concat(std::initializer_list<Series> &&list) {
            size_type allLength = 0;
            for (auto &item : list) {
                allLength += item.size();
                if (item.getDtype() != getDtypeByTraints<num_type>()) {
                    throw DtypeNotMatch();
                }
            }
            Series series(getDtypeByTraints<num_type>(), allLength);
            for (auto &item : list) {
                series.insert(series.end_iterator(), item.begin_iterator(), item.size());
            }
            return move(series);
        }
    };
}

#endif //ANNUM_SERIESOPERATOR_H
