#pragma once
#include "Numerics.hpp"

inline constexpr double Pi = 3.14159265358979323846;
// Absolute tolerance for rotation orthogonality and determinant checks.
inline constexpr double Tolerance = 1e-6;

// Fixed-size 3D vector backed by the generic Vector from lab 1.
class Vec3 {
public:
    using size_type = Vector::size_type;

    // Construct from Cartesian components; defaults to the zero vector.
    Vec3(double x = 0, double y = 0, double z = 0);
    // Copy a generic vector; reject sizes other than three with std::invalid_argument.
    explicit Vec3(const Vector& vector);
    // Copy even from rvalues, keeping both objects' fixed-size storage valid.
    Vec3(const Vec3&) = default;
    Vec3& operator=(const Vec3&) = default;
    // Access x, y, z at indices 0, 1, 2; throw std::out_of_range otherwise.
    double& operator[](size_type index);
    double operator[](size_type index) const;
    // In-place vector addition/subtraction and scalar scaling, using lab 1 operators.
    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(double scalar);
    // Read-only generic storage for lab 1 arithmetic, without allowing resizing.
    const Vector& Algebra() const noexcept;

    // Scalar dot product, delegated to the generic Vector implementation.
    static double Dot(const Vec3& a, const Vec3& b);
    // Right-handed cross product, perpendicular to both input vectors.
    static Vec3 Cross(const Vec3& a, const Vec3& b);
    // Euclidean length, using the supplied generic vector norm.
    double Norm() const;
    // Return a unit vector; reject zero or non-finite inputs with std::invalid_argument.
    Vec3 Normalize() const;

private:
    Vector vector_;
};

// Natural vector algebra; results remain Vec3 and storage cannot be resized.
Vec3 operator+(const Vec3& vector);
Vec3 operator-(const Vec3& vector);
Vec3 operator+(const Vec3& left, const Vec3& right);
Vec3 operator-(const Vec3& left, const Vec3& right);
Vec3 operator*(const Vec3& vector, double scalar);
Vec3 operator*(double scalar, const Vec3& vector);
// Vector-vector multiplication is the scalar dot product, as in lab 1.
double operator*(const Vec3& left, const Vec3& right);

// True only when all three components are finite (neither NaN nor infinity).
bool isFinite(const Vec3& v);
