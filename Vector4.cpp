#include "Vector4.h"
#include <cmath>

// �����Ȃ��R���X�g���N�^
// ��x�N�g���ɏ���������
Vector4::Vector4()
	: x(0), y(0), z(0), w(0)
{
}

// ����4�̃R���X�g���N�^
Vector4::Vector4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

// �x�N�g���̒��������߂�
float Vector4::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

// �x�N�g���𐳋K������(�P�ʃx�N�g����)����
Vector4& Vector4::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

// ���ς����߂�
float Vector4::dot(const Vector4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

// �O�ς����߂�
Vector4 Vector4::cross(const Vector4& v)const
{
	// ref: https://icemilktea4444.seesaa.net/article/201905article_4.html

	return Vector4(y * v.w - w * v.y,
				   z * v.x - x * v.z,
				   w * v.y - y * v.w,
				   x * v.z - z * v.x);
}

// �P�����Z�q +
Vector4 Vector4::operator+() const
{
	return *this;
}
// �P�����Z�q -
Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

// ������Z�q +=
Vector4& Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
// ������Z�q -=
Vector4& Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}
// ������Z�q *=
Vector4& Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}
// ������Z�q /=
Vector4& Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

// 2�����Z�q +
const Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp += v2;
}
// 2�����Z�q -
const Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp -= v2;
}
const Vector4 operator*(const Vector4& v, float s)
{
	Vector4 temp(v);
	return temp *= s;
}
const Vector4 operator/(const Vector4& v, float s)
{
	Vector4 temp(v);
	return temp /= s;
}