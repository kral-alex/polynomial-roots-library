#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Alex Král on 02.05.2022.
//

#include "gtest/gtest.h"
#include "../roots_library/library.h"

TEST(HelloTest, TestPrintHello){
    testing::internal::CaptureStdout();
    hello();
    EXPECT_EQ("Hello, World!\n", testing::internal::GetCapturedStdout());
}


#pragma clang diagnostic pop