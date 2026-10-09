#pragma once
#include "LinAlgebra.hpp"

// Exercises 2 and 3. A and b are modified.

// In both methods: require a non-empty square system and a compatible vector;
// if not satisfied, throw std::invalid_argument.
// Return false when a pivot is below tolerance. With pivoting=false,
// this does NOT imply A is singular: row swaps might still be needed.
bool gaussianElimination(Matrix& A, Vector& b, bool pivoting, double tolerance, Operations& operations);

// U contains multipliers below the diagonal, which must be ignored here.
// Replace c with the solution; return false if a pivot is too small.
bool backSubstitution(const Matrix& U, Vector& c, double tolerance, Operations& operations);

// Exercise 2: ||Ax-b||_2 / ||b||_2. If b=0, return ||Ax-b||_2.
// The norm helper is already provided.
double relativeResidual(const Matrix& A, const Vector& x, const Vector& b);
