#include "Numerics.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

// Allocate row-major storage filled with value; reject an overflowing element count.
Matrix::Matrix(std::size_t r, std::size_t c, double value) : rows(r), cols(c) {
    if (c && r > std::numeric_limits<std::size_t>::max() / c)
        throw std::length_error("Matrix too big!");
    values.assign(r * c, value);
}

// Exchange complete rows in place; invalid row indices throw std::out_of_range.
void Matrix::swapRows(std::size_t first, std::size_t second) {
    if (first >= rows || second >= rows) throw std::out_of_range("Nonexistent row");
    for (std::size_t j = 0; j < cols; ++j)
        std::swap(values[first * cols + j], values[second * cols + j]);
}

// Accumulate length without explicitly forming a potentially overflowing sum of squares.
double norm(const Vector& vector) {
    double result = 0;
    for (std::size_t i = 0; i < vector.size(); ++i)
        result = std::hypot(result, vector[i]);
    return result;
}
