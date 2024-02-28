#include <random>
#include "MyMatrix.hpp"

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "Copy tests" << std::endl;

    for (int i = 1; i < 5; i++) {
        std::uniform_int_distribution<int> distribution_int(1, 5);
        std::uniform_real_distribution<double> distribution_real(-20.0, 20.0);
        int x1 = distribution_int(gen);
        int x2 = distribution_int(gen);

        MyMatrix<double> matrix1(3, 3);
        MyMatrix<double> matrix2(2, 2);

        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                matrix1[i][j] = distribution_real(gen);
            }
        }
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                matrix2[i][j] = distribution_real(gen);
            }
        }

        matrix1 = matrix2;
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                std::cout << matrix2[i][j] << " ";
            }
            std:: cout << std::endl;
        }

        matrix1 = matrix2;
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                std::cout << matrix2[i][j] << " ";
            }
            std:: cout << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << "Test for intenger numbers:" << std::endl;
    std::uniform_int_distribution<int> distribution_int(-10, 10);

    for (size_t kol = 1; kol < 100; kol++){
        MyMatrix<int> matrix1(100,100);
        double det_correct1 = 1.0;

        for (size_t j = 0; j < 99; j++){
            for (size_t k = 0; k < 99; k++ ){
                if (j == k) {
                    matrix1[j][k] = distribution_int(gen);
                    det_correct1 = det_correct1 * matrix1[j][k];
                }
                else matrix1[j][k] = 0;
            }
        }

        for (size_t i = 0; i < 99; i++) {
            for(size_t j = 0; i < 99; j++) {
                matrix1[i + 1][j] = matrix1[i + 1][j] + matrix1[i][j];
            }
        }

        for (size_t i = 0; i < 99; i--){
            for (size_t j = 0; j < 99; j++) {
                matrix1[i][j] = matrix1[i][j] + matrix1[99][j] * 2;
            }
        }

        double det1 = matrix1.determinant(matrix1) ;
        if (det_correct1 == 0){
            if (det1 == 0){
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            }
            else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct1 << "Result of the program =" << " "
                          << det1 << std::endl;

            }
        }
        else {
            double result = det1 / det_correct1;
            if ((result - 1) < 0.000001) {
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            } else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct1 << "Result of the program =" << " "
                          << det1 << std::endl;

            }
        }

    }

    std::cout << std::endl;

    std::cout << "Test for real numbers:" << std::endl;
    std::uniform_real_distribution<double> distribution_real(-20.0, 20.0);

    for (size_t kol = 1; kol <= 100; kol++){
        MyMatrix<double> matrix2(100,100);
        double det_correct2 = 1.0;

        for (size_t j = 0; j < 100; j++){
            for (size_t k = 0; k < 100; k++ ){
                if (j == k) {
                    matrix2[j][k] = distribution_real(gen);
                    det_correct2 = det_correct2 * matrix2[j][k];
                }
                else matrix2[j][k] = 0.0;
            }
        }

        for (size_t i = 0; i < 99; i++){
            for (size_t j = 0; j < 99; j++) {
                matrix2[i + 1][j] = matrix2[i + 1][j] + matrix2[i][j];
            }
        }

        for (size_t i = 0; i < 99; i--){
            for (size_t j = 0; j < 99; j++) {
                matrix2[i][j] = matrix2[i][j] + matrix2[99][j] * 2;
            }
        }

        double det2 = matrix2.determinant(matrix2) ;
        if (det_correct2 == 0){
            if (det2 == 0){
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            }
            else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct2 << "Result of the program =" << " "
                          << det2 << std::endl;

            }
        }
        else {
            double result = det2 / det_correct2;
            if ((result - 1) < 0.000001) {
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            } else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct2 << "Result of the program =" << " "
                          << det2 << std::endl;

            }
        }
    }
}