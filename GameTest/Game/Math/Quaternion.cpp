#include "stdafx.h"
#include "Quaternion.h"
#include "App/AppSettings.h"
#include "Game/Math/MathUtil.h"

// Constructors

Quaternion::Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f)
{
}

Quaternion::Quaternion(const FVector3 &eulerAnglesXYZ)
{
    // Assuming eulerAnglesXYZ contains angles in degrees
    float halfX = eulerAnglesXYZ.x * 0.5f;
    float halfY = eulerAnglesXYZ.y * 0.5f;
    float halfZ = eulerAnglesXYZ.z * 0.5f;

    float cosX = std::cos(halfX);
    float sinX = std::sin(halfX);
    float cosY = std::cos(halfY);
    float sinY = std::sin(halfY);
    float cosZ = std::cos(halfZ);
    float sinZ = std::sin(halfZ);

    // Quaternion multiplication: q = qx * qy * qz
    w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    x = sinX * cosY * cosZ - cosX * sinY * sinZ;
    y = cosX * sinY * cosZ + sinX * cosY * sinZ;
    z = cosX * cosY * sinZ - sinX * sinY * cosZ;
}


Quaternion::Quaternion(float pitch, float yaw, float roll)
{
    *this = Quaternion(FVector3(pitch, yaw, roll));
}


Quaternion::Quaternion(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_)
{
}

Quaternion::Quaternion(const Quaternion &copy) : w(copy.w), x(copy.x), y(copy.y), z(copy.z)
{
}

// Static Utility Functions

Quaternion Quaternion::Slerp(const Quaternion &start, const Quaternion &end, float t)
{
    // Normalize both quaternions to ensure accurate interpolation
    Quaternion q1 = start.Normalize();
    Quaternion q2 = end.Normalize();

    // Compute the cosine of the angle between the two quaternions
    float dot = q1.Dot(q2);

    // If the dot product is negative, the quaternions have opposite handed-ness and Slerp won't take the shorter path.
    // Fix by reversing one quaternion
    if (dot < 0.0f)
    {
        q2 = Quaternion(-q2.w, -q2.x, -q2.y, -q2.z);
        dot = -dot;
    }

    const float DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD)
    {
        // Quaternions are very close, use linear interpolation
        Quaternion result = Quaternion(q1.w + t * (q2.w - q1.w), q1.x + t * (q2.x - q1.x), q1.y + t * (q2.y - q1.y),
                                       q1.z + t * (q2.z - q1.z));
        return result.Normalize();
    }

    // Calculate the angle between the quaternions
    float theta_0 = std::acos(dot);        // Angle between input quaternions
    float theta = theta_0 * t;             // Angle between q1 and result
    float sin_theta = std::sin(theta);     // Compute once
    float sin_theta_0 = std::sin(theta_0); // Compute once

    float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    return Quaternion((q1.w * s0) + (q2.w * s1), (q1.x * s0) + (q2.x * s1), (q1.y * s0) + (q2.y * s1),
                      (q1.z * s0) + (q2.z * s1));
}


