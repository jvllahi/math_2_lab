#include "Quat.hpp"
#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>

namespace {
constexpr double epsilon=1e-10;

void expectMatrix(const Matrix3x3& actual, const Matrix3x3& expected, double tolerance=epsilon) {
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) EXPECT_NEAR(actual.At(i,j),expected.At(i,j),tolerance);
}

void expectVector(const Vec3& actual, const Vec3& expected, double tolerance=epsilon) {
    for (std::size_t i=0;i<3;++i) EXPECT_NEAR(actual[i],expected[i],tolerance);
}

Matrix3x3 expectedEuler(double yaw, double pitch, double roll) {
    const double cy=std::cos(yaw),sy=std::sin(yaw),cp=std::cos(pitch),sp=std::sin(pitch),
                 cr=std::cos(roll),sr=std::sin(roll);
    return {cy*cp,cy*sp*sr-sy*cr,cy*sp*cr+sy*sr,
            sy*cp,sy*sp*sr+cy*cr,sy*sp*cr-cy*sr,
            -sp,cp*sr,cp*cr};
}

constexpr std::array<double,13> angles{
    0,1e-12,1e-7,Pi/4,Pi/2,Pi-1e-7,Pi,Pi+1e-7,-Pi/3,-Pi,2*Pi,3*Pi,7.1};

const std::array<Vec3,6> axes{
    Vec3{1,0,0},Vec3{0,1,0},Vec3{0,0,1},Vec3{0,1,-1},Vec3{-3,2,-1},Vec3{1,1,1}};
}

TEST(RotationMatrixTest, IsRotationChecksOrthogonalityDeterminantAndFiniteness) {
    EXPECT_TRUE(Matrix3x3::Identity().IsRotation());
    EXPECT_TRUE((Matrix3x3{0,-1,0,1,0,0,0,0,1}.IsRotation()));
    EXPECT_FALSE(Matrix3x3{}.IsRotation());
    EXPECT_FALSE((Matrix3x3{1,0,0,0,1,0,0,0,-1}.IsRotation()));
    EXPECT_FALSE((Matrix3x3{1,0.01,0,0,1,0,0,0,1}.IsRotation()));
    Matrix3x3 r=Matrix3x3::Identity();
    r.At(0,0)=1+Tolerance/4;
    EXPECT_TRUE(r.IsRotation());
    r.At(0,0)=1+Tolerance;
    EXPECT_FALSE(r.IsRotation());
    for (double invalid:{std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::quiet_NaN(),1e300}) {
        r=Matrix3x3::Identity();
        r.At(0,0)=invalid;
        EXPECT_FALSE(r.IsRotation());
    }
}

TEST(RotationMatrixTest, RodriguesMatchesKnownRotationsAndNormalizesAxes) {
    const Matrix3x3 z{0,-1,0,1,0,0,0,0,1};
    expectMatrix(Matrix3x3::RotationAxisAngle({0,0,5},Pi/2),z);
    expectMatrix(Matrix3x3::RotationAxisAngle({1,0,0},Pi/2),{1,0,0,0,0,-1,0,1,0});
    expectMatrix(Matrix3x3::RotationAxisAngle({0,1,0},Pi/2),{0,0,1,0,1,0,-1,0,0});
    expectMatrix(Matrix3x3::RotationAxisAngle({1,1,1},2*Pi/3),{0,0,1,1,0,0,0,1,0});
    expectMatrix(Matrix3x3::RotationAxisAngle({0,0,1e300},Pi/2),z);
    expectMatrix(Matrix3x3::RotationAxisAngle({0,0,1e-300},Pi/2),z);
}

