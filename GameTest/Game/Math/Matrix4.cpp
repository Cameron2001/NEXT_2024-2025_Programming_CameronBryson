#include "stdafx.h"
#include "MathUtil.h"
#include "Matrix4.h"
#include "Vector4.h"
#include <algorithm>

Matrix4::Matrix4()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            Set(row, column, (row == column) ? 1.0f : 0.0f);
        }
    }
}

void Matrix4::Set(const int row, const int column, const float value)
{
    if (row < 0 || row >= 4 || column < 0 || column >= 4)
    {
        return;
    }
    m[column * 4 + row] = value;
}

void Matrix4::SetZero()
{
    for (int i = 0; i < 16; ++i)
    {
        m[i] = 0.0f;
    }
}

void Matrix4::SetIdentity()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            Set(row, column, (row == column) ? 1.0f : 0.0f);
        }
    }
}

float Matrix4::Get(const int row, const int column) const
{
    if (row < 0 || row >= 4 || column < 0 || column >= 4)
    {
        return 0.0f; 
    }
    return m[column * 4 + row];
}

Matrix4 Matrix4::CreatePerspectiveMatrix(const float fov, const float aspectRatio, const float zNear, const float zFar)
{
    Matrix4 perspectiveMatrix;
    const float radFov = MathUtil::DegreesToRadians(fov);
    const float tanHalfFOV = tanf(radFov / 2.0f);

    perspectiveMatrix.Set(0, 0, 1.0f / (aspectRatio * tanHalfFOV));
    perspectiveMatrix.Set(1, 1, 1.0f / tanHalfFOV);
    perspectiveMatrix.Set(2, 2, -(zFar + zNear) / (zFar - zNear));
    perspectiveMatrix.Set(2, 3, -1.0f);                                   
    perspectiveMatrix.Set(3, 2, -(2.0f * zFar * zNear) / (zFar - zNear)); 
    perspectiveMatrix.Set(3, 3, 0.0f);

    return perspectiveMatrix;
}

Matrix4 Matrix4::CreateViewMatrix(const FVector3 &origin, const FVector3 &position, const FVector3 &up)
{
    FVector3 zaxis = (origin - position).Normalize(); 
    FVector3 xaxis = up.Cross(zaxis).Normalize(); 
    FVector3 yaxis = zaxis.Cross(xaxis);

    Matrix4 translation;
    translation.Set(0, 3, -position.x);
    translation.Set(1, 3, -position.y);
    translation.Set(2, 3, -position.z);

    Matrix4 rotation;
    rotation.Set(0, 0, xaxis.x);
    rotation.Set(0, 1, yaxis.x);
    rotation.Set(0, 2, zaxis.x);

    rotation.Set(1, 0, xaxis.y);
    rotation.Set(1, 1, yaxis.y);
    rotation.Set(1, 2, zaxis.y);

    rotation.Set(2, 0, xaxis.z);
    rotation.Set(2, 1, yaxis.z);
    rotation.Set(2, 2, zaxis.z);

    return rotation * translation;
}


Matrix4 Matrix4::CreateTranslationMatrix(const FVector3 &translation)
{
    Matrix4 translationMatrix;
    translationMatrix.Set(0, 3, translation.x);
    translationMatrix.Set(1, 3, translation.y);
    translationMatrix.Set(2, 3, translation.z);

    return translationMatrix;
}

Matrix4 Matrix4::CreateScaleMatrix(const FVector3 &scale)
{
    Matrix4 scaleMatrix;
    scaleMatrix.Set(0, 0, scale.x);
    scaleMatrix.Set(1, 1, scale.y);
    scaleMatrix.Set(2, 2, scale.z);
    return scaleMatrix;
}

FVector3 Matrix4::GetRight() const
{
    return FVector3{m[0], m[1], m[2]};
}

FVector3 Matrix4::GetUp() const
{
    return FVector3{m[4], m[5], m[6]};
}

FVector3 Matrix4::GetForward() const
{
    return FVector3{m[8], m[9], m[10]};
}

