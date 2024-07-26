#include <fmt/core.h>
// #include <vector>
// #include <string>
// #include <optional>
#include "algebra.hpp"


template<typename T>
algebra::MATRIX<T> algebra::create_matrix(std::size_t rows,
                        std::size_t columns,
                        std::optional<MatrixType> type,
                        std::optional<T> lowerBound,
                        std::optional<T> upperBound){
                        MatrixType actualType = type.value_or(MatrixType::Zeros);
                        if (rows == 0 || columns == 0){
                            // throw an error if the dimensions are invalid
                            throw std::invalid_argument("Invalid matrix dimensions.");}
                        switch (actualType)
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
                                for (std::size_t i = 0; i< rows; i++){
                                    matrix[i][i] = 1;
                                }
                                return matrix;
                            }
                            case MatrixType::Random:
                            {
                                // check lowerBound < upperBound
                                // check lowerBound is valid
                                if (!lowerBound.has_value()){
                                    throw std::invalid_argument("lowerBound must be a valid number.");
                                }
                                // check upperBound is valid
                                if (!upperBound.has_value()){
                                    throw std::invalid_argument("upperBound must be a valid number.");
                                }
                                if (lowerBound.has_value() && upperBound.has_value() && lowerBound.value() >= upperBound.value()){
                                    throw std::invalid_argument("lowerBound must be less than upperBound.");
                                }
                                MATRIX<T> matrix(rows, std::vector<T>(columns, 0));
                                for (std::size_t i =0; i< rows; i++){
                                    for (std::size_t j = 0; j<columns; j++){
                                        matrix[i][j] = algebra::random_number<T>(lowerBound.value_or(0), upperBound.value_or(1));
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
    std::size_t rows = matrix.size();
    std::size_t cols = matrix[0].size();

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
        std::size_t rowsA, colsA;
        std::size_t rowsB, colsB;
        if (matrixA.empty() && matrixB.empty()){
            return {};
        }
        rowsA = matrixA.size(); colsA = matrixA[0].size();
        rowsB = matrixB.size(); colsB = matrixB[0].size();
        if (rowsA != rowsB || colsA != colsB){
            throw std::invalid_argument("Matrix dimensions must match for addition or subtraction.");}
        if (operation != "sum" && operation != "sub"){
            throw std::invalid_argument("Invalid operation. Choose either 'sum' or 'sub'.");
        }
        algebra::MATRIX<T> result(rowsA, std::vector<T>(colsA, 0));
        for (std::size_t i = 0; i< rowsA; i++){
            for (std::size_t j = 0; j< colsA; j++){
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
    std::size_t rows, cols;
    rows = matrix.size(); cols = matrix[0].size();
    algebra::MATRIX<T> result(rows, std::vector<T>(cols, 0));
    for (std::size_t i = 0; i< rows; i++){
        for (std::size_t j = 0; j< cols; j++){
            result[i][j] = matrix[i][j] * scalar;
        }
    }
    return result;
}
template <typename T>
algebra::MATRIX<T> algebra::multiply(const algebra::MATRIX<T>& matrixA, const algebra::MATRIX<T> & matrixB){
    // first, check if the matrices can be multiplied

    std::size_t rowsA, colsA;
    std::size_t rowsB, colsB;
    rowsA = matrixA.size(); colsA = matrixA[0].size();
    rowsB = matrixB.size(); colsB = matrixB[0].size();
    if (colsA != rowsB) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    }
    algebra::MATRIX<T> result(rowsA, std::vector<T>(colsB, 0));
    // O(mnp) complexity for matrixA (m*n) and matrixB (n*p)
    for (std::size_t i = 0; i < rowsA; i++) {
        for (std::size_t j = 0; j < colsB; j++) {
            for (std::size_t k = 0; k < colsA; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return result;

}

template <typename T>
algebra::MATRIX<T> algebra::hadamard_product(const algebra::MATRIX<T>& matrixA, const algebra::MATRIX<T>& matrixB){
    // ensure that the matrices have the same dimensions
    std::size_t rowsA, colsA;
    std::size_t rowsB, colsB;
    rowsA = matrixA.size(); colsA = matrixA[0].size();
    rowsB = matrixB.size(); colsB = matrixB[0].size();
    if (rowsA != colsA || rowsB != colsB){
        throw std::invalid_argument("Matrix dimensions must match for Hadamard product.");
    }
    algebra::MATRIX<T> result(rowsA, std::vector<T>(colsA, 0));
    for (std::size_t i = 0; i< rowsA; i++){
        for (std::size_t j = 0; j< colsA; j++){
            result[i][j] = matrixA[i][j] * matrixB[i][j];
        }
    }
    return result;

}

template <typename T>
algebra::MATRIX<T> algebra::transpose(const algebra::MATRIX<T>& matrix){
    // Block-wise transpose (credit: GPT-4)

    if (matrix.empty()){
        return {};
    }
    std::size_t rows, cols;
    std::size_t approximateCacheSize = 32 * 1024; // 32 KB, L1 Cache size
    std::size_t elementSize = sizeof(int);
    std::size_t elementsPerBlock = approximateCacheSize / elementSize;
    std::size_t blockSize = std::sqrt(elementsPerBlock); // Assuming square blocks for simplicity

    // Ensure blockSize is at least 1
    blockSize = std::max((std::size_t)1, blockSize);

    rows = matrix.size(); cols = matrix[0].size();
    algebra::MATRIX<T> result(cols, std::vector<T>(rows, 0));
    for (std::size_t rowStart = 0; rowStart < rows; rowStart++){
        for (std::size_t colStart = 0; colStart < cols; colStart++){
            algebra::transpose_block(result, rowStart, colStart, blockSize, matrix);
        }
    }
    return result;
}



template <typename T>
T algebra::trace(const algebra::MATRIX<T>& matrix){
    T t = T(0);
    if (matrix.size() != matrix[0].size()){
        throw std::invalid_argument("Matrix must be square for trace calculation.");
    }
    for (std::size_t i = 0; i< matrix.size(); i++){
        t += matrix[i][i];
    }
    return t;

}


template <typename T>
double algebra::determinant(const MATRIX<T>& matrix){
    std::size_t rows, cols;
    rows = matrix.size(); cols = matrix[0].size();
    if (rows != cols){
            throw std::invalid_argument("Matrix must be square for determinant calculation.");
        }
    return algebra::determinant_rec(matrix, rows);
}

template <typename T>
algebra::MATRIX<double> algebra::inverse(const algebra::MATRIX<T>& matrix){

    double det = algebra::determinant(matrix);
    if (det == 0){
        throw std::invalid_argument("Determinant of the matrix is 0. Inverse does not exist.");
    }
    std::size_t N = matrix.size();
    algebra::MATRIX<double> inv(N, std::vector<double>(N, 0));
    algebra::MATRIX<double> adj(N, std::vector<double>(N, 0));
    algebra::adjoint(matrix, adj);
    for (std::size_t i = 0; i < N; i++){
        for (std::size_t j = 0; j < N; j++){
            inv[i][j] = adj[i][j] / det;
        }
    }
    return inv;
}

/* Explicit instantiation:
If you want to keep the implementation in the .cpp file, you can explicitly instantiate the templates for the types you're using. At the end of your .cpp file, add:
cpp */
// create_matrix
template algebra::MATRIX<int> algebra::create_matrix<int>(std::size_t, std::size_t, std::optional<MatrixType>, std::optional<int>, std::optional<int>);
template algebra::MATRIX<double> algebra::create_matrix<double>(std::size_t, std::size_t, std::optional<MatrixType>, std::optional<double>, std::optional<double>);
template algebra::MATRIX<float> algebra::create_matrix<float>(std::size_t, std::size_t, std::optional<MatrixType>, std::optional<float>, std::optional<float>);
// display
template void algebra::display<int>(const algebra::MATRIX<int>&);
template void algebra::display<double>(const algebra::MATRIX<double>&);

// sum_sub
template algebra::MATRIX<int> algebra::sum_sub<int>(const algebra::MATRIX<int>&, const algebra::MATRIX<int>&, std::optional<std::string>);
template algebra::MATRIX<double> algebra::sum_sub<double>(const algebra::MATRIX<double>&, const algebra::MATRIX<double>&, std::optional<std::string>);

// multiply
template algebra::MATRIX<int> algebra::multiply<int>(const algebra::MATRIX<int>&, const int);
template algebra::MATRIX<double> algebra::multiply<double>(const algebra::MATRIX<double>&, const double);
template algebra::MATRIX<int> algebra::multiply<int>(const algebra::MATRIX<int>&, const algebra::MATRIX<int>&);
template algebra::MATRIX<double> algebra::multiply<double>(const algebra::MATRIX<double>&, const algebra::MATRIX<double>&);

// hadamard_product
template algebra::MATRIX<int> algebra::hadamard_product<int>(const algebra::MATRIX<int>&, const algebra::MATRIX<int>&);
template algebra::MATRIX<double> algebra::hadamard_product<double>(const algebra::MATRIX<double>&, const algebra::MATRIX<double>&);

// transpose
template algebra::MATRIX<int> algebra::transpose<int>(const algebra::MATRIX<int>&);
template algebra::MATRIX<double> algebra::transpose<double>(const algebra::MATRIX<double>&);

// trace
template int algebra::trace<int>(const algebra::MATRIX<int>&);
template double algebra::trace<double>(const algebra::MATRIX<double>&);

// determinant
template double algebra::determinant<int>(const algebra::MATRIX<int>&);
template double algebra::determinant<double>(const algebra::MATRIX<double>&);

// inverse
template algebra::MATRIX<double> algebra::inverse<int>(const algebra::MATRIX<int>&);
template algebra::MATRIX<double> algebra::inverse<double>(const algebra::MATRIX<double>&);


/*
recursive function to calculate the determinant of a square matrix
Suppose the input matrix is of size n x n
Note that because we reuse the submatrix of the same size, each size we only create one submatrix
SIZE |  submatrix size
n    |  (n-1) * (n-1)
n-1  |  (n-2) * (n-2)
...
2    |  1 * 1
1    |  0
SC: O(n^3) (0^2 + 1^2 + ... + (n-1)^2 = n(n-1)(2n-1)/6 = O(n^3))
reference: https://sdjee2015.wixsite.com/whyandhow/single-post/2017/01/22/determinant-of-a-matrix-using-recursion
*/







