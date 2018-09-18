
#pragma once

#include "ECommon.h"
#include "EVector.h"

namespace E3D
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT	= 0,	// 环境光
		LIGHT_DIRECTION = 1,	// 平行光
		LIGHT_POINT		= 2,	// 点灯光
	};

	struct ELight
	{
		EInt		id;			// 灯光的分配id, 全局唯一
		EBool		lightOn;	// 灯光是否启用

		LIGHT_TYPE	lightType;	// 灯光类型

		EFloat		power;			// 灯光的强度, 默认为1.0f
		EFloat		shadowFactor;	// 灯光背面强度, 默认为0.05f

		EColor		ambient;	// 灯光的环境色(环境光使用)
		EColor		diffuse;	// 灯光漫反射颜色
		EColor		specular;	// 灯光高光颜色

		EVector4D	position;	// 灯光世界坐标位置
		EVector4D	direction;	// 光源朝向, 点灯光无效

		EVector4D	transPosition;	// 灯光摄像机坐标系位置
		EVector4D	transDirection;	// 摄像机坐标系光源朝向, 点灯光无效

		EFloat		kc, kl, kq;	// 衰减因子

		EFloat		spot_inner;	// 聚光灯内锥角
		EFloat		spot_outer;	// 聚光灯外锥角
		EFloat		pf;			// 聚光灯指数因子

		ELight(EInt lightId, LIGHT_TYPE type);
	};

	// 最大灯光数
	#define MAX_LIGHTS 8

	// 全局灯光管理
	extern std::vector<ELight*> *GLights;

	typedef std::vector<ELight*>::iterator LightItr;

	// 创建一个新的灯光, 创建成功返回灯光id(全局唯一), 创建失败则返回-1
	// 场景内最大光源数为MAX_LIGHTS
	extern EInt		CreateLight(LIGHT_TYPE lightType);
	// 获得一个灯光, 如果灯光不存在, 则返回NULL
	extern ELight*	GetLight(EInt id);
	extern EInt		GetLightSize();
	// 清楚全部光源
	extern void		DestoryAllLights();

}