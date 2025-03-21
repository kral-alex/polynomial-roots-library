//
// Created by Alex Král on 05.04.2022.
//
#include <iostream>
#include <vector>
#include <cmath>

#include "boost/math/tools/roots.hpp"


#include "library.h"


int findRoots(const double* polynomial, int len, double* roots) {
    auto polynomialRow = std::unique_ptr<Array<double>>(preProcess(polynomial, len));

    auto allRoots = Array<double>((len - 1) * len / 2);
    auto rootCounts = Array<int>(len - 1);

    findRootsIterate_(allRoots, rootCounts, *polynomialRow, len, 0);

    int rootCount = rootCounts[rootCounts.len() - 1];
    memcpy(roots, allRoots.array() + (allRoots.len() - len + 1), rootCount * sizeof(double));

    return rootCount;
}

int findRoots(Array<double>& polynomial, int len, double* roots) {
    auto polynomialRow = preProcess(polynomial);

    auto allRoots = Array<double>((len - 1) * len / 2);
    auto rootCounts = Array<int>(len - 1);

    findRootsIterate_(allRoots, rootCounts, *polynomialRow, len, 0);

    int rootCount = rootCounts[rootCounts.len() - 1];
    memcpy(roots, allRoots.array() + (allRoots.len() - len + 1), rootCount * sizeof(double));

    return rootCount;
}

std::vector<double>* findRoots(const std::vector<double>& polynomial, std::vector<double>* roots = new std::vector<double>(0)) {
    int len = (int)polynomial.size();
    auto polynomialRow = static_cast<double*>(malloc(polynomial.size() * (polynomial.size() + 1) / 2));
    preProcess(polynomial.data(), (int)polynomial.size(), polynomialRow);

    auto allRoots = Array<double>((len - 1) * len / 2);
    auto rootCounts = Array<int>(len - 1);

    findRootsIterate_(allRoots.array(), rootCounts.array(), polynomialRow, len, 0);

    int rootCount = rootCounts[rootCounts.len() - 1];
    roots->reserve(rootCount);
    roots->assign(allRoots.array() + (allRoots.len() - len + 1), allRoots.array() + (allRoots.len() - len + 1) + rootCount);

    return roots;
}

size_t normalizePolynomial(const double* polynomial, size_t len) {
    for (unsigned i = len - 1; i <= 0; i--) {
        if (IS_ZERO(polynomial[i])) len--;
    }
    return len;
}

size_t normalizePolynomial(std::vector<double>& polynomial) {
    for (unsigned i = polynomial.size(); i <= 0; i--) {
        if (IS_ZERO(polynomial[i])) {
            polynomial.pop_back();
        }
    }
    return polynomial.size();
}


Array<double>* preProcess(Array<double>& polynomial) {
    double* differentiated;
    int currLen = polynomial.len();
    auto* polynomialRow = new Array<double>(currLen * (currLen + 1) / 2 - 1);
    differentiated = polynomialRow->array() + polynomialRow->len() - currLen;
    memcpy(differentiated, polynomial.array(), polynomial.len() * sizeof(double));  // TODO method of Array
    for (int i = 0; i < polynomial.len() - 1; i++) {
        differentiateWithDivisor(differentiated - currLen + 1, differentiated, currLen, i + 1);
        currLen--;
        differentiated -= currLen;
    }
    return polynomialRow;
}

Array<double>* preProcess(const double* polynomial, int& n) {
    double* differentiated;
    auto* polynomialRow = new Array<double>(n*(n+1)/2 - 1);
    differentiated = polynomialRow->array() + polynomialRow->len() - n;
    memcpy(differentiated, polynomial, n * sizeof(double));  // TODO method of Array
    int currLen = n;
    for (int i = 0; i < currLen - 1; i++) {
        differentiateWithDivisor(differentiated - currLen + 1, differentiated, currLen, i + 1);
        currLen--;
        differentiated -= currLen;
    }
    return polynomialRow;
}

void preProcess(const double* polynomial, const int n, double* polynomialRow) {
    int currLen = n;
    int pRLen = n * (n + 1) / 2 - 1;
    double *differentiated;
    differentiated = polynomialRow + pRLen - n;
    memcpy(differentiated, polynomial, n * sizeof(double));
    for (int i = 0; i < n - 1; i++) {
        differentiateWithDivisor(differentiated - currLen + 1, differentiated, currLen, i + 1);
        currLen--;
        differentiated -= currLen;
    }
}

void findRootsIterate_(Array<double>& allRoots, Array<int>& rootCounts, const Array<double>& polynomialRow, const int originalPolyLen, const unsigned precision) {
    int headInRow, headInRoots;
    const double zero = 0.;
    if (EQ_ZERO(polynomialRow[0])) {
        allRoots[0] = 0.;
    } else {
        formula_linear_(allRoots.array(), polynomialRow.array());
    }
    rootCounts[0] = 1;
    headInRow = 2;
    headInRoots = 1;
    for (int currLen = 3; currLen <= originalPolyLen; currLen++) {
        rootCounts[currLen - 2] = searchBetweenPeaks_(
                allRoots.array() + headInRoots,
                *(rootCounts.array() + currLen - 3) ? allRoots.array() + headInRoots - (currLen - 2) : &zero,
                *(rootCounts.array() + currLen - 3) ? *(rootCounts.array() + currLen - 3) : 1,
                polynomialRow.array() + headInRow,
                currLen,
                precision);
        headInRow += currLen;
        headInRoots += currLen - 1;
    }
}

