#include "Validation.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <stdexcept>

namespace {

// Independent reference calculations must not call the students' rotation methods.
// Apply R to a column vector directly, without recording arithmetic counts.
Vec3 apply(const Matrix3x3& r, const Vec3& v) {
    return {r.At(0,0)*v[0]+r.At(0,1)*v[1]+r.At(0,2)*v[2],
            r.At(1,0)*v[0]+r.At(1,1)*v[1]+r.At(1,2)*v[2],
            r.At(2,0)*v[0]+r.At(2,1)*v[1]+r.At(2,2)*v[2]};
}
// Build a reference matrix by applying Rodrigues' vector formula to each basis vector.
Matrix3x3 axisMatrix(const Vec3& axis, double angle) {
    const Vec3 u=axis.Normalize();
    Matrix3x3 r;

    for (int j=0;j<3;++j) {
        const Vec3 v{j==0 ? 1.0 : 0.0,j==1 ? 1.0 : 0.0,j==2 ? 1.0 : 0.0};
        const Vec3 cross=Vec3::Cross(u,v);
        const double projection=Vec3::Dot(u,v)*(1-std::cos(angle));
        r.At(0,j)=v[0]*std::cos(angle)+cross[0]*std::sin(angle)+u[0]*projection;
        r.At(1,j)=v[1]*std::cos(angle)+cross[1]*std::sin(angle)+u[1]*projection;
        r.At(2,j)=v[2]*std::cos(angle)+cross[2]*std::sin(angle)+u[2]*projection;
    }
    return r;
}
// Form A*B by transforming B's columns with A.
Matrix3x3 product(const Matrix3x3& a, const Matrix3x3& b) {
    Matrix3x3 r;
    for (int j=0;j<3;++j) {
        const Vec3 c=apply(a,{b.At(0,j),b.At(1,j),b.At(2,j)});
        r.At(0,j)=c[0]; r.At(1,j)=c[1]; r.At(2,j)=c[2];
    }
    return r;
}
// Reference intrinsic ZYX rotation: Rz(yaw)*Ry(pitch)*Rx(roll).
Matrix3x3 eulerMatrix(double yaw, double pitch, double roll) {
    return product(product(axisMatrix({0,0,1},yaw),axisMatrix({0,1,0},pitch)),
                   axisMatrix({1,0,0},roll));
}
// Hamilton product using scalar/vector parts; does not normalize either operand.
Quat hamilton(const Quat& a, const Quat& b) {
    const Vec3 u{a.x,a.y,a.z}, v{b.x,b.y,b.z}, cross=Vec3::Cross(u,v);
    return {a.s*b.s-Vec3::Dot(u,v),a.s*v[0]+b.s*u[0]+cross[0],
            a.s*v[1]+b.s*u[1]+cross[1],a.s*v[2]+b.s*u[2]+cross[2]};
}
// Reference quaternion rotation q*(0,v)*conjugate(q), after normalizing q.
Vec3 quatVector(const Quat& input, const Vec3& v) {
    const Quat q=input.Normalized(), conjugate{q.s,-q.x,-q.y,-q.z};
    const Quat r=hamilton(hamilton(q,{0,v[0],v[1],v[2]}),conjugate);
    return {r.x,r.y,r.z};
}
// Rotated basis vectors become the columns of the equivalent rotation matrix.
Matrix3x3 quatMatrix(const Quat& q) {
    const Vec3 x=quatVector(q,{1,0,0}),y=quatVector(q,{0,1,0}),z=quatVector(q,{0,0,1});
    return {x[0],y[0],z[0],x[1],y[1],z[1],x[2],y[2],z[2]};
}
// Check finite, orthonormal columns and positive orientation independently of IsRotation.
bool rotation(const Matrix3x3& r) {
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) if (!std::isfinite(r.At(i,j))) return false;
    const Vec3 a{r.At(0,0),r.At(1,0),r.At(2,0)},b{r.At(0,1),r.At(1,1),r.At(2,1)},
               c{r.At(0,2),r.At(1,2),r.At(2,2)};
    return std::abs(Vec3::Dot(a,a)-1)<=Tolerance && std::abs(Vec3::Dot(b,b)-1)<=Tolerance
        && std::abs(Vec3::Dot(c,c)-1)<=Tolerance && std::abs(Vec3::Dot(a,b))<=Tolerance
        && std::abs(Vec3::Dot(a,c))<=Tolerance && std::abs(Vec3::Dot(b,c))<=Tolerance
        && std::abs(Vec3::Dot(a,Vec3::Cross(b,c))-1)<=Tolerance;
}
// Non-finite outputs receive infinite error, so NaN cannot accidentally pass validation.
double difference(double a, double b) {
    return std::isfinite(a) && std::isfinite(b) ? std::abs(a-b) : INFINITY;
}
// Maximum component error, relative to the expected vector length when that exceeds one.
double error(const Vec3& a, const Vec3& b) {
    return std::max({difference(a[0],b[0]),difference(a[1],b[1]),difference(a[2],b[2])})
        /std::max(1.0,b.Norm());
}
// Maximum absolute entry error; rotation matrix entries need no magnitude scaling.
double error(const Matrix3x3& a, const Matrix3x3& b) {
    double result=0;
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j) result=std::max(result,difference(a.At(i,j),b.At(i,j)));
    return result;
}
// Products allow zero quaternions; rotation/conversion operations require a nonzero one.
bool validQuat(const Quat& q, bool nonzero) {
    return std::isfinite(q.s) && isFinite({q.x,q.y,q.z})
        && (!nonzero || q.s!=0 || q.x!=0 || q.y!=0 || q.z!=0);
}
// Determine whether this operation must accept the input or throw std::invalid_argument.
bool validInput(Task task, const Input& in) {
    switch (task) {
    case Task::AxisMatrix: case Task::AxisQuat:
        return isFinite(in.axis) && (in.axis[0]!=0 || in.axis[1]!=0 || in.axis[2]!=0) && std::isfinite(in.angle);
    case Task::MatrixAxis: case Task::MatrixQuat: case Task::MatrixEuler: return rotation(in.matrix);
    case Task::MatrixVector: return rotation(in.matrix) && isFinite(in.vector);
    case Task::EulerMatrix: return std::isfinite(in.yaw) && std::isfinite(in.pitch) && std::isfinite(in.roll);
    case Task::QuatProduct: return validQuat(in.first,false) && validQuat(in.second,false);
    case Task::QuatAxis: case Task::QuatMatrix: return validQuat(in.first,true);
    case Task::QuatVector: return validQuat(in.first,true) && isFinite(in.vector);
    case Task::IsRotation: return true;
    }
    return false;
}
}

