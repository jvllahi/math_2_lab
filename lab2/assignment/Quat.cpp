#include "Quat.hpp"
#include <stdexcept>

Quat Quat::Multiply(const Quat& , Operations& ) const {
    // TODO (exercise 3): compute the unnormalized Hamilton product and count arithmetic.
    throw std::logic_error("Not yet implemented: Multiply");
}

Vec3 Quat::Rotate(const Vec3& , Operations& ) const {
    // TODO (exercise 3): normalize the quaternion, rotate the vector, and count kernel arithmetic.
    throw std::logic_error("Not yet implemented: Rotate");
}

Quat Quat::FromAxisAngle(const Vec3& , double ) {
    // TODO (exercise 3): convert a nonzero axis and finite angle in radians to a unit quaternion.
    throw std::logic_error("Not yet implemented: FromAxisAngle");
}

Matrix3x3 Quat::ToMatrix3x3() const {
    // TODO (exercise 3): normalize the quaternion and build the equivalent rotation matrix.
    throw std::logic_error("Not yet implemented: ToMatrix3x3");
}

Quat Quat::FromMatrix3x3(const Matrix3x3& ) {
    // TODO (exercise 3): convert a valid rotation matrix to a unit quaternion, including pi rotations.
    throw std::logic_error("Not yet implemented: FromMatrix3x3");
}

void Quat::ToAxisAngle(Vec3& , double& ) const {
    // TODO (exercise 3): recover a unit axis and angle in [0,Pi], consistently for q and -q.
    throw std::logic_error("Not yet implemented: ToAxisAngle");
}
