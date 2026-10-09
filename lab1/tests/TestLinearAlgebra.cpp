#include "LinAlgebra.hpp"
#include <gtest/gtest.h>

namespace {
constexpr double tol_eps = 1e-12;
} // namespace

TEST(LinearAlgebraTest, VectorSpaceOps) {
    const Vector a{1.0, 2.0, 3.0};
    const Vector b{4.0, -1.0, 0.5};

    const Vector sum = a + b;
    EXPECT_NEAR(sum[0], 5.0, tol_eps);
    EXPECT_NEAR(sum[1], 1.0, tol_eps);
    EXPECT_NEAR(sum[2], 3.5, tol_eps);

    const Vector diff = a - b;
    EXPECT_NEAR(diff[0], -3.0, tol_eps);
    EXPECT_NEAR(diff[1], 3.0, tol_eps);
    EXPECT_NEAR(diff[2], 2.5, tol_eps);

    const Vector scaledLeft = 3.0 * b;
    EXPECT_NEAR(scaledLeft[0], 12.0, tol_eps);
    EXPECT_NEAR(scaledLeft[1], -3.0, tol_eps);
    EXPECT_NEAR(scaledLeft[2], 1.5, tol_eps);

    const Vector scaledRight = b * 3.0;
    EXPECT_NEAR(scaledRight[0], 12.0, tol_eps);
    EXPECT_NEAR(scaledRight[1], -3.0, tol_eps);
    EXPECT_NEAR(scaledRight[2], 1.5, tol_eps);

    const double dot = a * b;
    EXPECT_NEAR(dot, 3.5, tol_eps);
}

TEST(LinearAlgebraTest, MatrixSpaceOps) {
    Matrix A(2, 2);
    A.values = {1.0, 2.0,
                3.0, 4.0};

    Matrix B(2, 2);
    B.values = {5.0, 6.0,
                7.0, 8.0};

    const Matrix C = A + B;
    EXPECT_NEAR(C(0, 0), 6.0, tol_eps);
    EXPECT_NEAR(C(0, 1), 8.0, tol_eps);
    EXPECT_NEAR(C(1, 0), 10.0, tol_eps);
    EXPECT_NEAR(C(1, 1), 12.0, tol_eps);

    const Matrix D = 2.0 * A - B;
    EXPECT_NEAR(D(0, 0), -3.0, tol_eps);
    EXPECT_NEAR(D(0, 1), -2.0, tol_eps);
    EXPECT_NEAR(D(1, 0), -1.0, tol_eps);
    EXPECT_NEAR(D(1, 1), 0.0, tol_eps);

    const Vector x{2.0, -1.0};
    const Vector y = A * x;
    EXPECT_NEAR(y[0], 0.0, tol_eps);
    EXPECT_NEAR(y[1], 2.0, tol_eps);

    const Matrix E = A * B;
    EXPECT_NEAR(E(0, 0), 19.0, tol_eps);
    EXPECT_NEAR(E(0, 1), 22.0, tol_eps);
    EXPECT_NEAR(E(1, 0), 43.0, tol_eps);
    EXPECT_NEAR(E(1, 1), 50.0, tol_eps);
}

TEST(LinearAlgebraTest, AlgebraicExpressionExpApprox) {
    Matrix A(2, 2);
    A.values = {1.0, 2.0,
                0.0, -1.0};

    Matrix B(2, 2);
    B.values = {3.0, 1.0,
                2.0, 4.0};

    const Vector x{2.0, -1.0};
    const Vector v{4.0, -2.0};

    // 3-term truncated exp(A): (I + A + 0.5*A^2) applied to x,
    // then combined with an extra matrix-vector term.
    const Matrix I = Matrix::identity(2);
    const Vector result = (I + A + 0.5 * (A * A)) * x + 0.25 * (B * v);

    EXPECT_NEAR(result[0], 5.5, tol_eps);
    EXPECT_NEAR(result[1], -0.5, tol_eps);
}
