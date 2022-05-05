#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Alex Král on 02.05.2022.
//

#include "gtest/gtest.h"
#include "RootsTest.h"

#include "../roots_library/library.h"

constexpr double ACCEPTABLE_DELTA = 10e-8;

std::unique_ptr<std::vector<double>> randomPolyGenerator(int size, unsigned seed=time(nullptr)) {
    srandom(seed);

    auto polynomial = std::make_unique<std::vector<double>>(size);
    std::for_each(polynomial->begin(), polynomial->end(),[](double& e) {
        e = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);
    });
    return polynomial;
}

TEST_P(RootsTest, CloseToZero) {
    for (double root : roots) {
        double res = solveForX(polynomial.data(), (int)polynomial.size(), root);
        std::cout << root << ": " << res << "\n";
        // EXPECT_DOUBLE_EQ(res, 0.);
        EXPECT_NEAR(res, 0., ACCEPTABLE_DELTA);
    }
}


TEST(RootsValidity, RandomMakesZero) {
    int len = 50;
    const unsigned bit_precision = 3;
    auto input = Array<double>(len);

    srandom(time(nullptr));

    for (int i = 0; i < len; i++) input[i] = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);

    printPolynomial(std::cout, input.array(), input.len());

    //TODO see if two below are same (ideally in another test)
    //auto polynomialRow = std::unique_ptr<Array<double>>(preProcess(input));
    auto polynomialRow = std::unique_ptr<Array<double>>(preProcess(input.array(), len));

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
        EXPECT_NEAR(res, 0., ACCEPTABLE_DELTA);
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
    int rootCount = findRoots(input.array(), len, output.array(), bit_precision);

    ASSERT_NE(len % 2, rootCount % 2);

    std::cout << std::scientific << std::setprecision(17);
    std::cout << "# roots: "<< rootCount << "\n";

    for (int i = 0; i < rootCount; i++) {
        double res = solveForX(input.array(), len, output[i]);
        std::cout << output[i] << ": " << res << "\n";
        // EXPECT_DOUBLE_EQ(res, 0.);
        EXPECT_NEAR(res, 0., ACCEPTABLE_DELTA);
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
        EXPECT_NEAR(res, 0., ACCEPTABLE_DELTA);
    }
}

auto sizes = std::vector<int>{3, 10, 50, 100};
auto seed_time = time(nullptr);

INSTANTIATE_TEST_SUITE_P(RootsValidation, RootsTest,
        testing::Values(std::tuple<std::tuple<std::vector<double>, int>>(std::for_each(sizes.begin(), sizes.end(), [](int n) { return randomPolyGenerator(n, seed_time); } ), )
        );


#pragma clang diagnostic pop
