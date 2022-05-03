//
// Created by Alex Král on 05.04.2022.
//

#ifndef ROOTS___CORE_H
#define ROOTS___CORE_H

#include <cmath>

#include "Array.h"

#if __DBL_MANT_DIG__ != 53
#error "Requires IEEE 754 floating point!"
#endif

typedef struct ArrayLen {
    int count;
    int len;
    double* array;
} ArrayLen;


/* two problems I know of for equalsPrecise:
 *  when the difference overflows further into the mantissa
 *  when the (even one bit) difference overflows into a one bit difference in the exponent
 */
inline bool equalsPrecise(double a, double b, unsigned long sig_dig_precision) {
    if (!(*reinterpret_cast<u_int64_t *>(&a) & (0b0111111111111111111111111111111111111111111111111111111111111110) ||
          *reinterpret_cast<u_int64_t *>(&b) & (0b0111111111111111111111111111111111111111111111111111111111111110))) { // 0b0111111111111111111111111111111111111111111111111111111111111110 == (ULONG_LONG_MAX >> 1) - 1
        return true;
    }
    if ((*reinterpret_cast<u_int64_t*>(&a) >> 63) != (*reinterpret_cast<u_int64_t*>(&b) >> 63)) { //maybe useless, if signs don't equal minus operation does not work as expected?
        return false;
    }
    //return (*reinterpret_cast<u_int64_t*>(&b) - *reinterpret_cast<u_int64_t*>(&a)) < ((unsigned long long)1 << sig_dig_precision) ||
    //        (*reinterpret_cast<u_int64_t*>(&a) - *reinterpret_cast<u_int64_t*>(&b)) < ((unsigned long long)1 << sig_dig_precision);

    u_int64_t c = (*reinterpret_cast<u_int64_t*>(&b) - *reinterpret_cast<u_int64_t*>(&a));
    return !(c >> (sig_dig_precision + 1) && ~c >> sig_dig_precision); // exact with zero tolerance -> && (sig_dig_precision && ~c >> (sig_dig_precision -1)))
    // TODO compare with bit shift epsilon
}

#define EPSILON (__DBL_MIN__ * 64)
#define EQ_ZERO(x) ((x) < EPSILON && (x) > -(EPSILON))
#define IS_ZERO(x) ((x) == 0. || (x) == -0.)


Array<double>* preProcess(const double*, int);
Array<double>* preProcess(Array<double>&);

void findRootsIterate(Array<double>&, Array<int>&, const Array<double>&, int, unsigned long);

int findRoots(double*, const double*, int, const double*, int, unsigned long);

double approximateRoot(const double*, int, double, double, unsigned long);

void differentiateWithDivisor(double*, double const*, int, int);

double solveForX(const double*, int, double);

double solveForX(const ArrayLen, double);

inline void formula_linear_(double* root, double const* poly) {
    if (poly[1] == 0.) throw std::exception();
    *root = -poly[0] / poly[1];
}

template<class T>
void printRoots(const Array<T>& roots, const Array<int>& counts) {
    std::cout << "printing roots:\n";
    int currPos = 0;
    for (int i = 0; i < counts.len(); i++) {
        int count = counts[i];
        std::cout << "level: " << i + 1 << " , count: " << count << '\n';
        for (int j = 0; j < count; j++) {
            std::cout << "    " << j << ": " << roots[currPos + j] << '\n';
        }
        currPos += i + 1;
    }
}


#endif //ROOTS___CORE_H
