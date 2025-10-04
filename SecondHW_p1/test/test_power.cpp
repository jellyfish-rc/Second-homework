#include <iostream>
#include <cassert>
#include "math/MathFunctions.h"

int main(int argc, char *argv[])
{
    std::cout << "Testing power function..." << std::endl;

    // 测试用例
    assert(power(2, 3) == 8);
    assert(power(5, 0) == 1);
    assert(power(3, 4) == 81);
    assert(power(10, 2) == 100);

    std::cout << "All power function tests passed!" << std::endl;
    return 0;
}
