#pragma once
class Vector4
{
public:
	float x;	// x成分
	float y;	// y成分
	float z;	// z成分
	float w;	// w成分

public:
	// コンストラクタ
	Vector4();								// 零ベクトルとする
	Vector4(float x, float y, float z, float w);		// x成分, y成分, z成分, w成分 を指定しての生成

	// メンバ関数
	float length() const;						// ノルム(長さ)を求める
	Vector4& normalize();						// 正規化する
	float dot(const Vector4& v) const;			// 内積を求める
	Vector4 cross(const Vector4& v) const;		// 外積を求める

	// 単項演算子オーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	// 代入演算子オーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

