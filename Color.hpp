#pragma once

class Color
{
public:

	Color()
	{
		*((int *)this) = 0;
	}

	Color(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 0);
	}

	Color(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	void SetColor(int _r, int _g, int _b, int _a = 0)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int &_r, int &_g, int &_b, int &_a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	static Color DWORD2RGBA(DWORD col)
	{
		return Color(((col >> 16) & 0xFF), ((col >> 8) & 0xFF), (col & 0xFF), ((col >> 24) & 0xFF));
	}

	void SetRawColor(int color32)
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	inline float *getBase()
	{
		float clr[3];

		clr[0] = _color[0] / 255.0f;
		clr[1] = _color[1] / 255.0f;
		clr[2] = _color[2] / 255.0f;

		return &clr[0];
	}

	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=(const Color &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

	static Color Red() { return Color(255, 0, 0, 255); }
	static Color Green() { return Color(70, 255, 70, 255); }
	static Color Blue() { return Color(0, 0, 255, 255); }
	static Color LightBlue() { return Color(50, 160, 255, 255); }
	static Color Purple() { return Color(220, 0, 220, 255); }

private:
	unsigned char _color[4];
};