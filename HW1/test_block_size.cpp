#include <iostream>
#include <vector>

using MATRIX = std::vector<std::vector<int>>;

// Function to transpose a block of the matrix
void transposeBlock(const MATRIX& matrix, MATRIX& transposed, int rowStart, int colStart, int blockSize) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = rowStart; i < std::min(rowStart + blockSize, rows); ++i) {
        for (int j = colStart; j < std::min(colStart + blockSize, cols); ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
}

MATRIX transposeMatrixBlockWise(const MATRIX& matrix, int blockSize) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    MATRIX transposed(cols, std::vector<int>(rows));

    for (int i = 0; i < rows; i += blockSize) {
        for (int j = 0; j < cols; j += blockSize) {
            transposeBlock(matrix, transposed, i, j, blockSize);
        }
    }

    return transposed;
}

int main() {
    MATRIX matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16},
        {17, 18, 19, 20}
    };

    int blockSize = 4;
    MATRIX transposedMatrix = transposeMatrixBlockWise(matrix, blockSize);

    // Print the transposed matrix
    for (const auto& row : transposedMatrix) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
