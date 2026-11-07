#include "LinAlgebra.hpp"

#include <algorithm>
#include <stdexcept>

namespace {
void requireSameSize(std::size_t left, std::size_t right, const char* message) {
    if (left != right) throw std::invalid_argument(message);
}

void requireSameShape(const Matrix& left, const Matrix& right, const char* message) {
    if (left.rows != right.rows || left.cols != right.cols) throw std::invalid_argument(message);
}
} // namespace

Vector::Vector(size_type size, double value) : values_(size, value) {}

Vector::Vector(std::initializer_list<double> init) : values_(init) {}

Vector::size_type Vector::size() const noexcept {
    return values_.size();
}

void Vector::resize(size_type size) {
    values_.resize(size);
}

void Vector::assign(size_type count, double value) {
    values_.assign(count, value);
}

double& Vector::operator[](size_type index) {
    return values_[index];
}

const double& Vector::operator[](size_type index) const {
    return values_[index];
}

Vector& Vector::operator+=(const Vector& other) {
    requireSameSize(size(), other.size(), "Incompatible dimensions");
    for (size_type i = 0; i < size(); ++i) values_[i] += other.values_[i];
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    requireSameSize(size(), other.size(), "Incompatible dimensions");
    for (size_type i = 0; i < size(); ++i) values_[i] -= other.values_[i];
    return *this;
}

Vector& Vector::operator*=(double scalar) {
    for (double& value : values_) value *= scalar;
    return *this;
}

double Vector::dot(const Vector& other) const {
    requireSameSize(size(), other.size(), "Incompatible dimensions");
    double sum = 0.0;
    for (size_type i = 0; i < size(); ++i) sum += values_[i] * other.values_[i];
    return sum;
}

Vector operator+(const Vector& vector) {
    return vector;
}

Vector operator-(const Vector& vector) {
    Vector copy = vector;
    copy *= -1.0;
    return copy;
}

Vector operator+(const Vector& left, const Vector& right) {
    Vector result = left;
    result += right;
    return result;
}

Vector operator-(const Vector& left, const Vector& right) {
    Vector result = left;
    result -= right;
    return result;
}

Vector operator*(const Vector& vector, double scalar) {
    Vector result = vector;
    result *= scalar;
    return result;
}

Vector operator*(double scalar, const Vector& vector) {
    return vector * scalar;
}

double operator*(const Vector& left, const Vector& right) {
    return left.dot(right);
}

Matrix Matrix::identity(std::size_t size) {
    Matrix result(size, size, 0);
    for (std::size_t i = 0; i < size; ++i) result(i, i) = 1;
    return result;
}

double& Matrix::operator()(std::size_t row, std::size_t col) {
    if (row >= rows || col >= cols) throw std::out_of_range("Matrix index out of range");
    return values[row * cols + col];
}

double Matrix::operator()(std::size_t row, std::size_t col) const {
    if (row >= rows || col >= cols) throw std::out_of_range("Matrix index out of range");
    return values[row * cols + col];
}

Matrix& Matrix::operator+=(const Matrix& other) {
    requireSameShape(*this, other, "Incompatible dimensions");
    for (std::size_t i = 0; i < values.size(); ++i) values[i] += other.values[i];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    requireSameShape(*this, other, "Incompatible dimensions");
    for (std::size_t i = 0; i < values.size(); ++i) values[i] -= other.values[i];
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (std::size_t i = 0; i < values.size(); ++i) values[i] *= scalar;
    return *this;
}

Matrix operator+(const Matrix& matrix) {
    return matrix;
}

Matrix operator-(const Matrix& matrix) {
    Matrix copy = matrix;
    copy *= -1.0;
    return copy;
}

Matrix operator+(const Matrix& left, const Matrix& right) {
    Matrix result = left;
    result += right;
    return result;
}

Matrix operator-(const Matrix& left, const Matrix& right) {
    Matrix result = left;
    result -= right;
    return result;
}

Matrix operator*(const Matrix& matrix, double scalar) {
    Matrix result = matrix;
    result *= scalar;
    return result;
}

Matrix operator*(double scalar, const Matrix& matrix) {
    return matrix * scalar;
}

Vector operator*(const Matrix& matrix, const Vector& vector) {
    if (matrix.cols != vector.size()) throw std::invalid_argument("Incompatible dimensions");
    Vector result(matrix.rows, 0.0);
    for (std::size_t i = 0; i < matrix.rows; ++i) {
        double sum = 0.0;
        for (std::size_t j = 0; j < matrix.cols; ++j) sum += matrix(i, j) * vector[j];
        result[i] = sum;
    }
    return result;
}

Matrix operator*(const Matrix& left, const Matrix& right) {
    if (left.cols != right.rows) throw std::invalid_argument("Incompatible dimensions");
    Matrix result(left.rows, right.cols, 0.0);
    for (std::size_t i = 0; i < left.rows; ++i) {
        for (std::size_t j = 0; j < right.cols; ++j) {
            double sum = 0.0;
            for (std::size_t k = 0; k < left.cols; ++k) sum += left(i, k) * right(k, j);
            result(i, j) = sum;
        }
    }
    return result;
}
