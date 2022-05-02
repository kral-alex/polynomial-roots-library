#include <iostream>

#include "library.h"

#define LEN 50
#define BIT_PRECISION 2

int main() {

    std::cout << "Hello, World!\n";
    auto input = static_cast<double *>( malloc(LEN * sizeof(double)));
    srandomdev();
    for (int i = 0; i < LEN; i++) input[i] = (double)(random() & 0b1111111111111) * ((random() & 1) ? 1: -1);
    input[0] = 0.;
    //input[LEN - 1] = 0.;
    //double input[] = {19, 230, 223, -98, 8, -72};
    std::unique_ptr<Array<double>> polynomialRow(preProcess(input, LEN));
    free(input);
    //std::cout << *polynomialRow;

    //auto dummyRoots = std::make_unique<Array<double>>((LEN - 1) * LEN / 2);
    //auto dummyCounts = std::make_unique<Array<int>>(LEN - 1);
    Array<double> dummyRoots = Array<double>((LEN - 1) * LEN / 2);
    Array<int> dummyCounts = Array<int>(LEN - 1);
    findRootsIterate(dummyRoots, dummyCounts, *polynomialRow, LEN, BIT_PRECISION);
    //std::cout << dummyRoots;
    //std::cout << dummyCounts;
    printRoots(dummyRoots, dummyCounts);
    return 0;
}
