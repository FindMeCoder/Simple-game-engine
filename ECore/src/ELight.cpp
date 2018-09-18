
#include "ELight.h"

namespace E3D
{
	ELight::ELight(EInt lightId, LIGHT_TYPE type) :	id(lightId), lightOn(true), lightType(type), 
									ambient(255, 255, 255), diffuse(255, 255, 255), specular(255, 255, 255),
									position(0,0,0), direction(0, 0, 1), kc(1.0f), kl(1.0f), kq(0.0f), 
									spot_inner(45.0f), spot_outer(60.0f), power(1.0f), shadowFactor(0.05f){}

	

	std::vector<ELight*> *GLights = NULL;

	EInt CreateLight(LIGHT_TYPE lightType)
	{
		if (GLights == NULL)
			GLights = new std::vector<ELight*>;

		if ((EInt)GLights->size() > MAX_LIGHTS)
			return -1;

		EInt id = (EInt)GLights->size();
		GLights->push_back(new ELight(id, lightType));

		return id;
	}

	

	// 获得一个灯光, 如果灯光不存在, 则创建一个新灯光并返回
	ELight* GetLight(EInt id)
	{
		if ( GLights == NULL ||
			(EInt)GLights->size() > MAX_LIGHTS ||
			id < 0 || 
			id > (EInt)GLights->size() - 1)
			return NULL;

		return GLights->at(id);
	}

	

	EInt GetLightSize()
	{
		return GLights->size();
	}
	void DestoryAllLights()
	{
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
			SafeDelete((*itr));

		GLights->clear();
		SafeDelete(GLights);
	}
}