#pragma once
#include "Matrix3x3.hpp"

// Quaternion (s,x,y,z), with scalar s and vector part (x,y,z).
// Unit quaternions q and -q represent the same active, right-handed rotation.
struct Quat {

    double s = 1, x = 0, y = 0, z = 0;
    // Supplied unit-length copy; reject zero/non-finite quaternions with std::invalid_argument.
    Quat Normalized() const;

    // Hamilton product this*other; applies other's rotation first. Do not normalize.
    // Accept zero operands, reject non-finite ones, and accumulate scalar kernel counts.
    Quat Multiply(const Quat& other, Operations& operations) const;

    // Normalize this quaternion, rotate a finite vector, and accumulate kernel counts.
    // Zero/non-finite quaternions or non-finite vectors throw std::invalid_argument.
    Vec3 Rotate(const Vec3& v, Operations& operations) const;
    // Return a unit quaternion for a rotation matrix; otherwise throw std::invalid_argument.
    static Quat FromMatrix3x3(const Matrix3x3& matrix);
    // Normalize this quaternion and return its rotation matrix; reject zero/non-finite inputs.
    Matrix3x3 ToMatrix3x3() const;
    // Return a unit quaternion for a finite angle in radians and a normalized nonzero axis.
    // Invalid axes/angles throw std::invalid_argument.
    static Quat FromAxisAngle(const Vec3& axis, double angle);

    // Normalize and extract a unit axis and angle in [0,Pi] radians, treating q/-q equally.
    // The identity axis is non-unique; zero/non-finite inputs throw std::invalid_argument.
    void ToAxisAngle(Vec3& axis, double& angle) const;
};
