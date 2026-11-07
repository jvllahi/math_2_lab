#pragma once
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <vector>

// Count only scalar arithmetic (+, -, *, /), not indices or comparisons.
struct Operations {
    std::uint64_t add = 0, subtract = 0, multiply = 0, divide = 0;
    std::uint64_t total() const { return add + subtract + multiply + divide; }
};

/////////////////////////////////
// Matrix and Vector classes with basic linear algebra operations.
/////////////////////////////////

class Vector {
public:
    using size_type = std::size_t;

    Vector() = default;
    explicit Vector(size_type size, double value = 0.0);
    Vector(std::initializer_list<double> init);

    size_type size() const noexcept;
    void resize(size_type size);
    void assign(size_type count, double value);

    double& operator[](size_type index);
    const double& operator[](size_type index) const;

    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(double scalar);

    double dot(const Vector& other) const;

private:
    std::vector<double> values_;
};

// These non-member operators provide natural algebraic syntax and scalar-left forms.
Vector operator+(const Vector& vector);
Vector operator-(const Vector& vector);
Vector operator+(const Vector& left, const Vector& right);
Vector operator-(const Vector& left, const Vector& right);
Vector operator*(const Vector& vector, double scalar);
Vector operator*(double scalar, const Vector& vector);
double operator*(const Vector& left, const Vector& right);

// Row-major storage: row i starts at i * cols.
class Matrix {
public:
    std::size_t rows = 0, cols = 0;
    Vector values;

    Matrix() = default;
    Matrix(std::size_t rows, std::size_t cols, double value = 0);
    void swapRows(std::size_t first, std::size_t second);

    static Matrix identity(std::size_t size);
    double& operator()(std::size_t row, std::size_t col);
    double operator()(std::size_t row, std::size_t col) const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(double scalar);
};

// These non-member operators provide natural algebraic syntax and scalar-left forms.
Matrix operator+(const Matrix& matrix);
Matrix operator-(const Matrix& matrix);
Matrix operator+(const Matrix& left, const Matrix& right);
Matrix operator-(const Matrix& left, const Matrix& right);
Matrix operator*(const Matrix& matrix, double scalar);
Matrix operator*(double scalar, const Matrix& matrix);
Vector operator*(const Matrix& matrix, const Vector& vector);
Matrix operator*(const Matrix& left, const Matrix& right);
