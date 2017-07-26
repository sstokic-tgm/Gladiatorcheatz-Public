#pragma once

#define CHECK_VALID(_v) 0
#define Assert(_exp) ((void)0)

class Vector3
{
public:

	float xAechse, yAechse, zAechse;

	Vector3() {};
	Vector3(float xAechse, float yAechse, float zAechse) : xAechse{ xAechse }, yAechse{ yAechse }, zAechse{ zAechse } {}
	Vector3(float *xyz) { xAechse = xyz[0]; yAechse = xyz[1]; zAechse = xyz[2]; }
	Vector3(const Vector3 &v) { xAechse = v.xAechse; yAechse = v.yAechse; zAechse = v.zAechse; }
	Vector3(Vector3 &&v) : xAechse(0), yAechse(0), zAechse(0)
	{
		xAechse = v.xAechse;
		yAechse = v.yAechse;
		zAechse = v.zAechse;

		v.xAechse = 0;
		v.yAechse = 0;
		v.zAechse = 0;
	}

	~Vector3() {};

	inline void init(float xAechse, float yAechse, float zAechse)
	{
		this->xAechse = xAechse; this->yAechse = yAechse; this->zAechse = zAechse;
	}

	inline float *base()
	{
		return (float*)this;
	}

	inline float const *base() const
	{
		return (float const*)this;
	}

	inline float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	inline float &operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline float length()
	{
		float root = 0.0f;

		float sqsr = (this->xAechse * this->xAechse) + (this->yAechse * this->yAechse) + (this->zAechse * this->zAechse);

		_asm
		{
			sqrtss xmm0, sqsr
			movss root, xmm0
		}

		return root;
	}

	inline Vector3 normalize()
	{
		Vector3 vekt;
		float len = this->length();

		if (len != 0)
		{
			vekt.xAechse = this->xAechse / len;
			vekt.yAechse = this->yAechse / len;
			vekt.zAechse = this->zAechse / len;
		}
		else
		{
			vekt.xAechse = vekt.yAechse = 0.0f;
			vekt.zAechse = 1.0f;
		}

		return vekt;
	}

	inline float normalizeInPlace()
	{
		Vector3 &vekt = *this;

		float iradius = 1.f / (this->length() + 1.192092896e-07F);

		vekt.xAechse *= iradius;
		vekt.yAechse *= iradius;
		vekt.zAechse *= iradius;
	}

	inline float length2d()
	{
		float root = 0.0f;
		float sqs = (this->xAechse * this->xAechse) + (this->yAechse * this->yAechse);

		_asm
		{
			sqrtss xmm0, sqs
			movss root, xmm0
		}
		return root;
	}

	inline float lengthSqr()
	{
		return (pow(this->xAechse, 2) + pow(this->yAechse, 2) + pow(this->zAechse, 2));
	}

	inline void clearVekt()
	{
		this->xAechse = this->yAechse = this->zAechse = 0.0f;
	}

	inline bool Vector3::operator==(const Vector3 &src) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.xAechse == xAechse) && (src.yAechse == yAechse) && (src.zAechse == zAechse);
	}

	inline Vector3 operator+(const Vector3 &v) const
	{
		return Vector3(this->xAechse + v.xAechse, this->yAechse + v.yAechse, this->zAechse + v.zAechse);
	}

	inline Vector3 operator-(const Vector3 &v) const
	{
		return Vector3(this->xAechse - v.xAechse, this->yAechse - v.yAechse, this->zAechse - v.zAechse);
	}

	inline Vector3 operator*(const Vector3 &v) const
	{
		return Vector3(this->xAechse * v.xAechse, this->yAechse * v.yAechse, this->zAechse * v.zAechse);
	}

	inline Vector3 operator*(float f) const
	{
		return Vector3(this->xAechse * f, this->yAechse * f, this->zAechse * f);
	}

	inline Vector3 operator/(const Vector3 &v) const
	{
		return Vector3(this->xAechse / v.xAechse, this->yAechse / v.yAechse, this->zAechse / v.zAechse);
	}

	inline Vector3 operator/(float f) const
	{
		return Vector3(this->xAechse / f, this->yAechse / f, this->zAechse / f);
	}

	inline Vector3 &operator+=(const Vector3 &v)
	{
		this->xAechse += v.xAechse;
		this->yAechse += v.yAechse;
		this->zAechse += v.zAechse;

		return *this;
	}

	inline Vector3 &operator-=(const Vector3 &v)
	{
		this->xAechse -= v.xAechse;
		this->yAechse -= v.yAechse;
		this->zAechse -= v.zAechse;

		return *this;
	}

	inline Vector3 &operator*=(const Vector3 &v)
	{
		this->xAechse *= v.xAechse;
		this->yAechse *= v.yAechse;
		this->zAechse *= v.zAechse;

		return *this;
	}

	inline Vector3 &operator/=(const Vector3 &v)
	{
		this->xAechse /= v.xAechse;
		this->yAechse /= v.yAechse;
		this->zAechse /= v.zAechse;

		return *this;
	}

	inline Vector3 &operator*=(float f)
	{
		this->xAechse *= f;
		this->yAechse *= f;
		this->zAechse *= f;

		return *this;
	}

	inline Vector3 &operator/=(float f)
	{
		this->xAechse /= f;
		this->yAechse /= f;
		this->zAechse /= f;

		return *this;
	}

	inline bool operator==(const Vector3 &src)
	{
		return (src.xAechse == xAechse) && (src.yAechse == yAechse) && (src.zAechse == zAechse);
	}

	inline bool operator!=(const Vector3 &src)
	{
		return (src.xAechse != xAechse) || (src.yAechse != yAechse) || (src.zAechse != zAechse);
	}

	inline bool operator!()
	{
		return !xAechse && !yAechse && !zAechse;
	}

	inline Vector3 &operator=(Vector3 &&v)
	{
		if (this != &v)
		{
			xAechse = 0;
			yAechse = 0;
			zAechse = 0;

			xAechse = v.xAechse;
			yAechse = v.yAechse;
			zAechse = v.zAechse;

			v.xAechse = 0;
			v.yAechse = 0;
			v.zAechse = 0;
		}

		return *this;
	}

	inline Vector3 &operator=(const Vector3 &v)
	{
		if (this != &v)
		{
			xAechse = v.xAechse;
			yAechse = v.yAechse;
			zAechse = v.zAechse;
		}

		return *this;
	}
};

