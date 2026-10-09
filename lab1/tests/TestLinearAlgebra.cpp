#include "TestFramework.hpp"

#include "LinAlgebra.hpp"

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

static void testVectorSpaceOps(int& failed) {
    const Vector a{1.0, 2.0, 3.0};
    const Vector b{4.0, -1.0, 0.5};

    const Vector sum = a + b;
    EXPECT_NEAR(failed, sum[0], 5.0, 1e-12);
    EXPECT_NEAR(failed, sum[1], 1.0, 1e-12);
    EXPECT_NEAR(failed, sum[2], 3.5, 1e-12);

    const Vector diff = a - b;
    EXPECT_NEAR(failed, diff[0], -3.0, 1e-12);
    EXPECT_NEAR(failed, diff[1], 3.0, 1e-12);
    EXPECT_NEAR(failed, diff[2], 2.5, 1e-12);

    const Vector scaledLeft = 3.0 * b;
    EXPECT_NEAR(failed, scaledLeft[0], 12.0, 1e-12);
    EXPECT_NEAR(failed, scaledLeft[1], -3.0, 1e-12);
    EXPECT_NEAR(failed, scaledLeft[2], 1.5, 1e-12);

    const Vector scaledRight = b * 3.0;
    EXPECT_NEAR(failed, scaledRight[0], 12.0, 1e-12);
    EXPECT_NEAR(failed, scaledRight[1], -3.0, 1e-12);
    EXPECT_NEAR(failed, scaledRight[2], 1.5, 1e-12);

    const double dot = a * b;
    EXPECT_NEAR(failed, dot, 3.5, 1e-12);
}

static void testMatrixSpaceOps(int& failed) {
    Matrix A(2, 2);
    A.values = {1.0, 2.0,
                3.0, 4.0};

    Matrix B(2, 2);
    B.values = {5.0, 6.0,
                7.0, 8.0};

    const Matrix C = A + B;
    EXPECT_NEAR(failed, C(0, 0), 6.0, 1e-12);
    EXPECT_NEAR(failed, C(0, 1), 8.0, 1e-12);
    EXPECT_NEAR(failed, C(1, 0), 10.0, 1e-12);
    EXPECT_NEAR(failed, C(1, 1), 12.0, 1e-12);

    const Matrix D = 2.0 * A - B;
    EXPECT_NEAR(failed, D(0, 0), -3.0, 1e-12);
    EXPECT_NEAR(failed, D(0, 1), -2.0, 1e-12);
    EXPECT_NEAR(failed, D(1, 0), -1.0, 1e-12);
    EXPECT_NEAR(failed, D(1, 1), 0.0, 1e-12);

    const Vector x{2.0, -1.0};
    const Vector y = A * x;
    EXPECT_NEAR(failed, y[0], 0.0, 1e-12);
    EXPECT_NEAR(failed, y[1], 2.0, 1e-12);

    const Matrix E = A * B;
    EXPECT_NEAR(failed, E(0, 0), 19.0, 1e-12);
    EXPECT_NEAR(failed, E(0, 1), 22.0, 1e-12);
    EXPECT_NEAR(failed, E(1, 0), 43.0, 1e-12);
    EXPECT_NEAR(failed, E(1, 1), 50.0, 1e-12);
}

void runLinearAlgebraTests(int& failed) {
    runCase(failed, "testVectorSpaceOps", testVectorSpaceOps);
    runCase(failed, "testMatrixSpaceOps", testMatrixSpaceOps);
}