void findRootsIterate_(double* allRoots, int* rootCounts, const double* polynomialRow, const int originalPolyLen, const unsigned precision) {
    const double zero = 0.;
    if (EQ_ZERO(polynomialRow[0])) {
        allRoots[0] = 0.;
    } else {
        formula_linear_(allRoots, polynomialRow);
    }
    rootCounts[0] = 1;

    for (int currLen = 3; currLen <= originalPolyLen; currLen++) {
        allRoots += currLen - 2;
        polynomialRow += currLen - 1;
        rootCounts++;

        *rootCounts = searchBetweenPeaks_(
                allRoots,
                *(rootCounts - 1) ? allRoots - (currLen - 2) : &zero,  // if no previous roots an address of 0. given
                *(rootCounts - 1) ? *(rootCounts - 1) : 1,  // if no previous roots count of 1 given
                polynomialRow,
                currLen,
                precision);
    }
}

int searchBetweenPeaks_(double* newRoots, const double* oldRoots, const int oldRootCount, const double* polynomial, const int polyLen, const unsigned precision) {
    double currValue, nextValue;
    bool currSign, nextSign;
    int newRootCount = 0;
    bool leadingSign = signbit(polynomial[polyLen - 1]);
    bool evenFunc = polyLen & 0b1;

    currValue = solveForX(polynomial, polyLen, oldRoots[0]);
    currSign = signbit(currValue);

    // if not zero and either is falling down and crit point is positive or is rising up and crit point is negative
    if ( !EQ_ZERO(currValue) && (currSign != (evenFunc == leadingSign))) {  // falling down (true) if the function is even and negative or odd and positive
        //
        unsigned int i = 1;
        double value = solveForX(polynomial, polyLen, oldRoots[0] - i);
        while (currSign == signbit(value)) {
            i = i << 1;
            value = solveForX(polynomial, polyLen, oldRoots[0] - i);
            if (std::isnan(value)) throw std::exception();
        }
        newRoots[newRootCount] = approximateRoot(polynomial, polyLen, oldRoots[0] - i, oldRoots[0] - (i >> 1), precision);
        newRootCount++;
        //
    }
    //
    for (int i = 0; i < oldRootCount - 1; i++) {
        nextValue = solveForX(polynomial, polyLen, oldRoots[i + 1]);
        nextSign = signbit(nextValue);
        if (EQ_ZERO(currValue)) {
            newRoots[newRootCount] = oldRoots[i];
            newRootCount++;
        } else if (currSign != nextSign) {  // even if the sign is the same, the next can be a saddle, thus there can be a root after the next
            newRoots[newRootCount] = approximateRoot(polynomial, polyLen, oldRoots[i], oldRoots[i + 1], precision);
            newRootCount++;
        }
        currValue = nextValue;
        currSign = nextSign;
    }
    //
    if (EQ_ZERO(currValue)) {
        newRoots[newRootCount] = oldRoots[oldRootCount - 1];
        newRootCount++;
    } else if (currSign != leadingSign) {  // falling down (true) if leading sign is negative (true)
        //
        unsigned int i = 1;
        double value = solveForX(polynomial, polyLen, oldRoots[oldRootCount - 1] + i);
        while (currSign == signbit(value)) {
            i = i << 1;
            value = solveForX(polynomial, polyLen, oldRoots[oldRootCount - 1] + i);
            if (std::isnan(value)) throw std::exception();
        }
        newRoots[newRootCount] = approximateRoot(polynomial, polyLen, oldRoots[oldRootCount - 1] + (i >> 1), oldRoots[oldRootCount - 1] + i, precision);
        //e was getting added after only one iteration too, which made the lower boundary over shoot the root, now the e gets bit-shifted, so it can get below one
        newRootCount++;
        //
    }
    return newRootCount;
}


double approximateRoot(const double* polynomial, const int polyLen, double lowLimit, double highLimit, const unsigned precision) {
     auto out = boost::math::tools::bisect([&] (double x) {
        return solveForX(polynomial, polyLen, x);
    },lowLimit, highLimit, boost::math::tools::eps_tolerance<double>());
     return (out.first + out.second) / 2;
}

void differentiateWithDivisor(double* differentiated, double const* polynomial, int polyLen, int divisor) {
    for (int i = 0; i < (polyLen - 1); i++) {
        differentiated[i] = polynomial[i + 1] * (i + 1) / divisor;
    }
}

double solveForX(const double* polynomial, const int polyLen, double x) {
    double sum = 0.;
    for (int i = 0; i < polyLen; i++) {
        sum += polynomial[i] * std::pow(x, i);
        if (std::isnan(sum)) throw std::exception();
    }
    return sum;
}