TEST(RotationMatrixTest, AxisAngleRoundTripsIncludingZeroAndPi) {
    for (const Vec3& axis:axes)
        for (double angle:angles) {
            SCOPED_TRACE(angle);
            const Matrix3x3 r=Matrix3x3::RotationAxisAngle(axis,angle);
            ASSERT_TRUE(r.IsRotation());
            Vec3 recovered;
            double theta;
            r.ToAxisAngle(recovered,theta);
            EXPECT_NEAR(recovered.Norm(),1,epsilon);
            EXPECT_GE(theta,0);
            EXPECT_LE(theta,Pi);
            expectMatrix(Matrix3x3::RotationAxisAngle(recovered,theta),r);
        }
    Vec3 axis;
    double angle;
    Matrix3x3::Identity().ToAxisAngle(axis,angle);
    EXPECT_DOUBLE_EQ(angle,0);
    expectVector(axis,{1,0,0});
    Matrix3x3::RotationAxisAngle({-3,2,-1},1e-12).ToAxisAngle(axis,angle);
    EXPECT_NEAR(angle,1e-12,1e-20);
}

TEST(RotationMatrixTest, EulerZYXUsesIntrinsicColumnVectorConvention) {
    const double yaw=0.7,pitch=-0.4,roll=1.1;
    expectMatrix(Matrix3x3::FromEulerZYX(yaw,pitch,roll),expectedEuler(yaw,pitch,roll));
    double y,p,r;
    expectedEuler(yaw,pitch,roll).ToEulerZYX(y,p,r);
    EXPECT_NEAR(y,yaw,epsilon);
    EXPECT_NEAR(p,pitch,epsilon);
    EXPECT_NEAR(r,roll,epsilon);
}

TEST(RotationMatrixTest, EulerRoundTripsCanonicalAndNoncanonicalAngles) {
    for (double yaw:{-Pi,-0.7,0.0,Pi,8.0})
        for (double pitch:{-2.0,-Pi/2,-Pi/2+1e-7,0.0,Pi/2-1e-7,Pi/2,2.0})
            for (double roll:{-Pi,-0.3,0.0,1.2,Pi}) {
                const Matrix3x3 expected=expectedEuler(yaw,pitch,roll);
                expectMatrix(Matrix3x3::FromEulerZYX(yaw,pitch,roll),expected);
                double y,p,r;
                expected.ToEulerZYX(y,p,r);
                EXPECT_LE(std::abs(y),Pi);
                EXPECT_LE(std::abs(p),Pi/2);
                EXPECT_LE(std::abs(r),Pi);
                expectMatrix(expectedEuler(y,p,r),expected);
                if (std::abs(pitch)==Pi/2) EXPECT_DOUBLE_EQ(r,0);
            }
}

TEST(RotationMatrixTest, VectorRotationUsesGenericProductAndAccumulatesCounts) {
    const Matrix3x3 r{0,-1,0,1,0,0,0,0,1};
    Operations operations;
    expectVector(r.Rotate({1,2,3},operations),{-2,1,3});
    EXPECT_EQ(operations.multiply,9u);
    EXPECT_TRUE(operations.add==6 || operations.add==9);
    EXPECT_EQ(operations.subtract,0u);
    EXPECT_EQ(operations.divide,0u);
    const auto additions=operations.add;
    r.Rotate({0,0,0},operations);
    EXPECT_EQ(operations.multiply,18u);
    EXPECT_EQ(operations.add,2*additions);
}

TEST(RotationMatrixTest, InvalidRotationInputsAreRejected) {
    const double nan=std::numeric_limits<double>::quiet_NaN();
    const double inf=std::numeric_limits<double>::infinity();
    Vec3 axis;
    double angle,yaw,pitch,roll;
    Operations operations;
    EXPECT_THROW(Matrix3x3::RotationAxisAngle({},0),std::invalid_argument);
    EXPECT_THROW(Matrix3x3::RotationAxisAngle({nan,0,1},1),std::invalid_argument);
    EXPECT_THROW(Matrix3x3::RotationAxisAngle({0,0,1},inf),std::invalid_argument);
    EXPECT_THROW(Matrix3x3::FromEulerZYX(nan,0,0),std::invalid_argument);
    EXPECT_THROW(Matrix3x3::FromEulerZYX(0,inf,0),std::invalid_argument);
    EXPECT_THROW(Matrix3x3::FromEulerZYX(0,0,nan),std::invalid_argument);
    const Matrix3x3 reflection{1,0,0,0,1,0,0,0,-1};
    EXPECT_THROW(reflection.ToAxisAngle(axis,angle),std::invalid_argument);
    EXPECT_THROW(reflection.ToEulerZYX(yaw,pitch,roll),std::invalid_argument);
    EXPECT_THROW(reflection.Rotate({1,2,3},operations),std::invalid_argument);
    EXPECT_THROW(Matrix3x3::Identity().Rotate({0,inf,0},operations),std::invalid_argument);
    EXPECT_EQ(operations.total(),0u);
}

