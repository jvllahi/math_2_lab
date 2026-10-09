#pragma once

#include "Matrix.hpp"

namespace ui {
// Draws a compact matrix table; returns true if any editable cell changed.
bool matrixTable(const char* label, Matrix& matrix, bool editable);

// Draws a compact vector table; returns true if any editable value changed.
bool vectorTable(const char* label, Vector& vector, bool editable);

// Draws a green/red status line for boolean checks.
void feedback(const char* label, bool ok);

// Prints arithmetic operation counters in a single formatted line.
void operationCounts(const char* label, const Operations& op);
}
