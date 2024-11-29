#pragma once
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
	Matrix4();
public:
	Matrix4& CreateIdentityMatrix();
	Matrix4& CreatePerspectiveMatrix();
	Matrix4& CreateOrthoMatrix();
	Matrix4& CreateViewMatrix(); //Look at
	Matrix4& CreateTranslationMatrix();
	Matrix4& CreateScaleMatrix();
	Matrix4& CreateEulerAngleMatrix();



};

