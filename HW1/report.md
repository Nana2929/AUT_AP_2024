# HW1: Algebra Library 
## `constexpr`
```cpp
#include <iostream>
#include <random>
#include <type_traits>

// Template function for generating random numbers
template<typename T>
T random(T fmin, T fmax) {
    static_assert(std::is_arithmetic<T>::value, "Template parameter must be an arithmetic type.");

    std::random_device rd;
    std::default_random_engine re(rd());

    if constexpr (std::is_integral<T>::value) {
        std::uniform_int_distribution<T> dist(fmin, fmax);
        return dist(re);
    } else if constexpr (std::is_floating_point<T>::value) {
        std::uniform_real_distribution<T> dist(fmin, fmax);
        return dist(re);
    }
}
```
`if constexpr`: The if constexpr statement is evaluated at compile time. If the condition is true, only the corresponding branch is compiled. If the condition is false, the branch is discarded and not compiled at all.

## `#include<format>` error
https://stackoverflow.com/questions/69037873/why-am-i-missing-c20-headers-and-how-do-i-fix-this
https://stackoverflow.com/questions/71122029/missing-c20-headers-on-ubuntu-20-04-with-clang-13

https://fmt.dev/latest/get-started/#debianubuntu
Currently use `fmt` instead.
It looks like since g++13 has full support of `std::format`.
You need to do this to link fmt properly...
```
g++ -std=c++20 -o out {yourfile.cpp} -lfmt
```

## `using`
Advantages of `using`
- Type Safety: compiler 了解且會幫忙檢查
- Templates：可以直接和 template 合用，彈性大
- Readability：閱讀起來的感覺更像宣告（declaration），比 `typedef` 或 preprocessor macros `#define` 都還容易理解
```cpp=
template<typename T>
using MATRIX = std::vector<std::vector<T>>;
```
`typedef` cannot directly alias template types in the same way `using` can. For example, creating a type alias for a template class using typedef requires additional workarounds.
```cpp=
// no template
typedef std::vector<std::vector<int>> MATRIX

// with template
template<typename T>
struct MatrixAlias {
    typedef std::vector<std::vector<T>> type;
};
```

## `size_t`
[StackOverflow](https://stackoverflow.com/questions/994288/size-t-vs-int-in-c-and-or-c)
In general, `size_t` should be used whenever you are measuring the size of something. It is really strange that `size_t` is only required to represent between 0 and `SIZE_MAX` bytes and `SIZE_MAX` is only required to be 65,535...
- the return type of `sizeof()` is `size_t` and it is an `unsigned integer`
operator `new()`` takes the number of bytes to allocate as a size_t parameter
- `size_t` is defined in ``<cstddef>``
`= SIZE_MAX` is defined in ``<limits.h>`` in C99 but not mentioned in C++98?!
-`size_t` is not included in the list of fundamental integer types so I have always assumed that `size_t` is a type alias for one of the fundamental types: char, short int, int, and long int.

## `transpose`
Where in **Pitfalls** it asks us to handle Memory Usage...
Memory Usage: Transposing a large matrix may require significant memory for the new matrix, especially if the original matrix is large. Consider the memory implications of your implementation.
- in-place transpose: 作業要求我們不能變更輸入的矩陣
```cpp=
template <typename T>
void in_place_transpose(algebra::MATRIX<T>& square_matrix){
    size_t n = square_matrix.size();
    for (size_t i = 0; i<n;i++){
        for (size_t j = i+1; j<n; j++){
            std::swap(square_matrix[i][j], square_matrix[j][i]);
        }
    }
}
```
- block-wise processing: gpt-4 建議

## `determinant`
![](https://static.wixstatic.com/media/e90ace_bf60886c77dd414abfd2e50fe6a65195~mv2.png/v1/fill/w_538,h_278,al_c,q_85,usm_0.66_1.00_0.01,enc_auto/e90ace_bf60886c77dd414abfd2e50fe6a65195~mv2.png)
