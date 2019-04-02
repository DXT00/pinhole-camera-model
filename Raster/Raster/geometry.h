#pragma once
#include <cstdlib> 
#include <cstdio> 
#include <iostream> 
#include <iomanip> 
#include <cmath> 
#include <fstream>

using namespace std;

template<typename T>
class vec2
{
public:
	vec2() : x(0), y(0) {}
	vec2(T xx) : x(xx), y(xx) {}
	vec2(T xx, T yy) : x(xx), y(yy) {}
	vec2 operator + (const vec2 &v) const
	{
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator / (const T &r) const
	{
		return vec2(x / r, y / r);
	}
	vec2 operator * (const T &r) const
	{
		return vec2(x * r, y * r);
	}
	vec2& operator /= (const T &r)
	{
		x /= r, y /= r; return *this;
	}
	vec2& operator *= (const T &r)
	{
		x *= r, y *= r; return *this;
	}
	friend std::ostream& operator << (std::ostream &s, const vec2<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ']';
	}
	friend vec2 operator * (const T &r, const vec2<T> &v)
	{
		return vec2(v.x * r, v.y * r);
	}
	T x, y;

};

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;

template<typename T>
class vec3
{
public:
	vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	vec3(T xx) : x(xx), y(xx), z(xx) {}
	vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	vec3 operator + (const vec3 &v) const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	vec3 operator - (const vec3 &v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}
	vec3 operator - () const
	{
		return vec3(-x, -y, -z);
	}
	vec3 operator * (const T &r) const
	{
		return vec3(x * r, y * r, z * r);
	}
	vec3 operator * (const vec3 &v) const
	{
		return vec3(x * v.x, y * v.y, z * v.z);
	}
	T dotProduct(const vec3<T> &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	vec3& operator /= (const T &r)
	{
		x /= r, y /= r, z /= r; return *this;
	}
	vec3& operator *= (const T &r)
	{
		x *= r, y *= r, z *= r; return *this;
	}
	vec3 crossProduct(const vec3<T> &v) const
	{
		return vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	T norm() const
	{
		return x * x + y * y + z * z;
	}
	T length() const
	{
		return sqrt(norm());
	}

	const T& operator [] (uint8_t i) const { return (&x)[i]; }
	T& operator [] (uint8_t i) { return (&x)[i]; }
	vec3& normalize()
	{
		T n = norm();
		if (n > 0) {
			T factor = 1 / sqrt(n);
			x *= factor, y *= factor, z *= factor;
		}

		return *this;
	}

	friend vec3 operator * (const T &r, const vec3 &v)
	{
		return vec3<T>(v.x * r, v.y * r, v.z * r);
	}
	friend vec3 operator / (const T &r, const vec3 &v)
	{
		return vec3<T>(r / v.x, r / v.y, r / v.z);
	}

	friend std::ostream& operator << (std::ostream &s, const vec3<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
	}

	T x, y, z;
};

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;

template<typename T>
class Matrix44
{
public:

	T x[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };

	Matrix44() {}
	Matrix44(T a, T b, T c, T d, T e, T f, T g, T h,
		T i, T j, T k, T l, T m, T n, T o, T p)
	{
		x[0][0] = a;
		x[0][1] = b;
		x[0][2] = c;
		x[0][3] = d;
		x[1][0] = e;
		x[1][1] = f;
		x[1][2] = g;
		x[1][3] = h;
		x[2][0] = i;
		x[2][1] = j;
		x[2][2] = k;
		x[2][3] = l;
		x[3][0] = m;
		x[3][1] = n;
		x[3][2] = o;
		x[3][3] = p;
	}
	Matrix44(const Matrix44 &mat) {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				x[i][j] = mat[i][j];
			}
		}
		//return *this;
	}
	const T *operator [](unsigned int i)const { return x[i]; }
	T *operator [](unsigned int i) { return x[i]; }

	Matrix44 operator * (const Matrix44& v) const
	{
		Matrix44 tmp;
		multiply(*this, v, tmp);

		return tmp;
	}

	static void multiply(const Matrix44<T> &a, const Matrix44& b, Matrix44 &c) {
		for (uint8_t i = 0; i < 4; ++i) {
			for (uint8_t j = 0; j < 4; ++j) {
				c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
					a[i][2] * b[2][j] + a[i][3] * b[3][j];
			}
		}
	}
	Matrix44 transposed() const
	{

		Matrix44 t;
		for (uint8_t i = 0; i < 4; ++i) {
			for (uint8_t j = 0; j < 4; ++j) {
				t[i][j] = x[j][i];
			}
		}
		return t;
	}
	template<typename S>
	void multVecMatrix(const vec3<S> &src, vec3<S> &dst)const {

		S a, b, c, w;
		a = src.x*x[0][0] + src.y*x[1][0] + src.z*x[2][0] + x[3][0];
		b = src.x*x[0][1] + src.y*x[1][1] + src.z*x[2][1] + x[3][1];
		c = src.x*x[0][2] + src.y*x[1][2] + src.z*x[2][2] + x[3][2];
		w = src.x*x[0][3] + src.y*x[1][3] + src.z*x[2][3] + x[3][3];

		dst.x = a / w;
		dst.y = b / w;
		dst.z = c / w;

	}

	Matrix44 swap(int row1, int row2) const {
		Matrix44 res(*this);
		T tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp = res[row1][i];
			res[row1][i] = res[row2][i];
			res[row2][i] = tmp;
		}
		return res;
	}

	void swap(int row1, int row2) {
		T tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp = x[row1][i];
			x[row1][i] = x[row2][i];
			x[row2][i] = tmp;
		}
	}
	int findMaxInCol(int colIndex)const {//找出第colIndex列中最大值所在的行号（rowIndex）
		T Max = fabs(x[0][colIndex]);
		int rowIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			if (fabs(x[i][colIndex]) > Max) {
				Max = fabs(x[i][colIndex]);
				rowIndex = i;
			}
		}
		return rowIndex;
	}



	Matrix44 inverse() const {
		Matrix44 x_(*this);
		Matrix44 invMat;
		for (int j = 0; j < 4; j++)
		{
			if (x_[j][j] == 0) {
				int maxRowIndex = findMaxInCol(j);
				if (maxRowIndex == j) {
					perror("It is singular matrix,cannot be inversed!");
					break;
				}
				x_.swap(j, maxRowIndex);
				invMat.swap(j, maxRowIndex);

			}
			T pivot = x_[j][j];//pivot一定不等于0
			for (int i = 0; i < 4; i++)
			{
				if (i == j)continue;
				T cof = -x_[i][j] / pivot;

				for (int col = 0; col < 4; col++)
				{
					x_[i][col] = x_[i][col] + cof * x_[j][col];//0
					invMat[i][col] = invMat[i][col] + cof * invMat[j][col];
				}

				x_[i][j] = 0;
			}
			for (int col = 0; col < 4; col++)
			{
				x_[j][col] /= pivot;
				invMat[j][col] /= pivot;

			}

		}

		cout << x_;

		return invMat;

	}

	friend ostream& operator <<(ostream &s, const Matrix44<T> &m) {
		
		for (int i = 0; i < 4; i++)
		{
		
			for (int j = 0; j < 4; j++)
			{
				
				s << setprecision(5) << m[i][j] << " ";

			}
			s << endl;
		}
		
		return s;
	}
};


typedef Matrix44<float> Matrix44f;
