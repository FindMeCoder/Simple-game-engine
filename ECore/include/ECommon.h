
#pragma once

#include <cmath>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <iostream>
#include <cmath>

#include "E3DHeader.h"

namespace E3D
{
	//���Ͷ���
	typedef char			EChar;
	typedef short			EShort;
	typedef float			EFloat;
	typedef int				EInt;
	typedef bool			EBool;
	typedef	long			ELong;
	typedef double			EDouble;
	typedef std::string		EString;
	typedef std::wstring	EWSring;

	typedef	unsigned char	EUChar;
	typedef unsigned short	EUShort;
	typedef unsigned int	EUInt;

	// ���ڴ�С
	const EInt SCREEN_WIDTH		= 800;
	const EInt SCREEN_HEIGHT	= 600;

	// Ĭ�ϵ�����
	const EString	DEFAULT_NAME = "E3D_Default";

	const EString	DefaultPath = "Media/";//�ļ�Ĭ��·��
	static EString GetPath(const EString& file){ return DefaultPath + file;}

	// ��ѧ����
	#define PI 3.141592654f
	#define PI2 (PI * 2.0f)
	#define PI_DIV_2 (PI * 0.5f)
	#define PI_DIV_4 (PI * 0.25f)
	#define PI_INV 0.318309886f

	// ���㾫��
	#define EPSILON_E6 ((EFloat)(1E-6))

	// �Ƕȱ仡��ת��, ���ȱ�Ƕ�ת��
	#define Degree_TO_Radian(deg) (deg * 0.017453f)
	#define Radian_To_Degree(rad) (rad * 57.295780f)

	#define Abs(a) ((a) > 0 ? (a) : -(a))

	#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
	#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

	// ��ȫɾ��
	#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = 0; }
	#define SafeDeleteArray(ptr) if (ptr) { delete [] ptr; ptr = 0; }

	#define COUT(a) std::cout<< #a <<" : "<< (a) << std::endl;

	#define ECOLOR_16BIT(r, g, b) (((r) & 0xff) << 16 | ((g) & 0xff) << 8 | (b) & 0xff)
	
	struct EColor
	{
		EUChar r , g, b, a;
		EColor(EInt color, EUChar alpha = 255) 
		{ 
			r = (unsigned char)((color & 0xff0000) >> 16);
			g = (unsigned char)((color & 0x00ff00) >> 8);
			b = (unsigned char)(color & 0x0000ff);
			a = alpha;
		}
		EColor(EUChar ri = 0, EUChar gi = 0, EUChar bi = 0, EUChar ai = 255) : 
		r(ri), g(gi), b(bi), a(ai){}

		EInt ToInt() const { return ECOLOR_16BIT(r, g, b); }
		EColor operator *(const EColor &c) const
		{
			EInt ri = r * c.r >> 8;	// r * c.r / 256
			EInt gi = g * c.g >> 8;
			EInt bi = b * c.b >> 8;
			EInt ai = a * c.a >> 8;
			return EColor(ri, gi, bi, ai);
		}
	};

}