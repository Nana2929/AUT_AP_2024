#include <fmt/core.h>
// #include <vector>
// #include <string>
// #include <optional>
// #include <type_traits>
#include <random>
#include "algebra.hpp"


template<typename T>
T random(T fmin, T fmax, unsigned int seed=42){
    static_assert(std::is_arithmetic<T>::value, "Template parameter must be an arithmetic type.");

    if constexpr (std::is_integral<T>::value){
        std::uniform_int_distribution<T> dist(fmin, fmax);
        std::default_random_engine re(seed);
        return dist(re);
    }
    else if constexpr(std::is_floating_point<T>::value){
        std::uniform_real_distribution<T> dist(fmin, fmax);
        std::default_random_engine re(seed);
        return dist(re);
    }
}
template<typename T>
algebra::MATRIX<T> algebra::create_matrix(std::size_t rows,
                        std::size_t columns,
                        std::optional<MatrixType> type,
                        std::optional<T> lowerBound,
                        std::optional<T> upperBound){

                            switch (type)
                            {
                                case MatrixType::Zeros:
                                {
                                    MATRIX<T> matrix(rows, std::vector<T>(columns, 0));
                                    return matrix;
                                }
                                case MatrixType::Ones:
                                {
                                    MATRIX<T> matrix(rows, std::vector<T>(columns, 1));
                                    return matrix;
                                }
                                case MatrixType::Identity:
                                {
                                    if (rows != columns){throw std::invalid_argument("Dimension mismatch: rows shall == columns for an Identity matrix.");}
                                    MATRIX<T> matrix(rows, std::vector<T>(columns, 0));
                                    for (int i = 0; i< rows; i++){
                                        matrix[i][i] = 1;
                                    }
                                    return matrix;
                                }
                                case MatrixType::Random:
                                {
                                    MATRIX<T> matrix(rows, std::vector<T>(columns, 0));
                                    for (int i =0; i< rows; i++){
                                        for (int j = 0; j<columns; j++){
                                            matrix[i][j] = random(lowerBound, upperBound);
                                        }
                                    }
                                    return matrix;
                                }
                                default:
                                {
                                    MATRIX<T> matrix(rows, std::vector<T>(columns, 0));
                                    return matrix;
                                }
                        }
                }

template<typename T>
void algebra::display(const algebra::MATRIX<T>& matrix){
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    for (size_t i = 0; i < rows; i++) {
        std::string row;
        for (size_t j = 0; j < cols; j++) {
            if constexpr (std::is_same<T, double>::value) {
                row += fmt::format("|{:7.2f}", matrix[i][j]);
            } else {
                int value = static_cast<int>(matrix[i][j]);
                std::string formatted_value;
                if (value > 9999999 || value < -999999) {
                    formatted_value = std::to_string(value).substr(0,6)+"~";
                } else {
                    std::string value_str = std::to_string(value);
                    int padding = (7 - value_str.length()) / 2;
                    formatted_value = std::string(padding, ' ') + value_str + std::string(7 - padding - value_str.length(), ' ');
                }
                row += fmt::format("|{}", formatted_value);
            }
        }
        fmt::print("{}|\n", row);
    }

}
template<typename T>
algebra::MATRIX<T> algebra::sum_sub(const algebra::MATRIX<T> & matrixA, const algebra::MATRIX<T>& matrixB,
std::optional<std::string> operation){
        size_t rowsA, colsA;
        size_t rowsB, colsB;
        rowsA = matrixA.size(); colsA = matrixA[0].size();
        rowsB = matrixB.size(); colsB = matrixB[0].size();
        if (rowsA != rowsB || colsA != colsB){
            throw std::invalid_argument("Matrix dimensions must match for addition or subtraction.");}
        if (operation != "sum" && operation != "sub"){
            throw std::invalid_argument("Invalid operation. Choose either 'sum' or 'sub'.");
        }
        algebra::MATRIX<T> result(rowsA, std::vector<T>(colsA, 0));
        for (size_t i = 0; i< rowsA; i++){
            for (size_t j = 0; j< colsA; j++){
                if (operation == "sum"){
                    result[i][j] = matrixA[i][j] + matrixB[i][j];
                }
                else{
                    result[i][j] = matrixA[i][j] - matrixB[i][j];
                }
            }
        }
        return result;
}

template<typename T>
algebra::MATRIX<T>algebra::multiply(const algebra::MATRIX<T>& matrix, const T scalar){
    size_t rows, cols;
    rows = matrix.size(); cols = matrix[0].size();
    algebra::MATRIX<T> result(rows, std::vector<T>(cols, 0));
    for (size_t i = 0; i< rows; i++){
        for (size_t j = 0; j< cols; j++){
            result[i][j] = matrix[i][j] * scalar;
        }
    }
    return result;
}
template <typename T>
algebra::MATRIX<T> algebra::multiply(const algebra::MATRIX<T>& matrixA, const algebra::MATRIX<T> & matrixB){
    // first, check if the matrices can be multiplied
    size_t rowsA, colsA;
    size_t rowsB, colsB;
    rowsA = matrixA.size(); colsA = matrixA[0].size();
    rowsB = matrixB.size(); colsB = matrixB[0].size();
    if (colsA != rowsB) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    }
    algebra::MATRIX<T> result(rowsA, std::vector<T>(colsB, 0));
    // O(mnp) complexity for matrixA (m*n) and matrixB (n*p)
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return result;

}

template <typename T>
algebra::


// int main() {
//     algebra::MATRIX<double> matrix = {
//         {1.0, 2.0, 3.0},
//         {4.0, 5.0, 6.0},
//         {7.0, 8.0, 9.0}
//     };
//     algebra::MATRIX<int> intmat = {
//         {12223, 2, 123720123},
//         {4, 53, 6},
//         {7, 8, 9}
//     };

//     algebra::display(matrix);
//     algebra::display(intmat);


//     return 0;
// }
