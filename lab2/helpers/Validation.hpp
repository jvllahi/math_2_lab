#pragma once
#include "Quat.hpp"
#include <string>

// Operations selectable in the UI; ordering matches its operation dropdown.
enum class Task {
    IsRotation, AxisMatrix, AxisQuat, MatrixAxis, MatrixQuat, MatrixEuler,
    EulerMatrix, QuatProduct, QuatAxis, QuatMatrix, MatrixVector, QuatVector
};
// Editable inputs for all operations. Angles are radians, even though the UI uses degrees.
struct Input {
    Vec3 axis{0,0,1}, vector{1,2,3};
    double angle = Pi/4, yaw = Pi/6, pitch = Pi/18, roll = -Pi/9;
    Matrix3x3 matrix = Matrix3x3::Identity();
    Quat first, second;
};
// Operation outputs plus independent numerical validation and advisory arithmetic counts.
struct Report {
    Matrix3x3 matrix;
    Quat quat;
    Vec3 vector, axis;
    double angle = 0, yaw = 0, pitch = 0, roll = 0, milliseconds = 0, error = 0;
    // correct also covers rejecting invalid input; completed means the method returned normally.
    bool rotation = false, correct = false, completed = false;
    bool countsApplicable = false, countsExpected = false;
    Operations operations;
    std::string detail;
};

// Run the selected implementation and compare against independent reference calculations.
// std::invalid_argument is assessed against the input contract; other exceptions propagate.
Report runTask(Task task, const Input& input);
// Build UI examples 0..8, including identity, pi, gimbal lock, and deliberately invalid inputs.
Input exampleInput(int example);