Report runTask(Task task, const Input& in) {
    Report r;
    const bool valid=validInput(task,in);
    const auto start=std::chrono::steady_clock::now();
    try {
        switch (task) {
        case Task::AxisMatrix: r.matrix=Matrix3x3::RotationAxisAngle(in.axis,in.angle); break;
        case Task::AxisQuat: r.quat=Quat::FromAxisAngle(in.axis,in.angle); break;
        case Task::MatrixAxis: in.matrix.ToAxisAngle(r.axis,r.angle); break;
        case Task::MatrixQuat: r.quat=Quat::FromMatrix3x3(in.matrix); break;
        case Task::MatrixEuler: in.matrix.ToEulerZYX(r.yaw,r.pitch,r.roll); break;
        case Task::EulerMatrix: r.matrix=Matrix3x3::FromEulerZYX(in.yaw,in.pitch,in.roll); break;
        case Task::QuatProduct: r.quat=in.first.Multiply(in.second,r.operations); break;
        case Task::QuatAxis: in.first.ToAxisAngle(r.axis,r.angle); break;
        case Task::QuatMatrix: r.matrix=in.first.ToMatrix3x3(); break;
        case Task::MatrixVector: r.vector=in.matrix.Rotate(in.vector,r.operations); break;
        case Task::QuatVector: r.vector=in.first.Rotate(in.vector,r.operations); break;
        case Task::IsRotation: r.rotation=in.matrix.IsRotation(); break;
        }
    } catch (const std::invalid_argument& exception) {
        r.correct=!valid;
        r.detail=std::string(r.correct ? "Invalid input rejected: " : "Unexpected error: ")+exception.what();
        return r;
    }
    r.milliseconds=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count();
    r.completed=true;
    if (!valid) { r.detail="This input must be rejected with std::invalid_argument."; return r; }

    // Reconstruct equivalent rotations instead of comparing non-unique axes or Euler angles.
    switch (task) {
    case Task::AxisMatrix: r.error=error(r.matrix,axisMatrix(in.axis,in.angle)); break;
    case Task::AxisQuat: case Task::MatrixQuat: {
        const double n=std::hypot(r.quat.s,std::hypot(r.quat.x,r.quat.y,r.quat.z));
        if (!validQuat(r.quat,true) || difference(n,1)>Tolerance) r.error=INFINITY;
        else r.error=error(quatMatrix(r.quat),task==Task::AxisQuat ? axisMatrix(in.axis,in.angle) : in.matrix);
        break;
    }
    case Task::MatrixAxis: case Task::QuatAxis:
        if (!isFinite(r.axis) || difference(r.axis.Norm(),1)>Tolerance || !std::isfinite(r.angle)
            || r.angle<0 || r.angle>Pi+1e-12) r.error=INFINITY;
        else r.error=error(axisMatrix(r.axis,r.angle),task==Task::MatrixAxis ? in.matrix : quatMatrix(in.first));
        break;
    case Task::MatrixEuler:
        if (!std::isfinite(r.yaw) || !std::isfinite(r.pitch) || !std::isfinite(r.roll)
            || std::abs(r.pitch)>Pi/2+1e-12 || std::abs(r.yaw)>Pi+1e-12 || std::abs(r.roll)>Pi+1e-12)
            r.error=INFINITY;
        else r.error=error(eulerMatrix(r.yaw,r.pitch,r.roll),in.matrix);
        break;
    case Task::EulerMatrix: r.error=error(r.matrix,eulerMatrix(in.yaw,in.pitch,in.roll)); break;
    case Task::QuatProduct: {
        const Quat expected=hamilton(in.first,in.second);
        const double scale=std::max({1.0,std::abs(expected.s),std::abs(expected.x),std::abs(expected.y),std::abs(expected.z)});
        r.error=std::max({difference(r.quat.s,expected.s),difference(r.quat.x,expected.x),
                         difference(r.quat.y,expected.y),difference(r.quat.z,expected.z)})/scale;
        break;
    }
    case Task::QuatMatrix: r.error=error(r.matrix,quatMatrix(in.first)); break;
    case Task::MatrixVector: r.error=error(r.vector,apply(in.matrix,in.vector)); break;
    case Task::QuatVector: r.error=error(r.vector,quatVector(in.first,in.vector)); break;
    case Task::IsRotation: r.error=r.rotation==rotation(in.matrix) ? 0 : 1; break;
    }
    r.correct=std::isfinite(r.error) && r.error<=5e-6;
    r.detail=r.correct ? "" : "Review the result and edge cases.";
    r.countsApplicable=task==Task::QuatProduct || task==Task::MatrixVector || task==Task::QuatVector;
    if (r.countsApplicable) {
        // Standard kernel costs are advisory and do not change the numerical correctness flag.
        const auto& op=r.operations;
        if (task==Task::MatrixVector)
            r.countsExpected=op.multiply==9 && (op.add==6 || op.add==9) && op.subtract==0 && op.divide==0;
        else if (task==Task::QuatProduct)
            r.countsExpected=op.multiply==16 && op.add==6 && op.subtract==6 && op.divide==0;
        else r.countsExpected=op.multiply==18 && op.add==6 && op.subtract==6 && op.divide==0;
    }
    return r;
}

// Construct valid representations first, then invalidate only the fields needed by examples 7/8.
Input exampleInput(int example) {
    Input in;
    if (example==1) in.angle=0;
    if (example==2) { in.axis={0,1,-1}; in.angle=Pi; }
    if (example==3) { in.axis={1,-2,3}; in.angle=Pi-1e-7; }
    if (example==4 || example==5) in.pitch=example==4 ? Pi/2 : -Pi/2;
    if (example==6) in.pitch=Pi/2-1e-7;
    in.matrix=example>=4 && example<=6 ? eulerMatrix(in.yaw,in.pitch,in.roll) : axisMatrix(in.axis,in.angle);
    const Vec3 axis=in.axis.Normalize();
    in.first={std::cos(in.angle/2),axis[0]*std::sin(in.angle/2),axis[1]*std::sin(in.angle/2),axis[2]*std::sin(in.angle/2)};
    in.second={0.5,0.5,-0.5,0.5};
    if (example==7) in.matrix={1,0,0,0,1,0,0,0,-1};
    if (example==8) { in.axis={}; in.first={0,0,0,0}; }
    return in;
}
