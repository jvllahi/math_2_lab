# Maths_II - 2026
# Lab Assignment 1

## 1) Assignment

Your implementation work is in the `assignment/` folder:

- `assignment/Matrix.cpp`
- `assignment/Solver.cpp`

The function contracts are defined in:

- `assignment/Matrix.hpp`
- `assignment/Solver.hpp`

### Exercise 1: Matrix operations (`Matrix`)

Implement only these methods:

- `Matrix::identity`
- `Matrix::at` (read and write overloads)
- `Matrix::multiply(const Vector&, Operations&)`
- `Matrix::multiply(const Matrix&, Operations&)`

Required behavior:

- Validate dimensions and indices.
- Compute correct numerical results.
- Count only scalar arithmetic operations in `Operations`:
    `+`, `-`, `*`, `/`.
- Do not count index arithmetic, comparisons, or control-flow operations.

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
    - arithmetic operation counts by stage,
    - expected timing range (for larger cases),
    - residual checks for solve tasks,
    - error against expected output when available.

For large matrices/vectors, the UI shows a partial preview while still using
full data for computation.
