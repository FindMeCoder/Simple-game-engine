
#pragma once
#include "ECommon.h"
#include <map>
#include "EGraphics.h"

namespace E3D
{
	struct EMaterial
	{
		EString name;
		EColor	ambient;	// ����ɫ
		EColor	diffuse;	// ��������ɫ
		EColor	specular;	// ������ɫ

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

	// �������ʽű�, ��ȡ�������ʺ���ͼ
	// ���ʽű������ģ��������ͬ���� : ogreHead.mesh ogreHead.material
	extern void ParseMaterialScript(const EString &scriptName);
}