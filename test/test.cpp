//Generic main test runner.

#include <stdexcept>
#include <iostream>

#include "gtest/gtest.h"

int main(int ac, char **av)
{
    int result;

    ::testing::InitGoogleTest(&ac, av);

    //Run tests.
    result = RUN_ALL_TESTS();

    return result;
}