Quaternion Quaternion::LookAtPlusZ(const FVector3 &direction, const FVector3 &up)
{
    FVector3 forward = direction.Normalize();
    FVector3 upDir = up.Normalize();

    FVector3 right = upDir.Cross(forward).Normalize();
    FVector3 trueUp = forward.Cross(right);

    Matrix3 rotationMatrix;
    rotationMatrix.Set(0, 0, right.x);
    rotationMatrix.Set(1, 0, right.y);
    rotationMatrix.Set(2, 0, right.z);

    rotationMatrix.Set(0, 1, trueUp.x);
    rotationMatrix.Set(1, 1, trueUp.y);
    rotationMatrix.Set(2, 1, trueUp.z);

    rotationMatrix.Set(0, 2, forward.x);
    rotationMatrix.Set(1, 2, forward.y);
    rotationMatrix.Set(2, 2, forward.z);

    float trace = rotationMatrix.m[0] + rotationMatrix.m[4] + rotationMatrix.m[8];
    float w_, x_, y_, z_;

    if (trace > 0.0f)
    {
        float s = std::sqrt(trace + 1.0f) * 2.0f;
        w_ = 0.25f * s;
        x_ = (rotationMatrix.m[7] - rotationMatrix.m[5]) / s;
        y_ = (rotationMatrix.m[2] - rotationMatrix.m[6]) / s;
        z_ = (rotationMatrix.m[3] - rotationMatrix.m[1]) / s;
    }
    else if ((rotationMatrix.m[0] > rotationMatrix.m[4]) && (rotationMatrix.m[0] > rotationMatrix.m[8]))
    {
        float s = std::sqrt(1.0f + rotationMatrix.m[0] - rotationMatrix.m[4] - rotationMatrix.m[8]) * 2.0f;
        w_ = (rotationMatrix.m[7] - rotationMatrix.m[5]) / s;
        x_ = 0.25f * s;
        y_ = (rotationMatrix.m[1] + rotationMatrix.m[3]) / s;
        z_ = (rotationMatrix.m[2] + rotationMatrix.m[6]) / s;
    }
    else if (rotationMatrix.m[4] > rotationMatrix.m[8])
    {
        float s = std::sqrt(1.0f + rotationMatrix.m[4] - rotationMatrix.m[0] - rotationMatrix.m[8]) * 2.0f;
        w_ = (rotationMatrix.m[2] - rotationMatrix.m[6]) / s;
        x_ = (rotationMatrix.m[1] + rotationMatrix.m[3]) / s;
        y_ = 0.25f * s;
        z_ = (rotationMatrix.m[5] + rotationMatrix.m[7]) / s;
    }
    else
    {
        float s = std::sqrt(1.0f + rotationMatrix.m[8] - rotationMatrix.m[0] - rotationMatrix.m[4]) * 2.0f;
        w_ = (rotationMatrix.m[3] - rotationMatrix.m[1]) / s;
        x_ = (rotationMatrix.m[2] + rotationMatrix.m[6]) / s;
        y_ = (rotationMatrix.m[5] + rotationMatrix.m[7]) / s;
        z_ = 0.25f * s;
    }

    return Quaternion(w_, x_, y_, z_).Normalize();
}

Quaternion Quaternion::LookAtNegativeZ(const FVector3 &direction, const FVector3 &up)
{
    FVector3 forward = direction.Normalize();
    FVector3 upDir = up.Normalize();

    FVector3 right = upDir.Cross(forward).Normalize();
    FVector3 trueUp = forward.Cross(right);

    Matrix3 rotationMatrix;
    rotationMatrix.Set(0, 0, right.x);
    rotationMatrix.Set(1, 0, right.y);
    rotationMatrix.Set(2, 0, right.z);

    rotationMatrix.Set(0, 1, trueUp.x);
    rotationMatrix.Set(1, 1, trueUp.y);
    rotationMatrix.Set(2, 1, trueUp.z);

    rotationMatrix.Set(0, 2, -forward.x);
    rotationMatrix.Set(1, 2, -forward.y);
    rotationMatrix.Set(2, 2, -forward.z);

    float trace = rotationMatrix.m[0] + rotationMatrix.m[4] + rotationMatrix.m[8];
    float w_, x_, y_, z_;

    if (trace > 0.0f)
    {
        float s = std::sqrt(trace + 1.0f) * 2.0f;
        w_ = 0.25f * s;
        x_ = (rotationMatrix.m[7] - rotationMatrix.m[5]) / s;
        y_ = (rotationMatrix.m[2] - rotationMatrix.m[6]) / s;
        z_ = (rotationMatrix.m[3] - rotationMatrix.m[1]) / s;
    }
    else if ((rotationMatrix.m[0] > rotationMatrix.m[4]) && (rotationMatrix.m[0] > rotationMatrix.m[8]))
    {
        float s = std::sqrt(1.0f + rotationMatrix.m[0] - rotationMatrix.m[4] - rotationMatrix.m[8]) * 2.0f;
        w_ = (rotationMatrix.m[7] - rotationMatrix.m[5]) / s;
        x_ = 0.25f * s;
        y_ = (rotationMatrix.m[1] + rotationMatrix.m[3]) / s;
        z_ = (rotationMatrix.m[2] + rotationMatrix.m[6]) / s;
    }
    else if (rotationMatrix.m[4] > rotationMatrix.m[8])
    {
        float s = std::sqrt(1.0f + rotationMatrix.m[4] - rotationMatrix.m[0] - rotationMatrix.m[8]) * 2.0f;
        w_ = (rotationMatrix.m[2] - rotationMatrix.m[6]) / s;
        x_ = (rotationMatrix.m[1] + rotationMatrix.m[3]) / s;
        y_ = 0.25f * s;
        z_ = (rotationMatrix.m[5] + rotationMatrix.m[7]) / s;
    }
    else
    {
        float s = std::sqrt(1.0f + rotationMatrix.m[8] - rotationMatrix.m[0] - rotationMatrix.m[4]) * 2.0f;
        w_ = (rotationMatrix.m[3] - rotationMatrix.m[1]) / s;
        x_ = (rotationMatrix.m[2] + rotationMatrix.m[6]) / s;
        y_ = (rotationMatrix.m[5] + rotationMatrix.m[7]) / s;
        z_ = 0.25f * s;
    }

    return Quaternion(w_, x_, y_, z_).Normalize();
}

