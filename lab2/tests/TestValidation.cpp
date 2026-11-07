#include "Validation.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>

class ValidationTest : public testing::TestWithParam<Task> {};

TEST_P(ValidationTest, AllIncludedExamplesMeetTheAppContract) {
    const Task task=GetParam();
    for (int example=0;example<9;++example) {
        SCOPED_TRACE(example);
        const Report report=runTask(task,exampleInput(example));
        EXPECT_TRUE(report.correct) << report.detail;
        const bool invalidMatrix=example==7 && (task==Task::MatrixAxis || task==Task::MatrixQuat
            || task==Task::MatrixEuler || task==Task::MatrixVector);
        const bool invalidZero=example==8 && (task==Task::AxisMatrix || task==Task::AxisQuat
            || task==Task::QuatAxis || task==Task::QuatMatrix || task==Task::QuatVector);
        EXPECT_EQ(report.completed,!(invalidMatrix || invalidZero));
        if (report.completed) {
            EXPECT_TRUE(std::isfinite(report.error));
            EXPECT_LE(report.error,5e-6);
            EXPECT_GE(report.milliseconds,0);
            const bool counted=task==Task::QuatProduct || task==Task::MatrixVector || task==Task::QuatVector;
            EXPECT_EQ(report.countsApplicable,counted);
            if (counted) EXPECT_TRUE(report.countsExpected);
        } else {
            EXPECT_FALSE(report.detail.empty());
        }
    }
}

TEST_P(ValidationTest, RejectsNonfiniteInputsOrReportsNonrotation) {
    const Task task=GetParam();
    Input input=exampleInput(0);
    const double nan=std::numeric_limits<double>::quiet_NaN();
    switch (task) {
    case Task::AxisMatrix: case Task::AxisQuat: input.angle=nan; break;
    case Task::MatrixAxis: case Task::MatrixQuat: case Task::MatrixEuler:
    case Task::MatrixVector: case Task::IsRotation: input.matrix.At(1,2)=nan; break;
    case Task::EulerMatrix: input.roll=nan; break;
    case Task::QuatProduct: input.second.s=nan; break;
    case Task::QuatAxis: case Task::QuatMatrix: case Task::QuatVector: input.first.x=nan; break;
    }
    const Report report=runTask(task,input);
    EXPECT_TRUE(report.correct) << report.detail;
    EXPECT_EQ(report.completed,task==Task::IsRotation);
    if (task==Task::IsRotation) EXPECT_FALSE(report.rotation);
    else EXPECT_NE(report.detail.find("Invalid input rejected:"),std::string::npos);
}

INSTANTIATE_TEST_SUITE_P(Operations,ValidationTest,testing::Values(
    Task::IsRotation,Task::AxisMatrix,Task::AxisQuat,Task::MatrixAxis,
    Task::MatrixQuat,Task::MatrixEuler,Task::EulerMatrix,Task::QuatProduct,
    Task::QuatAxis,Task::QuatMatrix,Task::MatrixVector,Task::QuatVector));
