#pragma once
#include "LinAlgebra.hpp"
#include <filesystem>
#include <string>

// Supported operations used by the UI and validation pipeline.
enum class Task { Identity, MatrixVector, MatrixMatrix, Solve };

// Expected outcome category for solver runs.
enum class Outcome { Success, PivotFailure, InvalidInput };

// Complete input definition (and optional reference output) for one lab case.
struct Dataset {
    // Human-readable dataset name.
    std::string name = "Manual Case";

    // Operation type to execute.
    Task task = Task::Solve;

    // Operation data: A always exists; B is used for matrix-matrix tasks.
    Matrix A, B;

    // Input vector and expected output (when provided by dataset file).
    Vector input, expected;

    // True when expected contains a reference answer to compare against.
    bool hasExpected = false;

    // Expected solver outcomes with and without pivoting.
    Outcome withoutPivot = Outcome::Success, withPivot = Outcome::Success;

    // Numerical tolerances used in solve/validation checks.
    double pivotTolerance = 1e-12, accuracy = 1e-9;

    // Acceptable runtime range factors relative to reference complexity.
    double timeMinFactor = 0.05, timeMaxFactor = 40;
};

// Parses a .lab dataset file into a normalized Dataset structure.
Dataset loadDataset(const std::filesystem::path& path);

// Creates a synthetic editable dataset for manual testing from dimensions.
Dataset manualDataset(Task task, int rows, int inner, int columns);