Quaternion Quaternion::FromAxisAngle(const FVector3 &axis, float angle)
{
    FVector3 normAxis = axis.Normalize();

    float halfAngle = angle * 0.5f;
    float sinHalfAngle = std::sin(halfAngle);

    float w_ = std::cos(halfAngle);
    float x_ = normAxis.x * sinHalfAngle;
    float y_ = normAxis.y * sinHalfAngle;
    float z_ = normAxis.z * sinHalfAngle;

    return Quaternion(w_, x_, y_, z_).Normalize();
}


// Accessors

FVector3 Quaternion::GetEulerAnglesXYZ() const
{
    FVector3 eulerAnglesXYZ;

    // Roll (X-axis rotation)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    eulerAnglesXYZ.x = std::atan2(sinr_cosp, cosr_cosp);

    // Pitch (Y-axis rotation)
    float sinp = 2.0f * (w * y - z * x);
    if (std::fabs(sinp) >= 1.0f)
        eulerAnglesXYZ.y = std::copysign(PI / 2.0f, sinp); // Use 90 degrees if out of range
    else
        eulerAnglesXYZ.y = std::asin(sinp);

    // Yaw (Z-axis rotation)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    eulerAnglesXYZ.z = std::atan2(siny_cosp, cosy_cosp);

    return eulerAnglesXYZ;
}


Matrix3 Quaternion::GetRotationMatrix3() const
{
    Matrix3 rotationMatrix;

    Quaternion q = this->Normalize();

    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    rotationMatrix.Set(0, 0, 1.0f - 2.0f * (yy + zz));
    rotationMatrix.Set(0, 1, 2.0f * (xy - wz));
    rotationMatrix.Set(0, 2, 2.0f * (xz + wy));

    rotationMatrix.Set(1, 0, 2.0f * (xy + wz));
    rotationMatrix.Set(1, 1, 1.0f - 2.0f * (xx + zz));
    rotationMatrix.Set(1, 2, 2.0f * (yz - wx));

    rotationMatrix.Set(2, 0, 2.0f * (xz - wy));
    rotationMatrix.Set(2, 1, 2.0f * (yz + wx));
    rotationMatrix.Set(2, 2, 1.0f - 2.0f * (xx + yy));
    return rotationMatrix;
}

Matrix4 Quaternion::GetRotationMatrix4() const
{
    Matrix4 rotationMatrix;

    Quaternion q = this->Normalize();

    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    rotationMatrix.Set(0, 0, 1.0f - 2.0f * (yy + zz)); // m[0]
    rotationMatrix.Set(1, 0, 2.0f * (xy - wz));        // m[1]
    rotationMatrix.Set(2, 0, 2.0f * (xz + wy));        // m[2]
    rotationMatrix.Set(3, 0, 0.0f);                    // m[3]

    rotationMatrix.Set(0, 1, 2.0f * (xy + wz));        // m[4]
    rotationMatrix.Set(1, 1, 1.0f - 2.0f * (xx + zz)); // m[5]
    rotationMatrix.Set(2, 1, 2.0f * (yz - wx));        // m[6]
    rotationMatrix.Set(3, 1, 0.0f);                    // m[7]

    rotationMatrix.Set(0, 2, 2.0f * (xz - wy));        // m[8]
    rotationMatrix.Set(1, 2, 2.0f * (yz + wx));        // m[9]
    rotationMatrix.Set(2, 2, 1.0f - 2.0f * (xx + yy)); // m[10]
    rotationMatrix.Set(3, 2, 0.0f);                    // m[11]

    rotationMatrix.Set(0, 3, 0.0f); // m[12]
    rotationMatrix.Set(1, 3, 0.0f); // m[13]
    rotationMatrix.Set(2, 3, 0.0f); // m[14]
    rotationMatrix.Set(3, 3, 1.0f); // m[15]

    return rotationMatrix;
}


// Quaternion Operations

