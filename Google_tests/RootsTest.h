//
// Created by Alex Král on 05.05.2022.
//

#ifndef ROOTSLIB_ROOTSTEST_H
#define ROOTSLIB_ROOTSTEST_H

#include "../roots_library/library.h"

std::unique_ptr<std::vector<double>> polyGenerator1(int size);

class RootsTest :
public testing::TestWithParam<std::tuple<std::vector<double>, int>> {
protected:
    std::vector<double> polynomial = std::get<0>(GetParam());
    std::vector<double> roots;
    int bit_precision = std::max(4, std::get<1>(GetParam()));

    void SetUp() override {
        normalizePolynomial(polynomial);
        auto temp_roots = static_cast<double*>(malloc((polynomial.size() - 1) * sizeof(double)));
        int rootCount = findRoots(polynomial.data(), (int)polynomial.size(), temp_roots);
        roots = std::vector<double>(temp_roots, temp_roots + rootCount);

        ASSERT_NE(polynomial.size() % 2, rootCount % 2);
    }
};


inline std::unique_ptr<std::vector<double>> randomPolyGenerator(int size, unsigned seed=time(nullptr)) {
    srandom(seed);

    auto polynomial = std::make_unique<std::vector<double>>(size);
    std::for_each(polynomial->begin(), polynomial->end(),[](double& e) {
        e = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);
    });
    return polynomial;
}

#endif //ROOTSLIB_ROOTSTEST_H