TEST(QuaternionTest, HamiltonProductHasCorrectOrderAndCounts) {
    const Quat i{0,1,0,0},j{0,0,1,0};
    Operations operations;
    const Quat k=i.Multiply(j,operations);
    EXPECT_DOUBLE_EQ(k.s,0);
    EXPECT_DOUBLE_EQ(k.x,0);
    EXPECT_DOUBLE_EQ(k.y,0);
    EXPECT_DOUBLE_EQ(k.z,1);
    EXPECT_EQ(operations.multiply,16u);
    EXPECT_EQ(operations.add,6u);
    EXPECT_EQ(operations.subtract,6u);
    EXPECT_EQ(operations.divide,0u);
    const Quat negative=j.Multiply(i,operations);
    EXPECT_DOUBLE_EQ(negative.z,-1);
    EXPECT_EQ(operations.total(),56u);
    const Quat arbitrary=Quat{1,2,3,4}.Multiply({5,6,7,8},operations);
    EXPECT_DOUBLE_EQ(arbitrary.s,-60);
    EXPECT_DOUBLE_EQ(arbitrary.x,12);
    EXPECT_DOUBLE_EQ(arbitrary.y,30);
    EXPECT_DOUBLE_EQ(arbitrary.z,24);
    const Quat zero=Quat{0,0,0,0}.Multiply({1,2,3,4},operations);
    EXPECT_DOUBLE_EQ(zero.s,0);
    EXPECT_DOUBLE_EQ(zero.z,0);
}

TEST(QuaternionTest, AxisAngleAndMatrixMatchKnownRotations) {
    const Quat q=Quat::FromAxisAngle({0,0,5},Pi/2);
    EXPECT_NEAR(q.s,std::sqrt(0.5),epsilon);
    EXPECT_NEAR(q.z,std::sqrt(0.5),epsilon);
    expectMatrix(q.ToMatrix3x3(),{0,-1,0,1,0,0,0,0,1});
    expectMatrix(Quat{}.ToMatrix3x3(),Matrix3x3::Identity());
    const Quat recovered=Quat::FromMatrix3x3({0,-1,0,1,0,0,0,0,1});
    EXPECT_NEAR(recovered.s,q.s,epsilon);
    EXPECT_NEAR(recovered.z,q.z,epsilon);
}

TEST(QuaternionTest, ConversionsRoundTripAndTreatOppositeSignsEqually) {
    for (const Vec3& axis:axes)
        for (double angle:angles) {
            SCOPED_TRACE(angle);
            const Matrix3x3 r=Matrix3x3::RotationAxisAngle(axis,angle);
            const Quat q=Quat::FromAxisAngle(axis,angle);
            EXPECT_NEAR(std::hypot(q.s,std::hypot(q.x,q.y,q.z)),1,epsilon);
            expectMatrix(q.ToMatrix3x3(),r);
            const Quat recovered=Quat::FromMatrix3x3(r);
            EXPECT_NEAR(std::hypot(recovered.s,std::hypot(recovered.x,recovered.y,recovered.z)),1,epsilon);
            EXPECT_GE(recovered.s,0);
            expectMatrix(recovered.ToMatrix3x3(),r);
            for (double sign:{-1.0,1.0}) {
                const Quat signedQuat{sign*q.s,sign*q.x,sign*q.y,sign*q.z};
                Vec3 u;
                double theta;
                signedQuat.ToAxisAngle(u,theta);
                EXPECT_NEAR(u.Norm(),1,epsilon);
                EXPECT_GE(theta,0);
                EXPECT_LE(theta,Pi);
                expectMatrix(Matrix3x3::RotationAxisAngle(u,theta),r);
                expectMatrix(signedQuat.ToMatrix3x3(),r);
            }
        }
}

