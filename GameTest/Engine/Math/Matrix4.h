#pragma once
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
//eulerAngleXYZ
//Get Axes
//perspective
//Look at
//ortho
//translate
//scale
class Matrix4
{
public:
    //Column Major because of opengl
    float m[16];
    Matrix4();
    ~Matrix4() = default;
    void Set(int row, int column, float value);
    static Matrix4 CreatePerspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar);
    static Matrix4 CreateViewMatrix(const FVector3 &origin, const FVector3 &target, const FVector3 &up); //Look at
    static Matrix4 CreateTranslationMatrix(const FVector3 &translation);
    static Matrix4 CreateScaleMatrix(FVector3 scale);
    static Matrix4 CreateEulerAngleMatrixXYZ(const FVector3 &rotation);

    FVector3 GetRight() const;
    FVector3 GetUp() const;
    FVector3 GetForward() const;
    Matrix4 Inverse() const;
    Matrix4 Transpose() const;
    Matrix4 operator*(const Matrix4 &obj) const;
    FVector3 operator*(const FVector3 &obj) const;
    FVector4 operator*(const FVector4 &obj) const;
    FVector3 TransformWithPerspectiveDivide(const FVector3 &obj) const;


};