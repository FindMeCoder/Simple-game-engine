
#pragma once
#include "ECommon.h"
#include <map>
#include "EGraphics.h"

namespace E3D
{
	struct EMaterial
	{
		EString name;
		EColor	ambient;	// 环境色
		EColor	diffuse;	// 漫反射颜色
		EColor	specular;	// 镜面颜色

		EBitmap *bitmap;

		EMaterial();
		~EMaterial();
		inline bool useTexture() { return bitmap && bitmap->isValid(); }
		EColor getPixel(EFloat u, EFloat v);
	};

	typedef std::map<EString, EMaterial*>::iterator MaterialIter;
	extern std::map<EString, EMaterial*> *GMaterials;

	extern EMaterial *GetMaterial(const EString &name);
	extern bool SetMaterial(const EString &name, EMaterial *material);
	extern bool DestoryMaterial(const EString &name);
	extern void DestoryAllMaterials();

	// 解析材质脚本, 读取基本材质和贴图
	// 材质脚本必须和模型名称相同例如 : ogreHead.mesh ogreHead.material
	extern void ParseMaterialScript(const EString &scriptName);
}