#include "Geometry.hpp"
#include "Matrix3x3.hpp"
#include "Quat.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

// Supplied fixed-size storage and geometry utilities, shared by assignment and solution.
Vec3::Vec3(double x, double y, double z) : vector_{x,y,z} {}
Vec3::Vec3(const Vector& vector) : vector_(vector) {
    if (vector.size() != 3) throw std::invalid_argument("Vec3 requires three components");
}
double& Vec3::operator[](size_type index) {
    if (index >= 3) throw std::out_of_range("Vector index out of range");
    return vector_[index];
}
double Vec3::operator[](size_type index) const {
    if (index >= 3) throw std::out_of_range("Vector index out of range");
    return vector_[index];
}
Vec3& Vec3::operator+=(const Vec3& other) {
    vector_+=other.vector_;
    return *this;
}
Vec3& Vec3::operator-=(const Vec3& other) {
    vector_-=other.vector_;
    return *this;
}
Vec3& Vec3::operator*=(double scalar) {
    vector_*=scalar;
    return *this;
}
const Vector& Vec3::Algebra() const noexcept { return vector_; }

Vec3 operator+(const Vec3& vector) { return Vec3(+vector.Algebra()); }
Vec3 operator-(const Vec3& vector) { return Vec3(-vector.Algebra()); }
Vec3 operator+(const Vec3& left, const Vec3& right) {
    return Vec3(left.Algebra()+right.Algebra());
}
Vec3 operator-(const Vec3& left, const Vec3& right) {
    return Vec3(left.Algebra()-right.Algebra());
}
Vec3 operator*(const Vec3& vector, double scalar) { return Vec3(vector.Algebra()*scalar); }
Vec3 operator*(double scalar, const Vec3& vector) { return vector*scalar; }
double operator*(const Vec3& left, const Vec3& right) { return left.Algebra()*right.Algebra(); }

bool isFinite(const Vec3& v) {
    return std::isfinite(v[0]) && std::isfinite(v[1]) && std::isfinite(v[2]);
}
double Vec3::Dot(const Vec3& a, const Vec3& b) { return a*b; }
Vec3 Vec3::Cross(const Vec3& a, const Vec3& b) {
    return {a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]};
}
double Vec3::Norm() const { return norm(vector_); }
Vec3 Vec3::Normalize() const {
    if (!isFinite(*this)) throw std::invalid_argument("Non-finite vector");
    const double scale = std::max({std::abs(vector_[0]),std::abs(vector_[1]),std::abs(vector_[2])});
    if (scale == 0) throw std::invalid_argument("Cannot normalize the zero vector");
    // Scale before measuring length to avoid overflow or underflow in normalization.
    const Vec3 v{vector_[0]/scale,vector_[1]/scale,vector_[2]/scale};
    const double n = v.Norm();
    return v*(1/n);
}
Quat Quat::Normalized() const {
    if (!std::isfinite(s) || !isFinite({x,y,z}))
        throw std::invalid_argument("Non-finite quaternion");
    const double scale = std::max({std::abs(s),std::abs(x),std::abs(y),std::abs(z)});
    if (scale == 0) throw std::invalid_argument("Cannot normalize the zero quaternion");
    // The scaled components are bounded by one, so the squared norm is safe.
    Quat q{s/scale,x/scale,y/scale,z/scale};
    const double n = std::sqrt(q.s*q.s+q.x*q.x+q.y*q.y+q.z*q.z);
    return {q.s/n,q.x/n,q.y/n,q.z/n};
}
Matrix3x3::Matrix3x3() : matrix_(3,3) {}
Matrix3x3::Matrix3x3(std::initializer_list<double> values) : matrix_(3,3) {
    if (values.size() != 9) throw std::invalid_argument("Matrix3x3 requires nine entries");
    matrix_.values = Vector(values);
}
Matrix3x3::Matrix3x3(const Matrix& matrix) : matrix_(matrix) {
    if (matrix.rows != 3 || matrix.cols != 3 || matrix.values.size() != 9)
        throw std::invalid_argument("Matrix3x3 requires a 3x3 matrix");
}
const Matrix& Matrix3x3::Algebra() const noexcept { return matrix_; }
double& Matrix3x3::At(std::size_t row, std::size_t col) { return matrix_(row,col); }
double Matrix3x3::At(std::size_t row, std::size_t col) const {
    return matrix_(row,col);
}
Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& other) {
    matrix_+=other.matrix_;
    return *this;
}
Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& other) {
    matrix_-=other.matrix_;
    return *this;
}
Matrix3x3& Matrix3x3::operator*=(double scalar) {
    matrix_*=scalar;
    return *this;
}
Matrix3x3 Matrix3x3::Identity() { return Matrix3x3(Matrix::identity(3)); }
Vec3 Matrix3x3::Multiply(const Vec3& v) const {
    // Reuse lab 1's general matrix-vector product, then retain the fixed-size interface.
    return Vec3(matrix_*v.Algebra());
}
Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& b) const {
    return Matrix3x3(matrix_*b.Algebra());
}
double Matrix3x3::Det() const {
    // Cofactor expansion along the first row; generic Matrix has no determinant method.
    return At(0,0)*(At(1,1)*At(2,2)-At(1,2)*At(2,1))
        -At(0,1)*(At(1,0)*At(2,2)-At(1,2)*At(2,0))
        +At(0,2)*(At(1,0)*At(2,1)-At(1,1)*At(2,0));
}
Matrix3x3 Matrix3x3::Transposed() const {
    Matrix3x3 result;
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) result.At(i,j)=At(j,i);
    return result;
}
double Matrix3x3::Trace() const { return At(0,0)+At(1,1)+At(2,2); }

Matrix3x3 operator+(const Matrix3x3& matrix) { return Matrix3x3(+matrix.Algebra()); }
Matrix3x3 operator-(const Matrix3x3& matrix) { return Matrix3x3(-matrix.Algebra()); }
Matrix3x3 operator+(const Matrix3x3& left, const Matrix3x3& right) {
    return Matrix3x3(left.Algebra()+right.Algebra());
}
Matrix3x3 operator-(const Matrix3x3& left, const Matrix3x3& right) {
    return Matrix3x3(left.Algebra()-right.Algebra());
}
Matrix3x3 operator*(const Matrix3x3& matrix, double scalar) {
    return Matrix3x3(matrix.Algebra()*scalar);
}
Matrix3x3 operator*(double scalar, const Matrix3x3& matrix) { return matrix*scalar; }
Vec3 operator*(const Matrix3x3& matrix, const Vec3& vector) { return matrix.Multiply(vector); }
Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right) { return left.Multiply(right); }
