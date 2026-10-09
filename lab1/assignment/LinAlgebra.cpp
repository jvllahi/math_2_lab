#include "LinAlgebra.hpp"

#include <stdexcept>

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

Vector& Vector::operator+=(const Vector& /*other*/) {
    // TODO: element-wise addition with dimension validation.
    throw std::logic_error("Pending: Vector::operator+=");
}

Vector& Vector::operator-=(const Vector& /*other*/) {
    // TODO: element-wise subtraction with dimension validation.
    throw std::logic_error("Pending: Vector::operator-=");
}

Vector& Vector::operator*=(double /*scalar*/) {
    // TODO: scalar multiplication.
    throw std::logic_error("Pending: Vector::operator*=");
}

Vector& Vector::operator/=(double /*scalar*/) {
    // TODO: scalar division (validate nonzero scalar).
    throw std::logic_error("Pending: Vector::operator/=");
}

double Vector::dot(const Vector& /*other*/) const {
    // TODO: dot product with dimension validation.
    throw std::logic_error("Pending: Vector::dot");
}

Vector operator+(const Vector& /*vector*/) {
    // TODO: unary plus.
    throw std::logic_error("Pending: Vector unary +");
}

Vector operator-(const Vector& /*vector*/) {
    // TODO: unary minus.
    throw std::logic_error("Pending: Vector unary -");
}

Vector operator+(const Vector& /*left*/, const Vector& /*right*/) {
    // TODO: vector addition.
    throw std::logic_error("Pending: Vector binary +");
}

Vector operator-(const Vector& /*left*/, const Vector& /*right*/) {
    // TODO: vector subtraction.
    throw std::logic_error("Pending: Vector binary -");
}

Vector operator*(const Vector& /*vector*/, double /*scalar*/) {
    // TODO: vector-scalar multiplication.
    throw std::logic_error("Pending: Vector * scalar");
}

Vector operator*(double /*scalar*/, const Vector& /*vector*/) {
    // TODO: scalar-vector multiplication.
    throw std::logic_error("Pending: scalar * Vector");
}

Vector operator/(const Vector& /*vector*/, double /*scalar*/) {
    // TODO: vector-scalar division.
    throw std::logic_error("Pending: Vector / scalar");
}

double operator*(const Vector& /*left*/, const Vector& /*right*/) {
    // TODO: dot product operator.
    throw std::logic_error("Pending: Vector dot operator*");
}

Matrix Matrix::identity(std::size_t /*size*/) {
    // TODO: build identity matrix.
    throw std::logic_error("Pending: Matrix::identity");
}

double& Matrix::operator()(std::size_t /*row*/, std::size_t /*col*/) {
    // TODO: validate indices and return a writable element reference.
    throw std::logic_error("Pending: Matrix::operator() (write)");
}

double Matrix::operator()(std::size_t /*row*/, std::size_t /*col*/) const {
    // TODO: validate indices and return the element value.
    throw std::logic_error("Pending: Matrix::operator() (read)");
}

Matrix& Matrix::operator+=(const Matrix& /*other*/) {
    // TODO: matrix addition with shape validation.
    throw std::logic_error("Pending: Matrix::operator+=");
}

Matrix& Matrix::operator-=(const Matrix& /*other*/) {
    // TODO: matrix subtraction with shape validation.
    throw std::logic_error("Pending: Matrix::operator-=");
}

Matrix& Matrix::operator*=(double /*scalar*/) {
    // TODO: scalar multiplication.
    throw std::logic_error("Pending: Matrix::operator*=");
}

Matrix& Matrix::operator/=(double /*scalar*/) {
    // TODO: scalar division (validate nonzero scalar).
    throw std::logic_error("Pending: Matrix::operator/=");
}

Matrix operator+(const Matrix& /*matrix*/) {
    // TODO: unary plus.
    throw std::logic_error("Pending: Matrix unary +");
}

Matrix operator-(const Matrix& /*matrix*/) {
    // TODO: unary minus.
    throw std::logic_error("Pending: Matrix unary -");
}

Matrix operator+(const Matrix& /*left*/, const Matrix& /*right*/) {
    // TODO: matrix addition.
    throw std::logic_error("Pending: Matrix binary +");
}

Matrix operator-(const Matrix& /*left*/, const Matrix& /*right*/) {
    // TODO: matrix subtraction.
    throw std::logic_error("Pending: Matrix binary -");
}

Matrix operator*(const Matrix& /*matrix*/, double /*scalar*/) {
    // TODO: matrix-scalar multiplication.
    throw std::logic_error("Pending: Matrix * scalar");
}

Matrix operator*(double /*scalar*/, const Matrix& /*matrix*/) {
    // TODO: scalar-matrix multiplication.
    throw std::logic_error("Pending: scalar * Matrix");
}

Matrix operator/(const Matrix& /*matrix*/, double /*scalar*/) {
    // TODO: matrix-scalar division.
    throw std::logic_error("Pending: Matrix / scalar");
}

Vector operator*(const Matrix& /*matrix*/, const Vector& /*vector*/) {
    // TODO: matrix-vector product with dimension validation.
    throw std::logic_error("Pending: Matrix * Vector");
}

Matrix operator*(const Matrix& /*left*/, const Matrix& /*right*/) {
    // TODO: matrix-matrix product with dimension validation.
    throw std::logic_error("Pending: Matrix * Matrix");
}
