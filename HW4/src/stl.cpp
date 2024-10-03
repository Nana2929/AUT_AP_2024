

#include "stl.hpp"
#include <algorithm>
#include <numeric> // std::accumulate
#include <set>
#include <string>
#include <vector>

/* Goal: Familiarize with stl functions and get used to lambda expressions */
void mylib::initial_vector_with_3_multiples(std::vector<int> &v, size_t size)
{
    v.resize(size);
    int i = 0;
    std::generate(v.begin(), v.end(),
                  [=]() mutable
                  {i++; return i*3; });
}

int mylib::count_unique_above(std::vector<int> v, int n)
{

    std::set uniques = std::set(v.begin(), v.end());
    // count_if is intended to be used to obtain the count directly, rather than using a side-effect inside the lambda. Since the lambda in your code only modifies unique_num by reference, the count_if return value itself is not used,
    // which causes the compiler error.
    return std::count_if(uniques.begin(), uniques.end(), [=](int x)
                         { return (x > n); });
}
std::string mylib::vector_to_string(std::vector<int> v, char separator)
{
    //   std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    //   EXPECT_EQ(mylib::vector_to_string(v1, ','), "9,8,7,6,5,4,3,2,1")
    auto separator_fold = [&](std::string a, int b)
    {
        return std::move(a) + separator + std::to_string(b);
    };

    std::string rs = std::accumulate(
        std::next(v.rbegin()), v.rend(),
        std::to_string(v.back()),
        separator_fold);
    return rs;
}

void mylib::square_elements(std::vector<int> &v)
{

    // use `std::for_each`
    std::for_each(v.begin(), v.end(), [&](int &x)
                  { x = x * x; });
    // or
    //                range first, range last,  where to store the result, unary op
    // std::transform(v.begin(),    v.end(),     v.begin(),              [](int x) { return x * x});
}
void mylib::copy_if_even(const std::vector<int> &source, std::vector<int> &destination)
{
    // clear the destination for CopyIfEvenTest.HandlesEmptySource
    destination.clear();
    std::copy_if(
        source.begin(), source.end(), std::back_inserter(destination), [&](int x)
        { return (x % 2 == 0); });
}
