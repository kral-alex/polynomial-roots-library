//
// Created by Alex Král on 02.05.2022.
//

#include "gtest/gtest.h"
#include "../roots_library/library.h"

TEST(Rootsvalidity, SetMakesZero) {

}

TEST(Rootsvalidity, RandomMakesZero) {
    const int len = 50;
    const  int bit_precision = 3;
    auto input = Array<double>(len);
    for (int i = 0; i < len; i++) input[i] = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);
    auto polynomialRow = preProcess(input);
    auto dummyRoots = Array<double>((len - 1) * len / 2);
    auto dummyCounts = Array<int>(len - 1);
    findRootsIterate(dummyRoots, dummyCounts, *polynomialRow, len, bit_precision);

    // printRoots(dummyRoots, dummyCounts);
    const auto topPolynomial = polynomialRow->const_slice(- len, 0);
    const auto topRoots = dummyRoots.const_slice(- len + 1, 0);
    const int topCount = dummyCounts[len - 2];

    for (int i = 0; i < topCount; i++) {
        double res = solveForX(topPolynomial->array(), len, (*topRoots)[i]);
        // EXPECT_DOUBLE_EQ(res, 0.);
        EXPECT_EQ(res, 0.);
    }
}