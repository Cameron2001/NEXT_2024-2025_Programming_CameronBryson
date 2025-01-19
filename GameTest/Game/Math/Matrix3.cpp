#include "stdafx.h"
#include "Matrix3.h"
#include "Vector3.h"

Matrix3::Matrix3()
{
    // Initialize as identity matrix
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            Set(row, col, (row == col) ? 1.0f : 0.0f);
        }
    }
}
void Matrix3::Set(int row, int column, float value)
{
    if (row < 0 || row >= 3 || column < 0 || column >= 3)
    {
        return;
    }
    m[column * 3 + row] = value;
}
Matrix3 Matrix3::SetZero()
{
    for (int i = 0; i < 9; ++i)
    {
        m[i] = 0.0f;
    }
    return *this;
}

void Matrix3::SetIdentity()
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            Set(row, col, (row == col) ? 1.0f : 0.0f);
        }
    }
}
float Matrix3::Get(int row, int column) const
{
    if (row < 0 || row >= 3 || column < 0 || column >= 3)
    {
        return 0.0f;
    }
    return m[column * 3 + row];
}
FVector3 Matrix3::GetRight() const
{
    return FVector3{m[0], m[1], m[2]};
}

FVector3 Matrix3::GetUp() const
{
    return FVector3{m[3], m[4], m[5]};
}

FVector3 Matrix3::GetForward() const
{
    return FVector3{m[6], m[7], m[8]};
}

Matrix3 Matrix3::Inverse() const
{
    Matrix3 inv;
    float det =
        m[0] * (m[4] * m[8] - m[5] * m[7]) - m[1] * (m[3] * m[8] - m[5] * m[6]) + m[2] * (m[3] * m[7] - m[4] * m[6]);

    if (det == 0.0f)
        return Matrix3(); // Return identity matrix

    float invDet = 1.0f / det;

    inv.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
    inv.m[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
    inv.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
    inv.m[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
    inv.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
    inv.m[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
    inv.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
    inv.m[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
    inv.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;

    return inv;
}

Matrix3 Matrix3::Transpose() const
{
    Matrix3 transposed;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            transposed.Set(row, col, m[col * 3 + row]);
        }
    }
    return transposed;
}

Matrix3 Matrix3::operator*(const Matrix3 &obj) const
{
    Matrix3 result;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            float sum = 0.0f;
            for (int k = 0; k < 3; ++k)
            {
                sum += m[k * 3 + row] * obj.m[col * 3 + k];
            }
            result.Set(row, col, sum);
        }
    }
    return result;
}

FVector3 Matrix3::operator*(const FVector3 &obj) const
{
    float x = m[0] * obj.x + m[3] * obj.y + m[6] * obj.z;
    float y = m[1] * obj.x + m[4] * obj.y + m[7] * obj.z;
    float z = m[2] * obj.x + m[5] * obj.y + m[8] * obj.z;
    return FVector3{x, y, z};
}

bool Matrix3::operator==(const Matrix3 &other) const
{
    for (int i = 0; i < 9; ++i)
    {
        if (m[i] != other.m[i])
            return false;
    }
    return true;
}

bool Matrix3::operator!=(const Matrix3 &other) const
{
    return !(*this == other);
}
