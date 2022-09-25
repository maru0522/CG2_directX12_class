#pragma once
#include "Vector3.h"

// Matrix4 �\����
struct Matrix4
{
    float m[4][4];
};

// �P�ʍs������߂�
Matrix4 Identity();

// �g��k���s��̐ݒ�
Matrix4 Scale(const Vector3& s);

// ��]�s��̐ݒ�
Matrix4 RotateX(float angle);
Matrix4 RotateY(float angle);
Matrix4 RotateZ(float angle);

// ���s�ړ��s��̍쐬
Matrix4 Translate(const Vector3& t);

// ���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
Vector3 Transform(const Vector3& v, const Matrix4& m);

// ������Z�q�I�[�o�[���[�h
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

// 2�����Z�q�I�[�o�[���[�h
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);