TEST(QuaternionTest, MatrixConversionHandlesEveryLargestDiagonalBranch) {
    for (const Matrix3x3& matrix:std::array<Matrix3x3,4>{
            Matrix3x3::Identity(),Matrix3x3{1,0,0,0,-1,0,0,0,-1},
            Matrix3x3{-1,0,0,0,1,0,0,0,-1},Matrix3x3{-1,0,0,0,-1,0,0,0,1}})
        expectMatrix(Quat::FromMatrix3x3(matrix).ToMatrix3x3(),matrix);
}

TEST(QuaternionTest, RotationNormalizesInputsAndAccumulatesCoreCounts) {
    const Quat q{2,0,0,2};
    Operations operations;
    expectVector(q.Rotate({1,2,3},operations),{-2,1,3});
    EXPECT_EQ(operations.multiply,18u);
    EXPECT_EQ(operations.add,6u);
    EXPECT_EQ(operations.subtract,6u);
    EXPECT_EQ(operations.divide,0u);
    expectVector(q.Rotate({0,0,0},operations),{0,0,0});
    EXPECT_EQ(operations.total(),60u);
    for (double scale:{1e-300,1e300}) {
        const Quat extreme{scale,0,0,scale};
        expectVector(extreme.Rotate({1,2,3},operations),{-2,1,3});
        expectMatrix(extreme.ToMatrix3x3(),{0,-1,0,1,0,0,0,0,1});
    }
}

TEST(QuaternionTest, ProductsComposeRotationsRightToLeft) {
    const Quat a=Quat::FromAxisAngle({0,0,1},0.7),b=Quat::FromAxisAngle({1,0,0},-0.4);
    Operations operations;
    const Quat product=a.Multiply(b,operations);
    expectMatrix(product.ToMatrix3x3(),a.ToMatrix3x3().Multiply(b.ToMatrix3x3()));
    const Vec3 v{2,-1,4};
    expectVector(product.Rotate(v,operations),a.Rotate(b.Rotate(v,operations),operations));
}

TEST(QuaternionTest, InvalidInputsAreRejectedWithoutCounting) {
    const double nan=std::numeric_limits<double>::quiet_NaN();
    const double inf=std::numeric_limits<double>::infinity();
    const Quat zero{0,0,0,0},nonfinite{1,0,nan,0};
    Vec3 axis;
    double angle;
    Operations operations;
    EXPECT_THROW(Quat::FromAxisAngle({},0),std::invalid_argument);
    EXPECT_THROW(Quat::FromAxisAngle({0,0,inf},0),std::invalid_argument);
    EXPECT_THROW(Quat::FromAxisAngle({0,0,1},nan),std::invalid_argument);
    EXPECT_THROW(zero.ToAxisAngle(axis,angle),std::invalid_argument);
    EXPECT_THROW(zero.ToMatrix3x3(),std::invalid_argument);
    EXPECT_THROW(zero.Rotate({1,2,3},operations),std::invalid_argument);
    EXPECT_THROW(nonfinite.ToMatrix3x3(),std::invalid_argument);
    EXPECT_THROW(nonfinite.ToAxisAngle(axis,angle),std::invalid_argument);
    EXPECT_THROW(nonfinite.Multiply({},operations),std::invalid_argument);
    EXPECT_THROW(Quat{}.Multiply(nonfinite,operations),std::invalid_argument);
    EXPECT_THROW(Quat{}.Rotate({nan,0,0},operations),std::invalid_argument);
    EXPECT_THROW(Quat::FromMatrix3x3({1,0,0,0,1,0,0,0,-1}),std::invalid_argument);
    EXPECT_EQ(operations.total(),0u);
}
