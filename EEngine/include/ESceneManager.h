
#pragma once
#include "ECommon.h"
#include "EMesh.h"
#include "ELight.h"
#include "ECore.h"
#include "ECamera.h"
#include <map>

namespace E3D
{
	// 各种资源管理类
	class ESceneManager
	{
		typedef std::map<EString, EMesh*>::iterator MeshIter;

	public:
		ESceneManager();
		~ESceneManager();

		// 绘制文字
		void drawString(const EString &str, EInt x, EInt y, const EColor &c = EColor(255, 255, 255));

		// 创建一个模型实体
		EMesh* createMesh(const EString &name, const EString &meshName);
		// 获取指定的模型实体
		EMesh* getMesh(const EString &name);
		// 销毁指定的模型实体
		void destroyMesh(const EString&name);

		void clearMesh();

		// 指定开启/关闭灯光(全局)
		void enableLights(EBool enable);
		// 当前是否开启灯光(全局)
		EBool isLightsEnable() const { return mLightEnable; }
		// 创建一盏灯光
		ELight* createLight(LIGHT_TYPE lightType);
		// 获得一个灯光, 如果灯光不存在, 则返回NULL
		ELight*	getLight(EInt id);

		// 获取摄像机指针
		ECamera* getCamera(){ return mCamera;}

		// 获取所有(可见/不可见)物体个数
		EInt getTotalObjectNumber() const { return mMeshs.size(); }
		// 获取当前可见物体个数
		EInt getVisibleObjectNumber() const { return mVisibleObjectNumber; }
		// 获取当前可见多边形个数
		EInt getVisiblePolyonNumber() const { return mVisiblePolyonNumber; }
		// 获取所有(可见/不可见)多边形个数
		EInt getTotalPolyonNumber() const { return mTotalPolyonNumber; }

		// 创建地形
		EMesh* createTerrain(const EString& rawFileName, const EString &heightMapFileName, 
								EFloat uTitle = 1, EFloat vTitle = 1, EFloat blockSize = 5.0f);
		EMesh* getTerrain() { return mTerrainMesh; }

		// 更新
		void update();
		
	protected:
		std::map<EString, EMesh*>	mMeshs;
		std::map<EString, EMesh*>	mUsedMesh;
		ERenderList4D				*mRenderList;

		EMesh						*mTerrainMesh;

		ECamera						*mCamera;
		EBool						mLightEnable;

		EInt						mVisibleObjectNumber;
		EInt						mVisiblePolyonNumber;
		EInt						mTotalPolyonNumber;
	};
}