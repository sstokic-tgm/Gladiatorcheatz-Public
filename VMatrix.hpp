#pragma once

#include <limits>
#include "Vector3.hpp"

class matrix3x4_t
{

public:

	matrix3x4_t() {};
	matrix3x4_t(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34)
	{
		m_flMatVal[0][0] = _11;		m_flMatVal[0][1] = _12;		m_flMatVal[0][2] = _13;		m_flMatVal[0][3] = _14;
		m_flMatVal[1][0] = _21;		m_flMatVal[1][1] = _22;		m_flMatVal[1][2] = _23;		m_flMatVal[1][3] = _24;
		m_flMatVal[2][0] = _31;		m_flMatVal[2][1] = _32;		m_flMatVal[2][2] = _33;		m_flMatVal[2][3] = _34;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector3 &xAxis, const Vector3 &yAxis, const Vector3 &zAxis, const Vector3 &vekOrigin)
	{
		m_flMatVal[0][0] = xAxis.xAechse;		m_flMatVal[0][1] = yAxis.xAechse;		m_flMatVal[0][2] = zAxis.xAechse;		m_flMatVal[0][3] = vekOrigin.xAechse;
		m_flMatVal[1][0] = xAxis.yAechse;		m_flMatVal[1][1] = yAxis.yAechse;		m_flMatVal[1][2] = zAxis.yAechse;		m_flMatVal[1][3] = vekOrigin.yAechse;
		m_flMatVal[2][0] = xAxis.zAechse;		m_flMatVal[2][1] = yAxis.zAechse;		m_flMatVal[2][2] = zAxis.zAechse;		m_flMatVal[2][3] = vekOrigin.zAechse;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector3 &xAxis, const Vector3 &yAxis, const Vector3 &zAxis, const Vector3 &vekOrigin)
	{
		Init(xAxis, yAxis, zAxis, vekOrigin);
	}

	inline void SetOrigin(Vector3 const &p)
	{
		m_flMatVal[0][3] = p.xAechse;
		m_flMatVal[1][3] = p.yAechse;
		m_flMatVal[2][3] = p.zAechse;
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				m_flMatVal[i][j] = std::numeric_limits<float>::infinity();;
			}
		}
	}

	inline float *operator[](int i)
	{
		return m_flMatVal[i];
	}

	inline const float *operator[](int i) const
	{
		return m_flMatVal[i];
	}

	inline float *base()
	{
		return &m_flMatVal[0][0];
	}

	inline const float *base() const
	{
		return &m_flMatVal[0][0];
	}

	float m_flMatVal[3][4];
};

class __declspec(align(16)) matrix3x4a_t : public matrix3x4_t
{
	matrix3x4a_t &operator=(const matrix3x4_t& src) { memcpy(base(), src.base(), sizeof(float) * 3 * 4); return *this; };
};