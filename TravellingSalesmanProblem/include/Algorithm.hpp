#ifndef TRAVELLINGSALESMANPROBLEM_ALGORITHM_HPP
#define TRAVELLINGSALESMANPROBLEM_ALGORITHM_HPP

#include <algorithm>
#include <numeric>

template<typename InputIterator, typename OutputIterator, typename Predicate>
void CopyIfOutput(InputIterator first, InputIterator last,
                  OutputIterator result, Predicate predicate) {
    for (; first != last; ++first, ++result) {
        if (predicate(*result)) {
            *result = *first;
        }
    }
}

#endif //TRAVELLINGSALESMANPROBLEM_ALGORITHM_HPP
