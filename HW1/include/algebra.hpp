#ifndef AUT_AP_2024_Spring_HW1
#define AUT_AP_2024_Spring_HW1

#include <vector>
#include <optional>
#include <string>
#include <type_traits>
#include <random>
namespace algebra{
// Matrix data structure
template<typename T>
using MATRIX = std::vector<std::vector<T>>;

// Matrix initialization types
enum class MatrixType { Zeros, Ones, Identity, Random };

template<typename T>
T random_number(T fmin, T fmax, unsigned int seed=42){
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
void getCofactor(const algebra::MATRIX<T>& A, algebra::MATRIX<T>& temp, std::size_t m, std::size_t n,
    std::size_t N){
    size_t i = 0, j = 0;
    // Looping for each element of the matrix
    for (std::size_t row = 0; row < N; row++) {
        for (std::size_t col = 0; col < N; col++) {
            // Copying into temporary matrix only those
            // element which are not in given row and column
            if (row != m && col != n) {
                temp[i][j++] = A[row][col];
                // Row is filled, so increase row index and
                // reset col index
                if (j == N - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}
template <typename T>
T determinant_rec(const algebra::MATRIX<T>& matrix, std::size_t N) {
    if (N == 1) {
        return matrix[0][0];
    }
    // cofactor matrix
    algebra::MATRIX<T> cofactor(N, std::vector<T>(N, T(0)));

    T det = T(0);
    T sign = T(1);

    // we will do cofactor expansion along the first row (i = 0)
    for (std::size_t c = 0; c < N; c++) {
        // construct the cofactor matrix
        getCofactor(matrix, cofactor, 0, c, N);
        det += sign * matrix[0][c] * determinant_rec(cofactor, N-1);
        sign = -sign;
    }

    return det;
}

template <typename T, typename U>
void adjoint(const algebra::MATRIX<T>& A, algebra::MATRIX<U>& adj)
{
    std::size_t N = A.size();
    if (A.size() == 1) {
        adj[0][0] = 1;
        return;
    }
    // temp is used to store cofactors of A[][]
    int sign = 1;
    algebra::MATRIX<T> temp(N, std::vector<T>(N, 0));

    for (std::size_t i = 0; i < N; i++) {
        for (std::size_t j = 0; j < N; j++) {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign) * (determinant_rec(temp, N - 1));
        }
    }
}
template <typename T>
void transpose_block(algebra::MATRIX<T>& res,
    std::size_t rowStart, std::size_t colStart, std::size_t blockSize,
    const algebra::MATRIX<T>& matrix){
    for (std::size_t i = 0; i< blockSize && rowStart + i < matrix.size(); i++){
        for (std::size_t j = 0; j < blockSize && colStart + j < matrix[0].size(); j++){
            res[colStart + j][rowStart + i] = matrix[rowStart + i][colStart + j];
        }
    }
}

// ============= MAIN API FUNCTIONS =============
template<typename T>
MATRIX<T> create_matrix(std::size_t rows,
                        std::size_t columns,
                        std::optional<MatrixType> type = MatrixType::Zeros,
                        std::optional<T> lowerBound = std::nullopt,
                        std::optional<T> upperBound = std::nullopt);

/**
 * Printing a formatted matrix
 * @param  const MATRIX<T>
 * @return void; prints the matrix to the console
 *         each element has the maximum of 7 char width (including 2 decimal points)
 *        each row is separated by a newline character  '\n'
 *       each element is separated by a '|' character
 */
template<typename T>
void display(const MATRIX<T>& matrix);

template<typename T>
MATRIX<T> sum_sub(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB,
            std::optional<std::string> operation = "sum");

// compile-time polymorphism for `multiply` function
template<typename T>
MATRIX<T> multiply(const MATRIX<T>& matrix, const T scalar);

template<typename T>
MATRIX<T> multiply(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB);

template<typename T>
MATRIX<T> hadamard_product(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB);


template<typename T>
MATRIX<T> transpose(const MATRIX<T>& matrix);

template<typename T>
T trace(const MATRIX<T>& matrix);

template<typename T>
double determinant(const MATRIX<T>& matrix);

template<typename T>
MATRIX<double> inverse(const MATRIX<T>& matrix);

}

#endif //AUT_AP_2024_Spring_HW1
