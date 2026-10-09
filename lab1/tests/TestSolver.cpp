#include "TestFramework.hpp"

#include "Solver.hpp"

#include <exception>

namespace {
void runCase(int& failed, const char* name, void (*test)(int&)) {
    try {
        test(failed);
    } catch (const std::exception& error) {
        reportFailure(failed, __FILE__, __LINE__, std::string(name) + ": " + error.what());
    } catch (...) {
        reportFailure(failed, __FILE__, __LINE__, std::string(name) + ": unknown exception");
    }
}
} // namespace

static void testSolveRegularSystem(int& failed) {
    Matrix A(2, 2);
    A.values = {2.0, 1.0,
                1.0, 3.0};
    const Vector b{1.0, 2.0};

    Matrix U = A;
    Vector c = b;

    Operations eliminationOps;
    Operations substitutionOps;
    EXPECT_TRUE(failed, gaussianElimination(U, c, true, 1e-12, eliminationOps));
    EXPECT_TRUE(failed, backSubstitution(U, c, 1e-12, substitutionOps));

    EXPECT_NEAR(failed, c[0], 0.2, 1e-12);
    EXPECT_NEAR(failed, c[1], 0.6, 1e-12);

    const double residual = relativeResidual(A, c, b);
    EXPECT_NEAR(failed, residual, 0.0, 1e-12);
}

static void testPivotingChangesOutcome(int& failed) {
    Matrix A(2, 2);
    A.values = {0.0, 1.0,
                1.0, 1.0};
    const Vector b{1.0, 2.0};

    {
        Matrix U = A;
        Vector c = b;
        Operations operations;
        EXPECT_TRUE(failed, !gaussianElimination(U, c, false, 1e-12, operations));
    }

    {
        Matrix U = A;
        Vector c = b;
        Operations eliminationOps;
        Operations substitutionOps;
        EXPECT_TRUE(failed, gaussianElimination(U, c, true, 1e-12, eliminationOps));
        EXPECT_TRUE(failed, backSubstitution(U, c, 1e-12, substitutionOps));
        EXPECT_NEAR(failed, c[0], 1.0, 1e-12);
        EXPECT_NEAR(failed, c[1], 1.0, 1e-12);
    }
}

static void testInvalidArguments(int& failed) {
    Matrix nonSquare(2, 3, 0.0);
    Vector rhs{1.0, 2.0};
    Operations operations;

    EXPECT_THROW_INVALID_ARGUMENT(failed,
        gaussianElimination(nonSquare, rhs, true, 1e-12, operations));

    const Matrix A(2, 2, 0.0);
    const Vector x{1.0};
    EXPECT_THROW_INVALID_ARGUMENT(failed,
        relativeResidual(A, x, rhs));
}

void runSolverTests(int& failed) {
    runCase(failed, "testSolveRegularSystem", testSolveRegularSystem);
    runCase(failed, "testPivotingChangesOutcome", testPivotingChangesOutcome);
    runCase(failed, "testInvalidArguments", testInvalidArguments);
}
