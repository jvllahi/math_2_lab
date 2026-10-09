#pragma once
#include "Dataset.hpp"
#include <limits>
#include <string>

// Reference residual implementation used to validate solver output quality.
double independentResidual(const Matrix& A, const Vector& x, const Vector& b);

// Verifies that the student residual function behaves correctly.
bool checkResidualFunction(std::string& detail);

// Aggregated execution and validation results for one dataset run.
struct Report {
    // Result values produced by the executed task.
    Vector result;

    // Logical shape of result: matrix when resultCols > 1, vector otherwise.
    std::size_t resultRows = 0, resultCols = 0;

    // Arithmetic operation counters split by algorithmic stage.
    Operations product, elimination, substitution;

    // Measured execution time and expected interval in milliseconds.
    double milliseconds = 0, minMilliseconds = 0, maxMilliseconds = 0;

    // Reference residual, student residual, and numeric error versus expected.
    double residual = std::numeric_limits<double>::quiet_NaN();
    double studentResidual = std::numeric_limits<double>::quiet_NaN();
    double error = std::numeric_limits<double>::quiet_NaN();

    // Global correctness and high-level validation checks.
    bool correct = false, countsCorrect = false, timeExpected = false;

    // Specific checks for residual function and calibration consistency.
    bool residualCorrect = false, calibrationCorrect = false;

    // Flags indicating whether execution/count/time checks are applicable.
    bool completed = false, countsApplicable = false, timeApplicable = false;

    // Human-readable diagnostics.
    std::string detail, calibrationDetail;
};

// Executes one dataset task and computes all validation/report fields.
Report runTask(const Dataset& data, bool pivoting);