inline Vector3 operator*(float fl, const Vector3& v) { return v * fl; }

class __declspec(align(16)) MeinVektAligned : public Vector3
{
public:

	MeinVektAligned() {}
	MeinVektAligned(float xAechse, float yAechse, float zAechse)
	{
		this->xAechse = xAechse;
		this->yAechse = yAechse;
		this->zAechse = zAechse;
	}

	explicit MeinVektAligned(const Vector3 &v)
	{
		this->xAechse = v.xAechse;
		this->yAechse = v.yAechse;
		this->zAechse = v.zAechse;
	}

	inline MeinVektAligned &operator=(const Vector3 &v)
	{
		this->xAechse = v.xAechse;
		this->yAechse = v.yAechse;
		this->zAechse = v.zAechse;

		return *this;
	}

	inline MeinVektAligned &operator=(const MeinVektAligned &v)
	{
		_mm_store_ps(this->base(), _mm_load_ps(v.base()));

		return *this;
	}

	float w;
};

inline void VectorMA(const Vector3 &start, float scale, const Vector3 &direction, Vector3 &dest)
{
	CHECK_VALID(start);
	CHECK_VALID(direction);

	dest.xAechse = start.xAechse + scale * direction.xAechse;
	dest.yAechse = start.yAechse + scale * direction.yAechse;
	dest.zAechse = start.zAechse + scale * direction.zAechse;
}

typedef unsigned __int32		uint32;

inline uint32 const FloatBits(const float &f)
{
	union Convertor_t
	{
		float f;
		uint32 ul;
	}tmp;
	tmp.f = f;
	return tmp.ul;
}

inline bool IsFinite(const float &f)
{
#if _X360
	return f == f && fabs(f) <= FLT_MAX;
#else
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
#endif
}

inline void VectorMultiply(const Vector3 &a, float b, Vector3 &c)
{
	CHECK_VALID(a);
	Assert(IsFinite(b));
	c.xAechse = a.xAechse * b;
	c.yAechse = a.yAechse * b;
	c.zAechse = a.zAechse * b;
}

inline void CrossProduct(const Vector3 &a, const Vector3 &b, Vector3 &result)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	Assert(&a != &result);
	Assert(&b != &result);
	result.xAechse = a.yAechse*b.zAechse - a.zAechse*b.yAechse;
	result.yAechse = a.zAechse*b.xAechse - a.xAechse*b.zAechse;
	result.zAechse = a.xAechse*b.yAechse - a.yAechse*b.xAechse;
}

inline void VectorAdd(const Vector3 &a, const Vector3 &b, Vector3 &c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.xAechse = a.xAechse + b.xAechse;
	c.yAechse = a.yAechse + b.yAechse;
	c.zAechse = a.zAechse + b.zAechse;
}

inline void VectorSubtract(const Vector3 &a, const Vector3 &b, Vector3 &c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.xAechse = a.xAechse - b.xAechse;
	c.yAechse = a.yAechse - b.yAechse;
	c.zAechse = a.zAechse - b.zAechse;
}

inline float VectorNormalize(Vector3 &v)
{
	float l = v.length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.xAechse = v.yAechse = v.zAechse = 0.0f;
	}
	return l;
}