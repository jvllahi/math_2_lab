#include "Solver.hpp"
#include <gtest/gtest.h>

TEST(SolverTest, SolveRegularSystem) {
    Matrix A(2, 2);
    A.values = {2.0, 1.0,
                1.0, 3.0};
    const Vector b{1.0, 2.0};

    Matrix U = A;
    Vector c = b;

    Operations eliminationOps;
    Operations substitutionOps;
    EXPECT_TRUE(gaussianElimination(U, c, true, 1e-12, eliminationOps));
    EXPECT_TRUE(backSubstitution(U, c, 1e-12, substitutionOps));

    EXPECT_NEAR(c[0], 0.2, 1e-12);
    EXPECT_NEAR(c[1], 0.6, 1e-12);

    const double residual = relativeResidual(A, c, b);
    EXPECT_NEAR(residual, 0.0, 1e-12);
}

TEST(SolverTest, PivotingChangesOutcome) {
    Matrix A(2, 2);
    A.values = {0.0, 1.0,
                1.0, 1.0};
    const Vector b{1.0, 2.0};

    {
        Matrix U = A;
        Vector c = b;
        Operations operations;
        EXPECT_FALSE(gaussianElimination(U, c, false, 1e-12, operations));
    }

    {
        Matrix U = A;
        Vector c = b;
        Operations eliminationOps;
        Operations substitutionOps;
        EXPECT_TRUE(gaussianElimination(U, c, true, 1e-12, eliminationOps));
        EXPECT_TRUE(backSubstitution(U, c, 1e-12, substitutionOps));
        EXPECT_NEAR(c[0], 1.0, 1e-12);
        EXPECT_NEAR(c[1], 1.0, 1e-12);
    }
}

TEST(SolverTest, InvalidArguments) {
    Matrix nonSquare(2, 3, 0.0);
    Vector rhs{1.0, 2.0};
    Operations operations;

    EXPECT_THROW(
        gaussianElimination(nonSquare, rhs, true, 1e-12, operations),
        std::invalid_argument);

    const Matrix A(2, 2, 0.0);
    const Vector x{1.0};
    EXPECT_THROW(relativeResidual(A, x, rhs), std::invalid_argument);
}
