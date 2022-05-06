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
    //std::vector<double> polynomial;
    std::vector<double> polynomial = std::get<0>(GetParam());
    std::vector<double> roots;
    int bit_precision = std::get<1>(GetParam());

    void SetUp() override {
        std::cout << std::defaultfloat << std::setprecision(8);
        printPolynomial(std::cout, polynomial.data(), (int)polynomial.size());
        std::cout << std::scientific;
        auto temp_roots = static_cast<double*>(malloc((polynomial.size() - 1) * sizeof(double)));
        int rootCount = findRoots(polynomial.data(), (int)polynomial.size(), temp_roots, bit_precision);
        roots = std::vector<double>(temp_roots, temp_roots + rootCount);

        ASSERT_NE(polynomial.size() % 2, rootCount % 2);
    }

};


#endif //ROOTSLIB_ROOTSTEST_H
