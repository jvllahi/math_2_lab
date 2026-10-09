#include "Matrix.hpp"
#include <stdexcept>

Matrix Matrix::identity(std::size_t /*size*/) {
    // TODO: build I with ones on the diagonal and zeros elsewhere.
    throw std::logic_error("Pending: Matrix::identity");
}

double& Matrix::at(std::size_t /*row*/, std::size_t /*col*/) {
    // TODO: validate indices and return a reference to the value.
    throw std::logic_error("Pending: Matrix::at (write)");
}

double Matrix::at(std::size_t /*row*/, std::size_t /*col*/) const {
    // TODO: validate indices and return the value.
    throw std::logic_error("Pending: Matrix::at (read)");
}

Vector Matrix::multiply(const Vector& /*vector*/, Operations& /*operations*/) const {
    // TODO: validate dimensions, compute Ax, and count sums/products.
    throw std::logic_error("Pending: Matrix::multiply (vector)");
}

Matrix Matrix::multiply(const Matrix& /*matrix*/, Operations& /*operations*/) const {
    // TODO: validate dimensions, compute AB, and count sums/products.
    throw std::logic_error("Pending: Matrix::multiply (matrix)");
}
