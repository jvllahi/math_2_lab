#pragma once
#include "LinAlgebra.hpp"
#include <stdexcept>

// Euclidean length (L2); returns zero for an empty vector.
// Uses hypot to avoid overflow or underflow from squaring individual components.
double norm(const Vector& vector);
