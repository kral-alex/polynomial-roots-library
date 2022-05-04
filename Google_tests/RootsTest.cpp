//
// Created by Alex Král on 02.05.2022.
//

#include "gtest/gtest.h"
#include "../roots_library/library.h"

TEST(RootsValidity, SetMakesZero) {

}

TEST(RootsValidity, RandomMakesZero) {
    int len = 50;
    const unsigned bit_precision = 3;
    auto input = Array<double>(len);

    srandom(time(nullptr));

    for (int i = 0; i < len; i++) input[i] = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);
    auto polynomialRow = std::unique_ptr<Array<double>>(preProcess(input));
    //auto polynomialRow = *preProcess(input);
    auto dummyRoots = Array<double>((len - 1) * len / 2);
    auto dummyCounts = Array<int>(len - 1);
    findRootsIterate_(dummyRoots, dummyCounts, *polynomialRow, len, bit_precision);

    // printRoots(dummyRoots, dummyCounts);
    const auto topPolynomial = polynomialRow->const_slice(- len, 0);
    const auto topRoots = dummyRoots.const_slice(- len + 1, 0);
    const int topCount = dummyCounts[len - 2];

    ASSERT_NE(len % 2, topCount % 2);

    std::cout << std::scientific << std::setprecision(17);
    std::cout << "#roots: "<< topCount << "\n";

    for (int i = 0; i < topCount; i++) {
        double res = solveForX(topPolynomial->array(), len, (double)(*topRoots)[i]);
        std::cout << (*topRoots)[i] << ": " << res << "\n";
        // EXPECT_DOUBLE_EQ(res, 0.);
        EXPECT_NE((double)(*topRoots)[i], 0.);
        EXPECT_NEAR(res, 0., 10e-32);
    }
}

TEST(RootsValidity, RandomRootCheck) {
    const int len = 50;
    const unsigned bit_precision = 3;
    auto input = Array<double>(len);
    auto output = Array<double>(len - 1);

    srandom(time(nullptr));

    for (int i = 0; i < len; i++) input[i] = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);

    //int rootCount = findRoots(input.array(), len, output.array(), bit_precision);
    int rootCount = findRoots(input, len, output.array(), bit_precision);

    ASSERT_NE(len % 2, rootCount % 2);

    std::cout << std::scientific << std::setprecision(17);
    std::cout << "# roots: "<< rootCount << "\n";

    for (int i = 0; i < rootCount; i++) {
        double res = solveForX(input.array(), len, output[i]);
        std::cout << output[i] << ": " << res << "\n";
        // EXPECT_DOUBLE_EQ(res, 0.);
        EXPECT_NEAR(res, 0., 10e-32);
    }
}

TEST(RootsValidity, GivenRootCheck) {
    const int len = 7;
    const unsigned bit_precision = 3;
    const double input[] = {1, 2, 3, 4, 5, 1000, 0.01};
    auto output = Array<double>(len - 1);

    int rootCount = findRoots(input, len, output.array(), bit_precision);

    ASSERT_NE(len % 2, rootCount % 2);

    std::cout << std::scientific << std::setprecision(17);
    std::cout << "#roots: "<< rootCount << "\n";

    for (int i = 0; i < rootCount; i++) {
        double res = solveForX(input, len, output[i]);
        std::cout << output[i] << ": " << res << "\n";
        // EXPECT_DOUBLE_EQ(res, 0.);
        EXPECT_NEAR(res, 0., 10e-32);
    }
}