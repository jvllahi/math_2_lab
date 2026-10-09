#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

using Vector = std::vector<double>;

// Count only scalar arithmetic (+, -, *, /), not indices or comparisons.
struct Operations {
    std::uint64_t add = 0, subtract = 0, multiply = 0, divide = 0;
    std::uint64_t total() const { return add + subtract + multiply + divide; }
};

// Row-major storage: row i starts at i * cols.
struct Matrix {
    std::size_t rows = 0, cols = 0;
    Vector values;
    Matrix() = default;
    Matrix(std::size_t rows, std::size_t cols, double value = 0);
    void swapRows(std::size_t first, std::size_t second);

    // Exercise 1: these are the only Matrix methods to complete.
    static Matrix identity(std::size_t size);
    double& at(std::size_t row, std::size_t col);
    double at(std::size_t row, std::size_t col) const;
    Vector multiply(const Vector& vector, Operations& operations) const;
    Matrix multiply(const Matrix& matrix, Operations& operations) const;
};