Matrix4 Matrix4::Inverse() const
{
    Matrix4 inv;
    float m[16];
    memcpy(m, this->m, 16 * sizeof(float));

    int indxc[4], indxr[4], ipiv[4] = {};
    int i, icol = 0, irow = 0, j, k, l, ll;
    float big, dum, pivinv;

    for (i = 0; i < 4; i++)
    {
        big = 0.0f;
        for (j = 0; j < 4; j++)
        {
            if (ipiv[j] != 1)
            {
                for (k = 0; k < 4; k++)
                {
                    if (ipiv[k] == 0)
                    {
                        if (fabs(m[j * 4 + k]) >= big)
                        {
                            big = fabs(m[j * 4 + k]);
                            irow = j;
                            icol = k;
                        }
                    }
                }
            }
        }
        ++(ipiv[icol]);

        if (irow != icol)
        {
            for (l = 0; l < 4; l++)
            {
                std::swap(m[irow * 4 + l], m[icol * 4 + l]);
                std::swap(inv.m[irow * 4 + l], inv.m[icol * 4 + l]);
            }
        }

        indxr[i] = irow;
        indxc[i] = icol;

        if (m[icol * 4 + icol] == 0.0f)
            return Matrix4(); // Singular matrix, return identity as per your convention

        pivinv = 1.0f / m[icol * 4 + icol];
        m[icol * 4 + icol] = 1.0f;
        for (l = 0; l < 4; l++)
            m[icol * 4 + l] *= pivinv;
        for (l = 0; l < 4; l++)
            inv.m[icol * 4 + l] *= pivinv;

        for (ll = 0; ll < 4; ll++)
        {
            if (ll != icol)
            {
                dum = m[ll * 4 + icol];
                m[ll * 4 + icol] = 0.0f;
                for (l = 0; l < 4; l++)
                {
                    m[ll * 4 + l] -= m[icol * 4 + l] * dum;
                    inv.m[ll * 4 + l] -= inv.m[icol * 4 + l] * dum;
                }
            }
        }
    }

    for (l = 3; l >= 0; l--)
    {
        if (indxr[l] != indxc[l])
        {
            for (k = 0; k < 4; k++)
            {
                std::swap(m[k * 4 + indxr[l]], m[k * 4 + indxc[l]]);
            }
        }
    }

    return inv;
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 result;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result.m[row * 4 + col] = m[col * 4 + row];
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4 &obj) const
{
    Matrix4 result;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            float sum = 0.0f;
            for (int i = 0; i < 4; ++i)
            {
                sum += this->m[i * 4 + row] * obj.m[col * 4 + i];
            }
            result.Set(row, col, sum);
        }
    }
    return result;
}

FVector3 Matrix4::operator*(const FVector3 &obj) const
{
    const float x = m[0] * obj.x + m[4] * obj.y + m[8] * obj.z + m[12] * 1.0f;
    const float y = m[1] * obj.x + m[5] * obj.y + m[9] * obj.z + m[13] * 1.0f;
    const float z = m[2] * obj.x + m[6] * obj.y + m[10] * obj.z + m[14] * 1.0f;
    return FVector3{x, y, z};
}

FVector4 Matrix4::operator*(const FVector4 &obj) const
{
    const float x = m[0] * obj.x + m[4] * obj.y + m[8] * obj.z + m[12] * obj.w;
    const float y = m[1] * obj.x + m[5] * obj.y + m[9] * obj.z + m[13] * obj.w;
    const float z = m[2] * obj.x + m[6] * obj.y + m[10] * obj.z + m[14] * obj.w;
    const float w = m[3] * obj.x + m[7] * obj.y + m[11] * obj.z + m[15] * obj.w;
    return FVector4{x, y, z, w};
}

FVector3 Matrix4::TransformWithPerspectiveDivide(const FVector3 &obj) const
{
    float x = m[0] * obj.x + m[4] * obj.y + m[8] * obj.z + m[12] * 1.0f;
    float y = m[1] * obj.x + m[5] * obj.y + m[9] * obj.z + m[13] * 1.0f;
    float z = m[2] * obj.x + m[6] * obj.y + m[10] * obj.z + m[14] * 1.0f;
    const float w = m[3] * obj.x + m[7] * obj.y + m[11] * obj.z + m[15] * 1.0f;

    // Perspective divide when requested
    if (w != 0.0f)
    {
        x /= w;
        y /= w;
        z /= w;
    }
    return FVector3{x, y, z};
}

bool Matrix4::operator==(const Matrix4 &other) const
{
    for (int i = 0; i < 16; ++i)
    {
        if (m[i] != other.m[i])
        {
            return false;
        }
    }
    return true;
}

bool Matrix4::operator!=(const Matrix4 &other) const
{
    return !(*this == other);
}
