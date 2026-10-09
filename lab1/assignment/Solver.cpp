#include "Solver.hpp"
#include "Numerics.hpp"
#include <cmath>

bool gaussianElimination(Matrix& /*A*/, Vector& /*b*/, bool /*pivoting*/,
                         double /*tolerance*/, Operations& /*operations*/) {
    // TODO (ex. 2): validate sizes, pivots, multipliers, and row updates.
    // TODO (ex. 3): if pivoting=true, choose and swap the pivot row.
    throw std::logic_error("Pending: gaussianElimination");
}

bool backSubstitution(const Matrix& /*U*/, Vector& /*c*/, double /*tolerance*/,
                      Operations& /*operations*/) {
    // TODO: validate sizes, iterate bottom-up, and replace c with x.
    throw std::logic_error("Pending: backSubstitution");
}

double relativeResidual(const Matrix& /*A*/, const Vector& /*x*/, const Vector& /*b*/) {
    // TODO: compute ||Ax-b|| / ||b|| with norm (provided); handle b=0.
    // Residual operations are not included in the solver operation count.
    throw std::logic_error("Pending: relativeResidual");
}
