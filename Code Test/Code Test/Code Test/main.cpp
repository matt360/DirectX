#include <iostream>
#include <stdio.h>
#include <math.h>

#pragma region Q2
//void Foo(int x)
//{
//	int& a;
//	int returnValue = 0;
//	for (int i = 0; i < x i++)
//	{
//		returnValue += i;
//	}
//}
//int main()
//{
//	int num;
//	num = Foo(10);
//	return 0;
//}  
#pragma endregion

#pragma region Q5

class Vector3
{
public:
	Vector3()
	{}

	Vector3(float fX, float fY, float fZ)
		: m_fX(fX)
		, m_fY(fY)
		, m_fZ(fZ)
	{}

	float GetMagnitude() const;
	void Normalise();
	static float GetAngleBetween(const Vector3& xVec1, const Vector3& xVec2);
	static Vector3 GetPerpVector(const Vector3& xVec1, const Vector3& xVec2);

	float m_fX;
	float m_fY;
	float m_fZ;
};
// Part (a) - write your implementation here:
float Vector3::GetMagnitude() const
{
	return abs(sqrtf(powf(m_fX, 2) + powf(m_fY, 2) + powf(m_fZ, 2)));
}

// Part (b) - write your implementation here:
void Vector3::Normalise()
{
	float vectorLenght = GetMagnitude();
	// x = x / length
	// y = y / lenght
	// z = z / lenght
	m_fX /= vectorLenght;
	m_fY /= vectorLenght;
	m_fZ /= vectorLenght;
}
// Part (c) - write your implementation here:
float Vector3::GetAngleBetween(const Vector3& u, const Vector3& v)
{
	float numerator = (u.m_fX * v.m_fX) + (u.m_fY * v.m_fY) + (u.m_fZ * v.m_fZ);
	float lengthOfVector1 = abs(sqrtf(powf(u.m_fX, 2) + powf(u.m_fY, 2) + powf(u.m_fZ, 2)));
	float lengthOfVector2 = abs(sqrtf(powf(v.m_fX, 2) + powf(v.m_fY, 2) + powf(v.m_fZ, 2)));
	float denominator = lengthOfVector1 * lengthOfVector2;

	return (numerator)    // xVec1 dot xVec2
	           /          // divided 
	       (denominator); // length(xVec1) times lenght(xVec2)
}
// Part (d) - write your implementation here:
Vector3 Vector3::GetPerpVector(const Vector3& xVec1, const Vector3& xVec2)
{
	// cross product returns a perpendicular vector to the two input vectors
	Vector3 xResult;

	//	| i	 j	 k |
	//	|u1  u2  u3|   =   (u2 * v3 - u3 * v2)i - (u1 * v3 - u3 * v1)j + (u1 * v2 - u2 * v1)k = (x, y, z)
	//  |v1  v2  v3|
	//  
	//  or
	//
	//	| i	 j	 k |
	//	|u1  u2  u3|   =   (u2 * v3 - u3 * v2)i + (u3 * v1 - u1 * v3)j + (u1 * v2 - u2 * v1)k = (x, y, z)
	//  |v1  v2  v3|
	//
	//  or
	//
	//  u   v     u    v
	//  2 * 3  -  3 *  2       
	//  3 * 1  -  1 *  3
	//  1 * 2  -  2 *  1

	xResult.m_fX = (xVec1.m_fY * xVec2.m_fZ) - (xVec1.m_fZ * xVec2.m_fY);
	xResult.m_fY = (xVec1.m_fZ * xVec2.m_fX) - (xVec1.m_fX * xVec2.m_fZ);
	xResult.m_fZ = (xVec1.m_fX * xVec2.m_fY) - (xVec1.m_fY * xVec2.m_fX);

	return xResult;
}int main(int argc, char* argv[]){	std::cout << "Have " << argc << " arguments:" << std::endl;	for (int i = 0; i < argc; ++i)	{		std::cout << argv[i] << std::endl;	}	Vector3 u(1, 2, 3);	Vector3 v(4, 2, 6);	printf("u(%.1f, %.1f, %.1f)\n", u.m_fX, u.m_fY, u.m_fZ);	printf("v(%.1f, %.1f, %.1f)\n", v.m_fX, v.m_fY, v.m_fZ);	/*Vector3 p;	p = p.GetPerpVector(u, v);	printf("p(%.1f, %.1f, %.1f)\n", p.m_fX, p.m_fY, p.m_fZ);*/	u.Normalise();	v.Normalise();	printf("u(%.1f, %.1f, %.1f)\n", u.m_fX, u.m_fY, u.m_fZ);	printf("v(%.1f, %.1f, %.1f)\n", v.m_fX, v.m_fY, v.m_fZ);	float angle;	Vector3 q;	angle = q.GetAngleBetween(u, v);	printf("angle: %.4f\n", angle);	getchar();	return 0;}
#pragma endregion
