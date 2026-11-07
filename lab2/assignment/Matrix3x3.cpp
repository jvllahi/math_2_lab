#include "Matrix3x3.hpp"
#include <stdexcept>

bool Matrix3x3::IsRotation() const {
	// TODO (exercise 1): check finite, orthonormal columns and determinant +1.
    throw std::logic_error("Not yet implemented: IsRotation");
}

Matrix3x3 Matrix3x3::RotationAxisAngle(const Vec3& , double ) {
    // TODO (exercise 1): build the rotation from a nonzero axis and angle in radians.
    throw std::logic_error("Not yet implemented: RotationAxisAngle");
}

Vec3 Matrix3x3::Rotate(const Vec3& , Operations& ) const {
    // TODO (exercise 1): validate and apply R*v, accumulating scalar arithmetic counts.
    throw std::logic_error("Not yet implemented: Rotate");
}

void Matrix3x3::ToAxisAngle(Vec3& , double& ) const {
    // TODO (exercise 1): recover a unit axis and angle in [0,Pi], including identity and pi.
    throw std::logic_error("Not yet implemented: ToAxisAngle");
}

Matrix3x3 Matrix3x3::FromEulerZYX(double , double , double ) {
    // TODO (exercise 2): compose intrinsic ZYX rotations from finite angles in radians.
    throw std::logic_error("Not yet implemented: FromEulerZYX");
}

void Matrix3x3::ToEulerZYX(double& , double& , double& ) const {
    // TODO (exercise 2): extract canonical Euler ZYX angles, including gimbal-lock cases.
    throw std::logic_error("Not yet implemented: ToEulerZYX");
}
