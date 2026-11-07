# Laboratory 2 · Mathematics II · 2026

Repository: https://github.com/jvllahi/math_2_lab

## 1) Assignment

The complete assignment, theoretical framework, report questions, evaluation
criteria, and submission requirements are in
[docs/LabAssignment2.pdf](docs/LabAssignment2.pdf).
The LaTeX source is [docs/LabAssignment2.tex](docs/LabAssignment2.tex).

This lab builds on the generic `Vector`, `Matrix`, and `Operations` from lab 1.
You must supply your own completed implementation from the first assignment.
The supplied [helpers/](helpers/) include lab 1 numerical utilities,
fixed-size geometry, and independent UI validation.

`Vec3` contains a three-element `Vector`; `Matrix3x3` contains a 3x3 `Matrix`.
They retain the natural algebra operators from lab 1. Use vector indexing and
`R.At(row, col)` for component access. Constructors and basic algebra are
supplied; your work is in the rotation TODOs.

### Bring your lab1 algebra

Before configuring or building lab2:

1. Create a folder named `algebra` inside `lab2/`.
2. Copy your completed `LinAlgebra.cpp` and `LinAlgebra.hpp` from your lab1
   assignment into that folder, keeping their original filenames and public API.

The resulting layout must be:

```text
lab2/
  algebra/
    LinAlgebra.cpp
    LinAlgebra.hpp
```

This folder is intentionally not distributed and is ignored by Git. Use your own
previous work, not an instructor reference solution. The same local algebra is
required for both app and test builds, whichever implementation preset you select.
Matrix allocation, row swaps, and the vector norm are already supplied in
`helpers/Numerics.cpp`; do not copy duplicate implementations of these utilities.

The function contracts are defined in:

- [assignment/Matrix3x3.hpp](assignment/Matrix3x3.hpp)
- [assignment/Quat.hpp](assignment/Quat.hpp)

### Exercise 1: Rotation matrices and axis-angle

Implement in [assignment/Matrix3x3.cpp](assignment/Matrix3x3.cpp):

- `IsRotation`
- `RotationAxisAngle`
- `ToAxisAngle`
- `Rotate`

### Exercise 2: Euler ZYX angles

Implement `FromEulerZYX` and `ToEulerZYX` in
[assignment/Matrix3x3.cpp](assignment/Matrix3x3.cpp).

### Exercise 3: Quaternions

Implement in [assignment/Quat.cpp](assignment/Quat.cpp):

- `Multiply` and `Rotate`
- `FromAxisAngle` and `ToAxisAngle`
- `FromMatrix3x3` and `ToMatrix3x3`

All function angles are in radians; the UI uses degrees. Follow the header
contracts for input validation, output ranges, and arithmetic counts.
The handout explains the conventions and the required edge cases.

## 2) Build and run

1. Download the repository using **Code → Download ZIP** and extract it.
2. Install **CMake 3.24 or later** and a **C++17** compiler. On Windows,
   install Visual Studio with "Desktop development with C++"; on macOS,
   install the Xcode tools; on Linux, install the build tools and the
   OpenGL and SDL3 development dependencies.
3. Create `lab2/algebra/` and copy your previous solution into it as described above.
4. Open a terminal in the `lab2/` folder and run:

```sh
cmake --preset default
cmake --build --preset default --parallel
```

The first build requires Internet access to download SDL3 and Dear ImGui.

Run the application:

**Windows (PowerShell):**

```powershell
.\build\bin\lab2.exe
```

**macOS/Linux:**

```sh
./build/bin/lab2
```

After modifying the code, run the build command again and reopen the application.

### Tests

The GoogleTest suite in [tests/](tests/) checks generic-backed storage, natural
algebra, known rotations, round trips, singular cases, invalid inputs, arithmetic
counts, and the included UI examples. Tests need no SDL3, OpenGL, or display.
GoogleTest is found locally or downloaded on first configuration.

Run tests against the assignment implementation:

```sh
cmake --preset default-tests
cmake --build --preset default-tests --parallel
ctest --preset default-tests
```