float Quaternion::Dot(const Quaternion &other) const
{
    return w * other.w + x * other.x + y * other.y + z * other.z;
}

float Quaternion::Length() const
{
    return std::sqrt(w * w + x * x + y * y + z * z);
}

float Quaternion::LengthSquared() const
{
    return w * w + x * x + y * y + z * z;
}

Quaternion Quaternion::Normalize() const
{
    float len = Length();
    if (len == 0.0f)
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // Return identity quaternion if length is zero
    return Quaternion(w / len, x / len, y / len, z / len);
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::Inverse() const
{
    float lenSq = LengthSquared();
    if (lenSq == 0.0f)
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // Return identity if length is zero
    Quaternion conj = Conjugate();
    return Quaternion(conj.w / lenSq, conj.x / lenSq, conj.y / lenSq, conj.z / lenSq);
}

void Quaternion::ToAxisAngle(FVector3 &axis, float &angle) const
{
    Quaternion q = this->Normalize();
    angle = 2.0f * std::acos(q.w);
    float s = std::sqrt(1.0f - q.w * q.w);
    if (s < 0.001f) // If s is close to zero, direction of axis is not important
    {
        axis = FVector3(1.0f, 0.0f, 0.0f);
    }
    else
    {
        axis = FVector3(q.x / s, q.y / s, q.z / s);
    }
}


   void Quaternion::ApplyEulerAnglesXYZ(const FVector3 &eulerAnglesXYZ)
{
    Quaternion delta(eulerAnglesXYZ);
    *this = delta * (*this); // Pre-multiply instead of post-multiply
}
   

Quaternion Quaternion::operator+(const Quaternion &obj) const
{
    return Quaternion(w + obj.w, x + obj.x, y + obj.y, z + obj.z);
}

Quaternion &Quaternion::operator+=(const Quaternion &obj)
{
    w += obj.w;
    x += obj.x;
    y += obj.y;
    z += obj.z;
    return *this;
}

// Subtraction
Quaternion Quaternion::operator-(const Quaternion &obj) const
{
    return Quaternion(w - obj.w, x - obj.x, y - obj.y, z - obj.z);
}

Quaternion &Quaternion::operator-=(const Quaternion &obj)
{
    w -= obj.w;
    x -= obj.x;
    y -= obj.y;
    z -= obj.z;
    return *this;
}

// Scalar Multiplication
Quaternion Quaternion::operator*(float scalar) const
{
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

Quaternion &Quaternion::operator*=(float scalar)
{
    w *= scalar;
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

// Scalar Division
Quaternion Quaternion::operator/(float scalar) const
{
    if (scalar == 0.0f)
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // Return identity if dividing by zero
    float inv = 1.0f / scalar;
    return Quaternion(w * inv, x * inv, y * inv, z * inv);
}

Quaternion &Quaternion::operator/=(float scalar)
{
    if (scalar != 0.0f)
    {
        float inv = 1.0f / scalar;
        w *= inv;
        x *= inv;
        y *= inv;
        z *= inv;
    }
    else
    {
        // Handle division by zero if necessary
        // For now, do nothing or set to identity
        *this = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    }
    return *this;
}

// Quaternion Multiplication
Quaternion Quaternion::operator*(const Quaternion &obj) const
{
    return Quaternion(w * obj.w - x * obj.x - y * obj.y - z * obj.z, // w component
                      w * obj.x + x * obj.w + y * obj.z - z * obj.y, // x component
                      w * obj.y - x * obj.z + y * obj.w + z * obj.x, // y component
                      w * obj.z + x * obj.y - y * obj.x + z * obj.w  // z component
    );
}

Quaternion &Quaternion::operator*=(const Quaternion &obj)
{
    *this = *this * obj;
    return *this;
}

FVector3 Quaternion::operator*(const FVector3 &v) const
{
    // Convert the vector into a pure quaternion
    Quaternion qv(0.0f, v.x, v.y, v.z);

    // Rotate the vector: q_result = q * qv * q_conjugate
    Quaternion q_conj = this->Conjugate();
    Quaternion q_result = (*this) * qv * q_conj;

    // Extract the vector part of the resulting quaternion
    return FVector3(q_result.x, q_result.y, q_result.z);
}


// Comparison Operators
bool Quaternion::operator==(const Quaternion &other) const
{
    return (w == other.w && x == other.x && y == other.y && z == other.z);
}

bool Quaternion::operator!=(const Quaternion &other) const
{
    return !(*this == other);
}
