#include "stdafx.h"
#include "Quaternion.h"
#include "App/AppSettings.h"
#include "Game/Math/MathUtil.h"


Quaternion::Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f)
{
}



Quaternion::Quaternion(float x, float y, float z) : w(0.0f), x(x), y(y), z(z)
{
}


Quaternion::Quaternion(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_)
{
}

Quaternion::Quaternion(const Quaternion &copy) : w(copy.w), x(copy.x), y(copy.y), z(copy.z)
{
}



Quaternion Quaternion::LookAtPlusZ(const FVector3 &direction, const FVector3 &up)
{
    Matrix3 lookAtMatrix;

    FVector3 right = up.Cross(direction).Normalize();
    FVector3 normalizedRight = right * (1.0f / sqrtf(std::max(0.00001f, right.Dot(right))));
    FVector3 forward = right.Cross(normalizedRight);

    lookAtMatrix.Set(2, 0, direction.x);
    lookAtMatrix.Set(2, 1, direction.y);
    lookAtMatrix.Set(2, 2, direction.z);

    lookAtMatrix.Set(0, 0, normalizedRight.x);
    lookAtMatrix.Set(0, 1, normalizedRight.y);
    lookAtMatrix.Set(0, 2, normalizedRight.z);

    lookAtMatrix.Set(1, 0, forward.x);
    lookAtMatrix.Set(1, 1, forward.y);
    lookAtMatrix.Set(1, 2, forward.z);

    return Quaternion::FromMatrix3(lookAtMatrix);
}

Quaternion Quaternion::LookAtNegativeZ(const FVector3 &direction, const FVector3 &up)
{
    Matrix3 lookAtMatrix;

    FVector3 right = up.Cross(-direction).Normalize();
    FVector3 normalizedRight = right * (1.0f / sqrtf(std::max(0.00001f, right.Dot(right))));
    FVector3 forward = right.Cross(normalizedRight);

    lookAtMatrix.Set(2, 0, -direction.x);
    lookAtMatrix.Set(2, 1, -direction.y);
    lookAtMatrix.Set(2, 2, -direction.z);

    lookAtMatrix.Set(0, 0, normalizedRight.x);
    lookAtMatrix.Set(0, 1, normalizedRight.y);
    lookAtMatrix.Set(0, 2, normalizedRight.z);

    lookAtMatrix.Set(1, 0, forward.x);
    lookAtMatrix.Set(1, 1, forward.y);
    lookAtMatrix.Set(1, 2, forward.z);

    return Quaternion::FromMatrix3(lookAtMatrix);
}

Quaternion Quaternion::FromAxisAngle(const FVector3 &axis, float angle)
{
    float halfAngle = angle * 0.5f;

    float s = sinf(halfAngle);
    float c = cosf(halfAngle);

    float sinNorm = s / sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

    return Quaternion(c, axis.x * sinNorm, axis.y * sinNorm, axis.z * sinNorm);
}

Quaternion Quaternion::FromEulerAnglesXYZ(const FVector3 &eulerAngles)
{
    const float x = eulerAngles.x * 0.5f;
    const float y = eulerAngles.y * 0.5f;
    const float z = eulerAngles.z * 0.5f;

    const float c1 = cosf(x);
    const float c2 = cosf(y);
    const float c3 = cosf(z);

    const float s1 = sinf(x);
    const float s2 = sinf(y);
    const float s3 = sinf(z);

    return Quaternion(c1 * c2 * c3 - s1 * s2 * s3, s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3,
                      c1 * c2 * s3 + s1 * s2 * c3);
}

Quaternion Quaternion::FromMatrix3(const Matrix3 &matrix)
{
    Quaternion q;
    float fourXSquaredMinus1 = matrix.Get(0, 0) - matrix.Get(1, 1) - matrix.Get(2, 2);
    float fourYSquaredMinus1 = matrix.Get(1, 1) - matrix.Get(0, 0) - matrix.Get(2, 2);
    float fourZSquaredMinus1 = matrix.Get(2, 2) - matrix.Get(0, 0) - matrix.Get(1, 1);
    float fourWSquaredMinus1 = matrix.Get(0, 0) + matrix.Get(1, 1) + matrix.Get(2, 2);

    int biggestIndex = 0;
    float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
    float mult = 0.25f / biggestVal;

    switch (biggestIndex)
    {
    case 0:
        q.w = biggestVal;
        q.x = (matrix.Get(1, 2) - matrix.Get(2, 1)) * mult;
        q.y = (matrix.Get(2, 0) - matrix.Get(0, 2)) * mult;
        q.z = (matrix.Get(0, 1) - matrix.Get(1, 0)) * mult;
        break;
    case 1:
        q.x = biggestVal;
        q.w = (matrix.Get(1, 2) - matrix.Get(2, 1)) * mult;
        q.y = (matrix.Get(0, 1) + matrix.Get(1, 0)) * mult;
        q.z = (matrix.Get(2, 0) + matrix.Get(0, 2)) * mult;
        break;
    case 2:
        q.y = biggestVal;
        q.w = (matrix.Get(2, 0) - matrix.Get(0, 2)) * mult;
        q.x = (matrix.Get(0, 1) + matrix.Get(1, 0)) * mult;
        q.z = (matrix.Get(1, 2) + matrix.Get(2, 1)) * mult;
        break;
    case 3:
        q.z = biggestVal;
        q.w = (matrix.Get(0, 1) - matrix.Get(1, 0)) * mult;
        q.x = (matrix.Get(2, 0) + matrix.Get(0, 2)) * mult;
        q.y = (matrix.Get(1, 2) + matrix.Get(2, 1)) * mult;
        break;
    }
    return q;
}
float Quaternion::GetPitch() const
{
    const float sinPitch = 2.0f * (y * z + w * x);
    const float cosPitch = w * w - x * x - y * y + z * z;
    return std::atan2f(sinPitch, cosPitch);
}


