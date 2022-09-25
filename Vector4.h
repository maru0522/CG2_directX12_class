#pragma once
class Vector4
{
public:
	float x;	// x����
	float y;	// y����
	float z;	// z����
	float w;	// w����

public:
	// �R���X�g���N�^
	Vector4();								// ��x�N�g���Ƃ���
	Vector4(float x, float y, float z, float w);		// x����, y����, z����, w���� ���w�肵�Ă̐���

	// �����o�֐�
	float length() const;						// �m����(����)�����߂�
	Vector4& normalize();						// ���K������
	float dot(const Vector4& v) const;			// ���ς����߂�
	Vector4 cross(const Vector4& v) const;		// �O�ς����߂�

	// �P�����Z�q�I�[�o�[���[�h
	Vector4 operator+() const;
	Vector4 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

