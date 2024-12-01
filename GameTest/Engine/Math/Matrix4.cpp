#include "stdafx.h"
#include "Matrix4.h"
#include "MathUtil.h"

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

void Matrix4::Set(int row, int column, float value)
{
	if (row < 0 || row >= 4 || column < 0 || column >= 4)
	{
		return;
	}
	m[column * 4 + row] = value;
}

Matrix4 Matrix4::CreatePerspectiveMatrix(const float fov, const float aspectRatio, const float zNear, const float zFar)
{
	Matrix4 perspectiveMatrix;
	float radFov = MathUtil::DegreesToRadians(fov);
	float tanHalfFOV = tanf(radFov / 2.0f);
	float zRange = zNear - zFar;

	perspectiveMatrix.Set(0, 0, 1.0f / (aspectRatio * tanHalfFOV));
	perspectiveMatrix.Set(1, 1, 1.0f / tanHalfFOV);
	perspectiveMatrix.Set(2, 2, (-zNear - zFar) / zRange);
	perspectiveMatrix.Set(2, 3, (2.0f * zFar * zNear) / zRange);
	perspectiveMatrix.Set(3, 2, -1.0f);
	perspectiveMatrix.Set(3, 3, 0.0f);
	return perspectiveMatrix;
}

Matrix4 Matrix4::CreateViewMatrix(const FVector3& origin, const FVector3& target, const FVector3& up)
{
	//Right handed
	FVector3 forward = (target-origin).Normalize(); //zAxis
	FVector3 right = forward.Cross(up).Normalize(); //xAxis
	FVector3 newUp = right.Cross(forward); //yAxis

	Matrix4 viewMatrix;

	viewMatrix.Set(0, 0, right.X);
	viewMatrix.Set(1, 0, right.Y);
	viewMatrix.Set(2, 0, right.Z);

	viewMatrix.Set(0, 1, newUp.X);
	viewMatrix.Set(1, 1, newUp.Y);
	viewMatrix.Set(2, 1, newUp.Z);

	viewMatrix.Set(0, 2, -forward.X);
	viewMatrix.Set(1, 2, -forward.Y);
	viewMatrix.Set(2, 2, -forward.Z);

	viewMatrix.Set(3, 0, -right.Dot(origin));
	viewMatrix.Set(3, 1, -newUp.Dot(origin));
	viewMatrix.Set(3, 2, forward.Dot(origin));

	return viewMatrix;
}

Matrix4 Matrix4::CreateTranslationMatrix(const FVector3& translation)
{
	Matrix4 translationMatrix;
	translationMatrix.Set(0, 3, translation.X);
	translationMatrix.Set(1, 3, translation.Y);
	translationMatrix.Set(2, 3, translation.Z);

	return translationMatrix;
}

Matrix4 Matrix4::CreateScaleMatrix( const FVector3 scale)
{
	Matrix4 scaleMatrix;
	scaleMatrix.Set(0, 0, scale.X);
	scaleMatrix.Set(1, 1, scale.Y);
	scaleMatrix.Set(2, 2, scale.Z);
	return scaleMatrix;
}

Matrix4 Matrix4::CreateEulerAngleMatrixXYZ(const FVector3& rotation)
{
	Matrix4 eulerAngleMatrix;
	float radX = MathUtil::DegreesToRadians(rotation.X);
	float radY = MathUtil::DegreesToRadians(rotation.Y);
	float radZ = MathUtil::DegreesToRadians(rotation.Z);

	float cosX = cosf(-radX);
	float cosY = cosf(-radY);
	float cosZ = cosf(-radZ);

	float sinX = sinf(-radX);
	float sinY = sinf(-radY);
	float sinZ = sinf(-radZ);

	eulerAngleMatrix.Set(0, 0, cosY * cosZ);
	eulerAngleMatrix.Set(0, 1, -cosX * sinZ + sinX * sinY * cosZ);
	eulerAngleMatrix.Set(0, 2, sinX * sinZ + cosX * sinY * cosZ);
	eulerAngleMatrix.Set(0, 3, 0.0f);

	eulerAngleMatrix.Set(1, 0, cosY * sinZ);
	eulerAngleMatrix.Set(1, 1, cosX * cosZ + sinX * sinY * sinZ);
	eulerAngleMatrix.Set(1, 2, -sinX * cosZ + cosX * sinY * sinZ);
	eulerAngleMatrix.Set(1, 3, 0.0f);

	eulerAngleMatrix.Set(2, 0, -sinY);
	eulerAngleMatrix.Set(2, 1, sinX * cosY);
	eulerAngleMatrix.Set(2, 2, cosX * cosY);
	eulerAngleMatrix.Set(2, 3, 0.0f);

	eulerAngleMatrix.Set(3, 0, 0.0f);
	eulerAngleMatrix.Set(3, 1, 0.0f);
	eulerAngleMatrix.Set(3, 2, 0.0f);
	eulerAngleMatrix.Set(3, 3, 1.0f);
	return eulerAngleMatrix;
}

FVector3 Matrix4::GetRight() const
{
	return FVector3{ m[0], m[1], m[2] };
}

FVector3 Matrix4::GetUp() const
{
	return FVector3{ m[4], m[5], m[6] };
}

FVector3 Matrix4::GetForward() const
{
	return FVector3{ m[8], m[9], m[10] };
}

Matrix4 Matrix4::operator*(const Matrix4& obj) const
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

FVector3 Matrix4::operator*(const FVector3& obj) const
{
	float x = m[0] * obj.X + m[4] * obj.Y + m[8] * obj.Z + m[12] * 1.0f;
	float y = m[1] * obj.X + m[5] * obj.Y + m[9] * obj.Z + m[13] * 1.0f;
	float z = m[2] * obj.X + m[6] * obj.Y + m[10] * obj.Z + m[14] * 1.0f;
	float w = m[3] * obj.X + m[7] * obj.Y + m[11] * obj.Z + m[15] * 1.0f;
	if (w != 0.0f && w != 1.0f)
	{
		x /= w;
		y /= w;
		z /= w;
	}
	return FVector3{ x, y, z };
}
