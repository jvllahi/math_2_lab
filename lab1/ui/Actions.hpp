#pragma once

#include "State.hpp"
#include <filesystem>
#include <vector>

// Returns every .lab file available near the executable or current datasets folder.
std::vector<std::filesystem::path> discoverDatasetFiles(const std::filesystem::path& executablePath);

// Initializes runtime state using discovered datasets and optional CLI path override.
void initializeState(RunState& state, int argc, char** argv);

// Clears the current validation report when inputs or settings change.
void clearReport(RunState& state);

// Loads a dataset from state.path and updates task/state metadata.
void loadDatasetFromPath(RunState& state);

// Creates a manual editable dataset using the selected dimensions and task.
void createManualCase(RunState& state);

// Executes the current task and stores the resulting validation report.
void runCurrentTask(RunState& state);

// Updates solver pivot tolerance and marks the case as a custom manual variant.
void updatePivotTolerance(RunState& state, double tolerance);
