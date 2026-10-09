#pragma once

#include "Validation.hpp"
#include <filesystem>
#include <string>
#include <vector>

// Mutable application/session state shared by actions and view rendering.
struct RunState {
    // Active dataset currently shown and executed in the UI.
    Dataset data = manualDataset(Task::Solve, 3, 3, 3);

    // Last execution report for the active dataset.
    Report report;

    // True when report contains a valid result for the current inputs.
    bool hasReport = false;

    // Solver option: enable/disable partial pivoting.
    bool pivoting = false;

    // Manual case configuration selected in the combo/inputs.
    int task = static_cast<int>(Task::Solve);
    int rows = 3;
    int inner = 3;
    int columns = 3;

    // Selected index inside files for the dataset picker.
    int selection = 0;

    // Path buffer used by the text input to load .lab files.
    char path[2048] = "";

    // Last user-visible error message from load/execute actions.
    std::string error;

    // Cached list of available dataset files.
    std::vector<std::filesystem::path> files;
};
