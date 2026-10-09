#include "Actions.hpp"
#include <algorithm>
#include <cstdio>
#include <system_error>

std::vector<std::filesystem::path> discoverDatasetFiles(const std::filesystem::path& executablePath) {
    std::filesystem::path datasetDirectory = "datasets";
    if (!std::filesystem::exists(datasetDirectory))
        datasetDirectory = executablePath.parent_path() / "datasets";

    std::vector<std::filesystem::path> files;
    std::error_code directoryError;
    for (const auto& entry : std::filesystem::directory_iterator(datasetDirectory, directoryError))
        if (entry.path().extension() == ".lab") files.push_back(entry.path());
    std::sort(files.begin(), files.end());
    return files;
}

void initializeState(RunState& state, int argc, char** argv) {
    state.files = discoverDatasetFiles(std::filesystem::absolute(argv[0]));
    state.task = static_cast<int>(state.data.task);
    if (!state.files.empty()) std::snprintf(state.path, sizeof(state.path), "%s", state.files[0].string().c_str());
    if (argc > 1) std::snprintf(state.path, sizeof(state.path), "%s", argv[1]);
}

void clearReport(RunState& state) {
    state.hasReport = false;
}

void loadDatasetFromPath(RunState& state) {
    try {
        state.data = loadDataset(std::filesystem::u8path(state.path));
        state.task = static_cast<int>(state.data.task);
        state.error.clear();
        clearReport(state);
    } catch (const std::exception& exception) {
        state.error = exception.what();
        state.hasReport = false;
    }
}

void createManualCase(RunState& state) {
    state.data = manualDataset(static_cast<Task>(state.task), state.rows, state.inner, state.columns);
    clearReport(state);
    state.error.clear();
}

void runCurrentTask(RunState& state) {
    try {
        state.report = runTask(state.data, state.pivoting);
        state.hasReport = true;
        state.error.clear();
    } catch (const std::exception& exception) {
        state.error = exception.what();
        state.hasReport = false;
    }
}

void updatePivotTolerance(RunState& state, double tolerance) {
    state.data.pivotTolerance = tolerance;
    state.data.hasExpected = false;
    state.data.withoutPivot = state.data.withPivot = Outcome::Success;
    state.data.name = "Manual case (modified tolerance)";
    clearReport(state);
}
