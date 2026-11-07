#include "Quat.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>

static_assert(std::is_same_v<decltype(std::declval<const Vec3&>().Algebra()),const Vector&>);
static_assert(std::is_same_v<decltype(std::declval<const Matrix3x3&>().Algebra()),const Matrix&>);
static_assert(std::is_same_v<decltype(2*std::declval<Matrix3x3>()+3*std::declval<Matrix3x3>()),Matrix3x3>);
static_assert(std::is_same_v<decltype(2*std::declval<Vec3>()+3*std::declval<Vec3>()),Vec3>);
static_assert(std::is_same_v<decltype(std::declval<Matrix3x3>()*std::declval<Vec3>()),Vec3>);
static_assert(std::is_same_v<decltype(std::declval<Matrix3x3>()*std::declval<Matrix3x3>()),Matrix3x3>);
static_assert(std::is_same_v<decltype(std::declval<Vec3>()*std::declval<Vec3>()),double>);

TEST(GeometryTest, FixedSizeVectorsUseGenericAlgebra) {
    const Vec3 zero;
    EXPECT_EQ(zero.Algebra().size(),3u);
    EXPECT_DOUBLE_EQ(zero.Norm(),0);
    const Vec3 a{1,2,3}, b{4,-1,0.5};
    const Vec3 sum=a+b;
    EXPECT_DOUBLE_EQ(sum[0],5);
    EXPECT_DOUBLE_EQ(sum[1],1);
    EXPECT_DOUBLE_EQ(sum[2],3.5);
    EXPECT_DOUBLE_EQ(Vec3::Dot(a,b),3.5);
    const Vec3 cross=Vec3::Cross({1,0,0},{0,1,0});
    EXPECT_DOUBLE_EQ(cross[2],1);
    EXPECT_DOUBLE_EQ(Vec3::Dot(cross,{1,0,0}),0);
    EXPECT_THROW(Vec3(Vector(2)),std::invalid_argument);
    EXPECT_THROW(Vec3(Vector(4)),std::invalid_argument);
    Vec3 editable=a;
    editable[0]=7;
    EXPECT_DOUBLE_EQ(editable.Algebra()[0],7);
    EXPECT_DOUBLE_EQ(a[0],1);
    EXPECT_THROW(editable[3],std::out_of_range);
    EXPECT_THROW(a[3],std::out_of_range);
}

TEST(GeometryTest, FixedSizeMatricesUseGenericAlgebra) {
    const Matrix3x3 zero;
    EXPECT_EQ(zero.Algebra().rows,3u);
    EXPECT_EQ(zero.Algebra().cols,3u);
    EXPECT_EQ(zero.Algebra().values.size(),9u);
    EXPECT_DOUBLE_EQ(zero.Trace(),0);
    const Matrix3x3 a{1,2,3,4,5,6,7,8,9};
    const Matrix3x3 identity=Matrix3x3::Identity();
    const Matrix3x3 product=a.Multiply(identity);
    const Matrix3x3 generic(a.Algebra()*identity.Algebra());
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) {
            EXPECT_DOUBLE_EQ(product.At(i,j),a.At(i,j));
            EXPECT_DOUBLE_EQ(generic.At(i,j),a.At(i,j));
            EXPECT_DOUBLE_EQ(a.Transposed().At(i,j),a.At(j,i));
            EXPECT_DOUBLE_EQ(zero.At(i,j),0);
        }
    const Vec3 result=a.Multiply(Vec3{1,2,3});
    EXPECT_DOUBLE_EQ(result[0],14);
    EXPECT_DOUBLE_EQ(result[1],32);
    EXPECT_DOUBLE_EQ(result[2],50);
    EXPECT_DOUBLE_EQ(a.Det(),0);
    EXPECT_DOUBLE_EQ(a.Trace(),15);
    EXPECT_DOUBLE_EQ(identity.Det(),1);
    EXPECT_THROW(Matrix3x3(Matrix(2,3)),std::invalid_argument);
    EXPECT_THROW(Matrix3x3(Matrix(3,4)),std::invalid_argument);
    Matrix malformed(3,3);
    malformed.values.resize(8);
    EXPECT_THROW(Matrix3x3{malformed},std::invalid_argument);
    EXPECT_THROW((Matrix3x3{1,2,3}),std::invalid_argument);
    Matrix3x3 editable=a;
    editable.At(0,0)=42;
    EXPECT_DOUBLE_EQ(editable.Algebra()(0,0),42);
    EXPECT_DOUBLE_EQ(a.At(0,0),1);
    EXPECT_THROW(editable.At(3,0),std::out_of_range);
    EXPECT_THROW(a.At(0,3),std::out_of_range);
}

TEST(GeometryTest, NaturalVectorOperatorsPreserveValuesAndTypes) {
    const Vec3 a{1,2,3},b{4,-1,0.5};
    const Vec3 combined=2*a+3*b;
    EXPECT_DOUBLE_EQ(combined[0],14);
    EXPECT_DOUBLE_EQ(combined[1],1);
    EXPECT_DOUBLE_EQ(combined[2],7.5);
    const Vec3 difference=a-b,positive=+a,negative=-a,rightScaled=a*2;
    for (std::size_t i=0;i<3;++i) {
        EXPECT_DOUBLE_EQ(difference[i],a[i]-b[i]);
        EXPECT_DOUBLE_EQ(positive[i],a[i]);
        EXPECT_DOUBLE_EQ(negative[i],-a[i]);
        EXPECT_DOUBLE_EQ(rightScaled[i],2*a[i]);
    }
    EXPECT_DOUBLE_EQ(a*b,3.5);
    Vec3 compound=a;
    EXPECT_EQ(&(compound+=b),&compound);
    EXPECT_EQ(&(compound-=b),&compound);
    EXPECT_EQ(&(compound*=2),&compound);
    for (std::size_t i=0;i<3;++i) EXPECT_DOUBLE_EQ(compound[i],2*a[i]);
    compound-=compound;
    EXPECT_DOUBLE_EQ(compound.Norm(),0);
    EXPECT_DOUBLE_EQ(a[0],1);
    EXPECT_DOUBLE_EQ(b[0],4);
}