float Quaternion::GetYaw() const
{
    return asinf(MathUtil::Clamp(-2.0f * (x * z - w * y), -1.0f, 1.0f));
}

float Quaternion::GetRoll() const
{
    const float sinRoll = 2.0f * (x * y + w * z);
    const float cosRoll = w * w + x * x - y * y - z * z;
    return std::atan2f(sinRoll, cosRoll);
}


FVector3 Quaternion::GetEulerAnglesXYZ() const
{
    return (FVector3{GetPitch(), GetYaw(), GetRoll()});
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

    rotationMatrix.Set(0, 0, 1.0f - 2.0f * (yy + zz)); 
    rotationMatrix.Set(1, 0, 2.0f * (xy - wz));        
    rotationMatrix.Set(2, 0, 2.0f * (xz + wy));        
    rotationMatrix.Set(3, 0, 0.0f);                    

    rotationMatrix.Set(0, 1, 2.0f * (xy + wz));        
    rotationMatrix.Set(1, 1, 1.0f - 2.0f * (xx + zz)); 
    rotationMatrix.Set(2, 1, 2.0f * (yz - wx));        
    rotationMatrix.Set(3, 1, 0.0f);                    

    rotationMatrix.Set(0, 2, 2.0f * (xz - wy));       
    rotationMatrix.Set(1, 2, 2.0f * (yz + wx));        
    rotationMatrix.Set(2, 2, 1.0f - 2.0f * (xx + yy)); 
    rotationMatrix.Set(3, 2, 0.0f);                    

    rotationMatrix.Set(0, 3, 0.0f); 
    rotationMatrix.Set(1, 3, 0.0f); 
    rotationMatrix.Set(2, 3, 0.0f); 
    rotationMatrix.Set(3, 3, 1.0f); 

    return rotationMatrix;
}



float Quaternion::Dot(const Quaternion &other) const
{
    return w * other.w + x * other.x + y * other.y + z * other.z;
}

float Quaternion::Length() const
{
    return sqrtf(LengthSquared());
}

float Quaternion::LengthSquared() const
{
    return w * w + x * x + y * y + z * z;
}

Quaternion Quaternion::Normalize() const
{
    float length = Length();
    if (length == 0.0f)
    {
        return *this;
    }
    return Quaternion(w / length, x / length, y / length, z / length);
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::Inverse() const
{
    return Conjugate() / LengthSquared();
}

FVector3 Quaternion::RotateVector3(const FVector3 &vector) const
{
    Quaternion vecQuat(0.0f, vector.x, vector.y, vector.z);

    Quaternion invQuat = this->Inverse();

    Quaternion rotatedQuat = (*this) * vecQuat * invQuat;

    return FVector3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);
}

Quaternion Quaternion::operator*(const Quaternion &other) const
{
    return Quaternion(w * other.w - x * other.x - y * other.y - z * other.z,
                      w * other.x + x * other.w + y * other.z - z * other.y,
                      w * other.y - x * other.z + y * other.w + z * other.x,
                      w * other.z + x * other.y - y * other.x + z * other.w 
    );
}


Quaternion Quaternion::operator+(const Quaternion &other) const
{
    return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
}



Quaternion Quaternion::operator*(const float scalar) const
{
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}


Quaternion Quaternion::operator/(const float scalar) const
{
    if (scalar == 0.0f)
    {
        return Quaternion();
    }
    float invScalar = 1.0f / scalar;
    return Quaternion(w * invScalar, x * invScalar, y * invScalar, z * invScalar);
}






bool Quaternion::operator==(const Quaternion &other) const
{
    return w == other.w && x == other.x && y == other.y && z == other.z;
}

bool Quaternion::operator!=(const Quaternion &other) const
{
    return !(*this == other);
}
