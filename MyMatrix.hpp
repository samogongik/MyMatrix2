#include <iostream>
#include <cmath>

template<typename T>
concept numeric_type = std::copy_constructible<T> &&
                       std::copyable<T> && requires(T num, std::size_t n){
    { num + num } -> std::convertible_to<T>;
    { num - num } -> std::convertible_to<T>;
    { num * num } -> std::convertible_to<T>;
    { num / num } -> std::convertible_to<T>;

    { num != num } -> std::convertible_to<bool>;
    { num == num } -> std::convertible_to<bool>;

    { delete new(T) };
    { T(0) };
    { T(1) };
};
template <numeric_type T>  void construct(T* p, T& rhs){
    new(p) T(rhs);
}
template <numeric_type T> void construct(T* p, T&& rhs){
    new(p) T(std::move(rhs));
}

template <numeric_type T> void destroy(T* p){
    p->~T();
}

template <typename Iter> void destroy(Iter first, Iter last){
    while (first != last) {
        destroy(&*first++);
    }
}

template <numeric_type T> class MyMatrixBuff{
protected:
    T* arr;
    size_t m = 0;
    size_t n = 0;
    size_t used = 0;

protected:
    MyMatrixBuff (const MyMatrixBuff& rhs) = delete;
    MyMatrixBuff&operator= (const MyMatrixBuff& rhs) = delete;

    MyMatrixBuff(MyMatrixBuff&& rhs)noexcept
            : arr(rhs.arr), m(rhs.m), n(rhs.n), used(rhs.used){
        rhs.arr = nullptr;
        rhs.m = 0;
        rhs.n = 0;
        rhs.used = 0;
    }

    MyMatrixBuff&operator= (MyMatrixBuff&& rhs) noexcept{
        std::swap(arr, rhs.arr);
        std::swap(m, rhs.m);
        std::swap(n, rhs.n);
        std::swap(used, rhs.used);
        return *this;
    }

    MyMatrixBuff(size_t rows = 0, size_t columns = 0)
            : arr((rows == 0 || columns == 0) ? nullptr
                                              : static_cast<T *>(::operator new(sizeof(T) * rows * columns))),
              m(rows), n(columns) {}

    ~MyMatrixBuff(){
        destroy(arr, arr+used);
        ::operator delete (arr);
    }
};

