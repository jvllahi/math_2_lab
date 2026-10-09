# Maths_II - 2026
# Lab Assignment 1

## 1) Assignment

Your implementation work is in the `assignment/` folder:

- `assignment/LinAlgebra.cpp`
- `assignment/Solver.cpp`

The function contracts are defined in:

- `assignment/LinAlgebra.hpp`
- `assignment/Solver.hpp`

### Exercise 1: Linear algebra operations (`Vector` / `Matrix`)

Implement the algebraic API declared in `assignment/LinAlgebra.hpp`:

- `Matrix::identity`
- indexed access with `Matrix::operator()(row, col)`
- vector-space operations (`+`, `-`, scalar product, dot product)
- matrix-space operations (`+`, `-`, scalar product, `A*x`, `A*B`)

Required behavior:

- Validate dimensions and indices.
- Compute correct numerical results.

### Exercises 2 and 3: Linear system solver (`Solver`)

Implement:

- `gaussianElimination`
- `backSubstitution`
- `relativeResidual`

Expected rules from the headers/comments:

- Inputs must describe a non-empty square system with compatible vector size;
    otherwise throw `std::invalid_argument`.
- `gaussianElimination` returns `false` when a pivot is below tolerance.
- With `pivoting=false`, returning `false` does not necessarily mean the
    matrix is singular; a row swap might have fixed the step.
- `backSubstitution` must ignore elimination multipliers below the diagonal.
- `relativeResidual` must compute:
    `||Ax-b||_2 / ||b||_2`, and if `b=0`, return `||Ax-b||_2`.
- Residual arithmetic is not part of the solver operation counters.
- Count only scalar arithmetic operations in `Operations`:
    `+`, `-`, `*`, `/`.
- Do not count index arithmetic, comparisons, or control-flow operations.

Exercise split:

- Exercise 2: elimination + back substitution + residual.
- Exercise 3: add partial pivoting behavior to elimination.

## 2) Dataset `.lab` format (short reference)

Each dataset file is plain text with fixed keywords. The loader expects this
structure:

```text
LAB1 <version>
name "<case name>"
task <0..3>
outcomes <withoutPivot> <withPivot>
limits <pivotTolerance> <accuracy> <timeMinFactor> <timeMaxFactor>
A <rows> <cols>
<rows*cols values>
B <rows> <cols>
<rows*cols values>
input <count>
<count values>
expected <count>
<count values>
end
```

Notes:

- `task`: `0=Identity`, `1=MatrixVector`, `2=MatrixMatrix`, `3=Solve`.
- `outcomes`: `0=Success`, `1=PivotFailure`, `2=InvalidInput`.
- All numeric values must be finite.
- Matrix dimensions are capped at `2048 x 2048`.
- Vector value count is capped at `4*1024*1024`.
- Data after `end` is treated as an error.

## 3) Build and run

From the `lab1/` folder:

```sh
cmake --preset default
cmake --build --preset default --parallel
```

The first configure/build downloads SDL3 and Dear ImGui, so Internet access is
required.

Run the app:

Windows (PowerShell):

```powershell
.\build\bin\lab1.exe
```

macOS/Linux:

```sh
./build/bin/lab1
```

After code changes, rebuild and run again.

### Tests

A basic test suite is available under `tests/` for vector/matrix algebra and
solver behavior.

Run tests against the assignment implementation:

```sh
cmake --preset default-tests
cmake --build --preset default-tests --parallel
ctest --preset default-tests
```

**NOTE:** While the assignment is unfinished, tests are expected to fail; use them as a
progress indicator while implementing `assignment/LinAlgebra.cpp` and
`assignment/Solver.cpp`.

## 4) UI overview and workflow

The window is split into two panels.

### Left panel: Data

- Choose a predefined `.lab` case from `Included dataset`.
- Or type a path in `.lab file` and press `Load file`.
- Or create a manual case:
    - select operation type,
    - set dimensions,
    - press `Create editable manual case`.
- For solve tasks (`Ax=b`), you can toggle `Partial pivoting` and adjust
    `Pivot tolerance`.
- For small manual cases, matrix/vector cells are editable directly.

### Right panel: Validation

- Press `Run operation` to execute your current implementation.
- The app displays:
    - pass/fail status,
    - elapsed time,
    - arithmetic operation counts for solver stages,
    - expected timing range (for larger cases),
    - residual checks for solve tasks,
    - error against expected output when available.

For large matrices/vectors, the UI shows a partial preview while still using
full data for computation.

### Manual case workflow (step-by-step)

Use this when you want to craft a custom test instead of loading a dataset:

1. Build and open the app.
2. In the left panel, pick the target operation type (Identity, MatrixVector,
    MatrixMatrix, or Solve).
3. Set dimensions for the case (rows/cols and vector size as required by the
    selected operation).
4. Press `Create editable manual case`.
5. Fill matrix/vector entries in the editable grid.
6. For solve cases (`Ax=b`), set `Partial pivoting` and `Pivot tolerance`.
7. Press `Run operation`.
8. In the right panel, check status, output, operation counts, and residual
    information (for solve).
9. Change one variable at a time (dimensions, values, or pivoting), rerun,
    and compare results.

### Common manual-case mistakes

- Mismatched dimensions (for example, matrix-vector with `A.cols != x.size`).
- Solve case with non-square `A` or `b.size != A.rows`.
- Very large `Pivot tolerance`, which can trigger false pivot failures.
- Forgetting to rebuild after changing your C++ implementation.
- Changing many inputs at once, making regressions hard to isolate.

### App workflow checklist

Use this sequence every time you validate a change:

1. Build the project (`cmake --build --preset default --parallel`).
2. Start the app (`./build/bin/lab1` on macOS/Linux).
3. In the left panel, pick an `Included dataset` (or load a `.lab` path).
4. Check that the task type matches what you want to test.
5. For solver tasks, set `Partial pivoting` and `Pivot tolerance` explicitly.
6. If using a manual case, create it and edit matrix/vector entries as needed.
7. Press `Run operation`.
8. Read the right panel in this order:
    - `Result` status and detail message.
    - output shape and numeric error.
    - operation counts (`Product`, `Elimination`, `Substitution`).
    - residual checks for `Ax=b` tasks.
    - timing range checks (when applicable).
9. Repeat with at least one edge-case dataset (incompatible dimensions,
    nearly singular system, or zero vector/right-hand side).
10. Rebuild and retest after each code change.
