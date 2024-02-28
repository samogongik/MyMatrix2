#include<cassert>
#include "MyMatrix.hpp"



int main() {
    size_t size_x;
    std::cin >> size_x;
    assert(std::cin.good());

    MyMatrix<double> matrix(size_x, size_x);
    MyMatrix<double> matrix2(size_x, size_x);

    matrix = matrix2;

    for (size_t i = 0; i < size_x; i++) {
        for (size_t j = 0; j < size_x; j++) {
            std::cin >> matrix[i][j];
            assert(std::cin.good());
        }
    }

    double det = matrix.determinant(matrix);
    std::cout << det << std::endl;
}
