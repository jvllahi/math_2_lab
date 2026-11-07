#pragma once
#include "Geometry.hpp"
#include <cstddef>

// Fixed 3x3 Matrix for active, right-handed rotations acting on column vectors.
// Basic storage/algebra methods are supplied in helpers/Geometry.cpp.
class Matrix3x3 {
public:
    // ========================================================================
    // 1. TASKS TO COMPLETE: IMPLEMENT ONLY THESE SIX METHODS IN Matrix3x3.cpp
    // ========================================================================

    // [1/6] Test finite entries, R^T*R = I, and det(R) = +1 within Tolerance; do not throw.
    bool IsRotation() const;

    // [2/6] Rodrigues rotation about a normalized axis; angle is in radians.
    // Reject zero/non-finite axes or non-finite angles with std::invalid_argument.
    static Matrix3x3 RotationAxisAngle(const Vec3& axis, double angle);

    // [3/6] Extract a unit axis and angle in [0,Pi] radians, including identity and pi cases.
    // The identity axis is non-unique. Reject non-rotation matrices with std::invalid_argument.
    void ToAxisAngle(Vec3& axis, double& angle) const;

    // [4/6] Apply this rotation to a finite vector and accumulate only product-kernel counts.
    // Invalid matrices/vectors throw std::invalid_argument.
    Vec3 Rotate(const Vec3& v, Operations& operations) const;

    // [5/6] Construct intrinsic ZYX: Rz(yaw)*Ry(pitch)*Rx(roll), with finite radians.
    // Non-finite angles throw std::invalid_argument.
    static Matrix3x3 FromEulerZYX(double yaw, double pitch, double roll);

    // [6/6] Extract radians: yaw/roll in [-Pi,Pi], pitch in [-Pi/2,Pi/2].
    // Handle gimbal lock; reject non-rotation matrices with std::invalid_argument.
    void ToEulerZYX(double& yaw, double& pitch, double& roll) const;

    // ========================================================================
    // 2. SUPPLIED HELPERS: USE THESE METHODS; DO NOT REIMPLEMENT THEM
    // Already implemented in helpers/Geometry.cpp or defaulted below.
    // These declarations are NOT additional tasks.
    // ========================================================================

    // Default storage is zero, not a rotation; use Identity() for the identity.
    Matrix3x3();
    // Initialize exactly nine row-major entries; otherwise throw std::invalid_argument.
    Matrix3x3(std::initializer_list<double> values);
    // Copy a generic 3x3 Matrix; reject incompatible shape/storage with std::invalid_argument.
    explicit Matrix3x3(const Matrix& matrix);
    // Copy even from rvalues to preserve both objects' fixed-size storage.
    Matrix3x3(const Matrix3x3&) = default;
    Matrix3x3& operator=(const Matrix3x3&) = default;
    // Read-only generic Matrix for reusing lab 1 algebra without changing dimensions.
    const Matrix& Algebra() const noexcept;

    // Return the 3x3 identity matrix.
    static Matrix3x3 Identity();
    // Zero-based entry access; throw std::out_of_range for indices outside [0,2].
    double& At(std::size_t row, std::size_t col);
    double At(std::size_t row, std::size_t col) const;
    // In-place matrix addition/subtraction and scalar scaling, using lab 1 operators.
    Matrix3x3& operator+=(const Matrix3x3& other);
    Matrix3x3& operator-=(const Matrix3x3& other);
    Matrix3x3& operator*=(double scalar);
    // Generic R*v product; does not check rotation validity or record arithmetic counts.
    Vec3 Multiply(const Vec3& v) const;
    // Generic R*matrix product, using lab 1 multiplication.
    Matrix3x3 Multiply(const Matrix3x3& matrix) const;
    // Determinant; a proper rotation has determinant +1.
    double Det() const;
    // Return the transpose, which is the inverse for a rotation matrix.
    Matrix3x3 Transposed() const;
    // Sum of diagonal entries; for a rotation, trace = 1 + 2*cos(angle).
    double Trace() const;

private:
    // Supplied fixed-size storage; no student implementation required.
    Matrix matrix_;
};

// ========================================================================
// Supplied in Geometry.cpp; students do not implement these operators.
// Natural matrix algebra, with fixed-size results and no rotation checks or counts.
// ========================================================================

Matrix3x3 operator+(const Matrix3x3& matrix);
Matrix3x3 operator-(const Matrix3x3& matrix);
Matrix3x3 operator+(const Matrix3x3& left, const Matrix3x3& right);
Matrix3x3 operator-(const Matrix3x3& left, const Matrix3x3& right);
Matrix3x3 operator*(const Matrix3x3& matrix, double scalar);
Matrix3x3 operator*(double scalar, const Matrix3x3& matrix);
Vec3 operator*(const Matrix3x3& matrix, const Vec3& vector);
Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right);