TEST(GeometryTest, NaturalMatrixExpressionsReuseGenericAlgebra) {
    const Matrix3x3 A{1,2,3,4,5,6,7,8,9},B{2,0,0,0,3,0,0,0,4};
    const Matrix3x3 C=2*A+3*B;
    const Matrix3x3 difference=A-B,positive=+A,negative=-A,rightScaled=A*2;
    const Matrix3x3 product=A*B,reverse=B*A;
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) {
            EXPECT_DOUBLE_EQ(C.At(i,j),2*A.At(i,j)+3*B.At(i,j));
            EXPECT_DOUBLE_EQ(difference.At(i,j),A.At(i,j)-B.At(i,j));
            EXPECT_DOUBLE_EQ(positive.At(i,j),A.At(i,j));
            EXPECT_DOUBLE_EQ(negative.At(i,j),-A.At(i,j));
            EXPECT_DOUBLE_EQ(rightScaled.At(i,j),2*A.At(i,j));
            EXPECT_DOUBLE_EQ(product.At(i,j),A.At(i,j)*B.At(j,j));
            EXPECT_DOUBLE_EQ(reverse.At(i,j),B.At(i,i)*A.At(i,j));
        }
    const Vec3 v{1,2,3};
    const Vec3 w=(2*A+3*B)*v;
    EXPECT_DOUBLE_EQ(w[0],34);
    EXPECT_DOUBLE_EQ(w[1],82);
    EXPECT_DOUBLE_EQ(w[2],136);
    const Vec3 polynomial=(Matrix3x3::Identity()+A+0.5*A*A)*v;
    EXPECT_DOUBLE_EQ(polynomial[0],129);
    EXPECT_DOUBLE_EQ(polynomial[1],292);
    EXPECT_DOUBLE_EQ(polynomial[2],455);
    Matrix3x3 compound=A;
    EXPECT_EQ(&(compound+=B),&compound);
    EXPECT_EQ(&(compound-=B),&compound);
    EXPECT_EQ(&(compound*=2),&compound);
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) EXPECT_DOUBLE_EQ(compound.At(i,j),2*A.At(i,j));
    compound-=compound;
    EXPECT_DOUBLE_EQ(compound.Trace(),0);
    EXPECT_DOUBLE_EQ(A.At(0,0),1);
    EXPECT_DOUBLE_EQ(B.At(0,0),2);
}

TEST(GeometryTest, FixedSizeStorageRemainsValidAfterRvalueCopies) {
    Vec3 a{1,2,3};
    Vec3 b=std::move(a);
    EXPECT_EQ(a.Algebra().size(),3u);
    EXPECT_DOUBLE_EQ(b[2],3);
    b=std::move(a);
    EXPECT_DOUBLE_EQ(a[0],1);
    Matrix3x3 m=Matrix3x3::Identity();
    Matrix3x3 n=std::move(m);
    EXPECT_EQ(m.Algebra().values.size(),9u);
    EXPECT_DOUBLE_EQ(n.Det(),1);
    n=std::move(m);
    EXPECT_DOUBLE_EQ(m.Det(),1);
}

TEST(GeometryTest, SuppliedNumericsSupportGenericMatrices) {
    Matrix a(2,2);
    a.values={1,2,3,4};
    a.swapRows(0,1);
    EXPECT_DOUBLE_EQ(a(0,0),3);
    EXPECT_DOUBLE_EQ(a(1,1),2);
    EXPECT_THROW(a.swapRows(0,2),std::out_of_range);
    EXPECT_THROW(Matrix(std::numeric_limits<std::size_t>::max(),2),std::length_error);
    EXPECT_DOUBLE_EQ(norm(Vector{3,4}),5);
    const Matrix b=a*Matrix::identity(2);
    EXPECT_DOUBLE_EQ(b(0,1),4);
    EXPECT_THROW(a*Vector(3),std::invalid_argument);
    EXPECT_THROW(a*Matrix(3,2),std::invalid_argument);
}

TEST(GeometryTest, NormalizationIsStableForExtremeMagnitudes) {
    for (double scale:{1e-300,1.0,1e300,std::numeric_limits<double>::max()}) {
        const Vec3 v=Vec3{scale,-scale,scale}.Normalize();
        EXPECT_NEAR(v.Norm(),1,1e-14);
        EXPECT_NEAR(v[0],1/std::sqrt(3.0),1e-14);
        const Quat q=Quat{scale,-scale,scale,-scale}.Normalized();
        EXPECT_NEAR(q.s,0.5,1e-14);
        EXPECT_NEAR(q.x,-0.5,1e-14);
    }
    const double nan=std::numeric_limits<double>::quiet_NaN();
    const double inf=std::numeric_limits<double>::infinity();
    EXPECT_THROW(Vec3{}.Normalize(),std::invalid_argument);
    EXPECT_THROW((Vec3{nan,0,0}.Normalize()),std::invalid_argument);
    EXPECT_THROW((Vec3{0,inf,0}.Normalize()),std::invalid_argument);
    EXPECT_THROW((Quat{0,0,0,0}.Normalized()),std::invalid_argument);
    EXPECT_THROW((Quat{inf,0,0,0}.Normalized()),std::invalid_argument);
    EXPECT_THROW((Quat{1,0,nan,0}.Normalized()),std::invalid_argument);
}
