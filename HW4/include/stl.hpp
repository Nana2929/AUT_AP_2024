#ifndef STL_H
#define STL_H

#include <string>
#include <vector>
namespace mylib
{
    void initial_vector_with_3_multiples(std::vector<int> &v, size_t size);
    int count_unique_above(std::vector<int> v, int n);
    std::string vector_to_string(std::vector<int> v, char separator);
    void square_elements(std::vector<int> &v);
    void copy_if_even(const std::vector<int> &source, std::vector<int> &destination);
} // end of namespace `mylib`
#endif // STL_H