#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Alex Král on 02.05.2022.
//

#include "gtest/gtest.h"
#include "RootsTest.h"

namespace tests {

    constexpr double ACCEPTABLE_DELTA = 10e-10;

    union dbl_bits {
        double fl;
        u_int64_t bits;
    };

    TEST_P(RootsTest, BitDifference) {
        for (double root: roots) {
            dbl_bits root_bits {.fl = root};
            dbl_bits less {.bits =  root_bits.bits - (u_int64_t)(bit_precision + 1)};
            dbl_bits more {.bits =  root_bits.bits + (u_int64_t)(bit_precision + 1)};
            //std::cout << root_bits.fl << " : " << root_bits.bits << "\n";
            //std::cout << less.bits << " : " << more.bits << "\n";
            //std::cout << less.fl << " : " << more.fl << "\n";
            double on_root = solveForX(polynomial.data(), (int) polynomial.size(), root);
            double less_res = solveForX(polynomial.data(), (int) polynomial.size(), less.fl);
            double more_res = solveForX(polynomial.data(), (int) polynomial.size(), more.fl);

            std::cout << less_res << " | " << on_root << " | " << more_res << "\n";

            if (signbit(on_root) == signbit(less_res)) {
                EXPECT_LE(abs(on_root), abs(less_res));
            }
            if (signbit(on_root) == signbit(more_res)) {
                EXPECT_LE(abs(on_root), abs(more_res));
            } else {
                EXPECT_LE(abs(on_root), abs(less_res));
                EXPECT_LE(abs(on_root), abs(less_res));
            }
        }
    }

    TEST_P(RootsTest, CloseToZero) {
        std::cout << std::defaultfloat << std::setprecision(8);
        printPolynomial(std::cout, polynomial.data(), (int)polynomial.size());
        std::cout << std::scientific;

        std::cout << "#roots: " << roots.size() << "\n";
        for (double root: roots) {
            double res = solveForX(polynomial.data(), (int) polynomial.size(), root);
            std::cout << "\t" << root << ": " << res << "\n";
            // EXPECT_DOUBLE_EQ(res, 0.);
            EXPECT_NEAR(res, 0., ACCEPTABLE_DELTA * pow(2, polynomial.size()));
        }
    }

    std::string nameGeneratorFromSize(const testing::TestParamInfo<RootsTest::ParamType> info) {
        std::stringstream ss;
        ss << "len_" << std::get<0>(info.param).size() << "_bp_" << std::get<1>(info.param) << "";
        return ss.str();
    }

    auto sizes = std::vector<int>{3, 10, 50, 100};
    auto settings = std::vector<std::vector<double>>(sizes.size());
    auto seed_time = time(nullptr);

    auto valuesSuite = testing::Combine(
            testing::Values(
                    *randomPolyGenerator(sizes[1], seed_time * sizes.size()),
                    *randomPolyGenerator(sizes[2], seed_time * sizes.size()),
                    *randomPolyGenerator(sizes[3], seed_time * sizes.size()),
                    std::vector<double>{1, -1, 1}
            ),
            testing::Values(0)
    );

    INSTANTIATE_TEST_SUITE_P(RootsValidation, RootsTest, valuesSuite, nameGeneratorFromSize);
}
#pragma clang diagnostic pop