template <numeric_type T> class MyMatrix final : private MyMatrixBuff<T> {
public:
    using MyMatrixBuff<T>::arr;
    using MyMatrixBuff<T>::used;
    using MyMatrixBuff<T>::m;
    using MyMatrixBuff<T>::n;

    MyMatrix(size_t rows = 0, size_t columns = 0) : MyMatrixBuff<T>(rows, columns){}

    MyMatrix(MyMatrix&& rhs) = default;
    MyMatrix&operator=(MyMatrix&& rhs) = default;

    MyMatrix(const MyMatrix &rhs) : MyMatrixBuff<T>(rhs.used) {
        while (used < rhs.used) {
            construct(arr + used, rhs.arr[used]);
            used += 1;
        }
    }

    MyMatrix &operator=(const MyMatrix &rhs) {
        MyMatrix tmp(rhs);
        std::swap(*this, tmp);
        return *this;
    }

    struct ProxyRow {
        T *row;
        const T& operator[](size_t index) const { return row[index]; }
        T& operator[](size_t index) { return row[index]; }
    };

    ProxyRow operator[](size_t index) {
        return {arr + index * n};
    }
    ProxyRow operator[](size_t index) const{
        return {arr + index * n};
    }

    size_t count_rows() const{
        return m;
    }

    size_t count_columns() const{
        return n;
    }

    MyMatrix operator+(const MyMatrix &rhs) const {
        if (rhs.count_rows() != m || rhs.count_columns() != n) {
            throw std::runtime_error("Attempt to add matrices of different sizes");
        }

        MyMatrix result(m, n);
        for (size_t i = 0; i < m; i++) {
            for(size_t j = 0; j < n; j++){
                result[i][j] = arr[i][j] + rhs[i][j];
            }
        }
        return result;
    }

    MyMatrix operator-() const{
        MyMatrix<T> result(m,n);
        for (int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                result[i][j] = -arr[i][j];
            }
        }

        return result;
    }

    MyMatrix operator-(const MyMatrix &rhs) const {

        if ((rhs.count_rows() != m) && (rhs.count_columns() != n)) {
            throw std::runtime_error("Attempt to subtract matrices of different sizes");
        }

        return (*this) + (-rhs);
    }

    MyMatrix operator*(const T &num) const {
        MyMatrix result(m, n);
        for (size_t i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result[i][j] = arr[i][j] * num;
            }
        }
        return result;
    }

    MyMatrix transpoce_matrix(const MyMatrix& matrix){
        size_t matrix_rows = matrix.count_rows();
        size_t matrix_columns = matrix.count_columns();

        MyMatrix<T> transpoced(matrix_columns, matrix_rows);
        for (int i = 0; i < matrix_rows; i++){
            for (int j = 0; j < matrix_columns; j++){
                transpoced[j][i] = matrix[i][j];
            }
        }

        return transpoced;
    }

    MyMatrix operator*(const MyMatrix &rhs) const {
        if (n != rhs.m) {
            throw std::runtime_error("Attempt to multiply inconsistent matrices");
        }

        MyMatrix<T> result(m, rhs.n);
        MyMatrix<T> rhs_copy = rhs;
        MyMatrix<T> trancpoced = transpoce_matrix(rhs_copy);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < rhs.n; j++) {

                double now = 0;
                for (int k = 0; k < n; k++) {
                    now += arr[i][k] * trancpoced[j][k];
                }
                result[i][j] = now;
            }
        }

        return result;
    }

    double determinant(const MyMatrix& matrix) const {
        if (matrix.n == 0){
            return 0;
        }
        if (matrix.n != matrix.m) {
            throw std::runtime_error ("Attempt to calculate the determinant of a non-square matrix");
        }


        double det = 1.0;

        MyMatrix<double> data_copy(m, n);
        for (int i = 0; i < matrix.m; i++){
            for (int j = 0; j < matrix.n; j++){
                data_copy[i][j] = matrix[i][j];;
            }
        }

        for (int i = 0; i < matrix.n; i++) {
            auto max_el = data_copy[i][i];
            int index = i;

            for (int j = i; j < matrix.n; j++) {
                if (fabs(data_copy[j][i]) > fabs(max_el)) {
                    max_el = data_copy[j][i];
                    index = j;
                }
            }

            if (index != i) {
                det = det * (-1);
                for (int k = 0; k < matrix.n; k++) {
                    auto tmp = data_copy[i][k];
                    data_copy[i][k] = data_copy[index][k];
                    data_copy[index][k] = tmp;
                }
            }

            for (int j = i + 1; j < matrix.n; j++) {
                if (fabs(data_copy[i][i]) < 0.000000001 ) return 0.0;
                auto factor = data_copy[j][i] / data_copy[i][i];
                for (int k = 0; k < matrix.m; k++) {
                    data_copy[j][k] = data_copy[j][k] - data_copy[i][k] * factor;
                }
            }

            det = det * data_copy[i][i];

        }

        return det;
    }

    void print(const MyMatrix& matrix){
        for (int i = 0; i < matrix.m; i++){
            for (int j = 0; j < matrix.n; j++){
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

template<numeric_type T>
MyMatrix<T> operator*(const T &num, const MyMatrix<T> &matrix) {
    MyMatrix<T> result(matrix.count_rows(), matrix.count_columns());
    for (size_t i = 0; i < matrix.count_rows(); i++) {
        for(size_t j = 0; j < matrix.count_columns(); j++){
            result[i][j] = matrix[i][j] * num;
        }
    }
    return result;
}