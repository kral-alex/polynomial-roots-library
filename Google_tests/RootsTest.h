//
// Created by Alex Král on 05.05.2022.
//

#ifndef ROOTSLIB_ROOTSTEST_H
#define ROOTSLIB_ROOTSTEST_H

#include "../roots_library/library.h"

class RootsTest :
    public testing::TestWithParam<std::vector<double>> {
protected:
    RootsTest() {

    }

};

std::unique_ptr<std::vector<double>> polyGenerator1(int size) {
    auto polynomial = std::make_unique<std::vector<double>>(size);

    std::for_each(polynomial->begin(), polynomial->end(),[](double& e) {
        e = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);
    });
    return polynomial;
}

#endif //ROOTSLIB_ROOTSTEST_H
