#include <gtest/gtest.h>

#include <iostream>

#include "message.hpp"
#include "server.hpp"
#include "stl.hpp"
#include "user.hpp"

int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        std::vector<int> v;
        mylib::initial_vector_with_3_multiples(v, 3);
        for (int x : v)
        {
            std::cout << x << std::endl;
        }
    }

    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}