While assignment TODOs remain, rotation tests are expected to fail; use them as
a progress indicator. Supplied geometry tests should already pass.
The test executable is `build-tests/bin/lab2_tests` (`.exe` on Windows).

## 3) UI overview and workflow

Use the **Operation** and **Example** controls above the two panels.
Selecting an example does not load it until you press **Load example**.
Changing **Operation** clears the previous report; changing **Example** alone
does not change the inputs or clear the report.

### Left panel: Data

- Edit the inputs shown for the selected operation: axis, angle, matrix,
  quaternion, vector, or Euler angles.
- All editable matrices are 3x3 and all vectors have three components.
- Enter angles in degrees; the app converts them to radians for your methods.
- Quaternions use component order `(s,x,y,z)`.
- Included examples cover identity, 180-degree and nearby rotations, both
  gimbal-lock signs, a reflection, and zero axis/quaternion inputs.

Each operation uses only its displayed inputs. The axis-angle, matrix,
quaternion, and Euler fields are not automatically kept in sync. In particular,
**Identity (0 degrees)** does not reset the Euler angles: for an identity test
with **Euler ZYX -> matrix**, enter zero yaw, pitch, and roll yourself.

### Right panel: Validation

Press **Run operation** to execute your current implementation.
The app displays:

- **CORRECT** or **REVIEW**, with explanatory details when available.
- The output matrix, quaternion, vector, axis-angle, Euler angles, or boolean.
- Numerical error against an independent reference, with UI tolerance `5e-6`.
- Informational elapsed time. Arithmetic counts appear only for **Product q1 * q2**,
  **Rotate vector with matrix**, and **Rotate vector with quaternion**.
- Advisory standard-count comparisons; different algorithms require justification.
- An oblique projection for the two vector-rotation operations, when both vectors
  are finite: blue is the original vector, orange is the rotated vector.

An invalid input correctly rejected with `std::invalid_argument` can receive
**CORRECT** status without a numerical output. Ordinary unfinished TODOs
produce an **Error** message instead of a result status. Output, timing,
numerical error, and applicable counts are shown only when the implemented
method returns normally.

Use **Copy R to input** or **Copy q to q1**, when available, to reuse a result
as an input for another operation. Changing an editable input or operation
invalidates the previous report, as does **Load example**.
**Copy q to q1** clears the report, whereas **Copy R to input** leaves the
current report visible. Select the next operation or rerun to validate the
copied value.

### Manual case workflow

1. Build and open the app.
2. Select the operation you want to test.
3. Load a suitable example, then edit its inputs to create a manual case.
4. Press **Run operation**.
5. Read the result status and detail message before checking numerical output.
6. Inspect numerical error, arithmetic counts, and the projection when applicable.
7. Change one input at a time, rerun, and compare results.

### Common manual-case mistakes

- Confusing the UI's degrees with the methods' radians.
- Using a zero axis or zero quaternion for a conversion or rotation.
- Treating a reflection as a proper rotation.
- Mixing quaternion component order or Euler conventions.
- Expecting identical parameters when two representations describe the same rotation.
- Forgetting to rebuild and restart the app after changing C++ code.

### App workflow checklist

1. Rebuild after each implementation change.
2. Load a normal example and confirm the selected operation.
3. Run it and inspect the status, output, numerical error, and applicable counts.
4. Repeat with identity, a half-turn, and nearby angles.
5. Check both gimbal-lock examples for Euler conversions.
6. Check the relevant invalid-input examples and their rejection messages.
7. Run the automated tests and record the evidence required by the handout.

## 4) Academic integrity and AI use

Using AI tools (LLMs, code assistants) as tutoring support is allowed.
Submitting generated code or report text as your own work without disclosure is
not allowed.

Submission requirements:

- If you used AI support, include a short `AI_USAGE.md` file in your zip.
- In `AI_USAGE.md`, list the tool name, what it was used for, and the final prompts
  that directly influenced your submitted code or report.
- You must be able to explain and justify every submitted function and test.

During review, students will be asked for a brief explanation of selected code
fragments or test decisions. A submitted part that the student cannot explain
will be considered invalid for grading.
