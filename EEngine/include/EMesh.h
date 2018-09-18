
#pragma once

#include "ECommon.h"
#include "ECore.h"
#include "EMaterial.h"
#include "EObject.h"
//参考orge开源引擎的.mesh格式
namespace E3D
{
	enum COLLSION_TYPE
	{
		COLLSION_COLLSION,
		COLLSION_IGNORE,
	};

	class EMesh : public EObject
	{
	public:
		~EMesh();

		void setScale(const EVector3D &scale);
		void setPosition(const EVector3D &pos);
		EVector3D getPosition() const;
		void move(const EVector3D &mov);

		// 给定点是否在Mesh的包围盒内, 如果是则返回true, 否则返回false
		EBool intersect(const EVector3D &point);

		void setVisible(EBool visible) { mIsVisible = visible;}
		EBool isVisible() const { return mIsVisible; }

		// 是否需要执行剔除操作
		void setCullFlag(EBool needCull) {mObject->needCull = needCull; }

		// 绕Y轴旋转
		void yaw(EFloat yDegree);
		// 绕Z轴旋转
		void roll(EFloat rDegree);
		// 绕X轴旋转
		void pitch(EFloat pDegree);

		void setCollsionType(COLLSION_TYPE type) { mCollsionType = type; }
		COLLSION_TYPE getCollsionType() const { return mCollsionType; }

		EMaterial *getMaterial(EInt index = 0);
		void setMateria(EInt index, EString matName);

		EObject4D* getObject() const { return mObject; }
		EInt getPolyonNumber() const { return mPolyonNumber; }
		EInt getVertexNumber() const { return mVertexNumber; }
		EInt getSubmeshNumber() const { return mSubmeshNumber; }

		// 获取各轴的旋转分量, 以角度表示
		EVector3D getRotateVec() const { return mRotate; }

		// 对此模型进行clone, 避免再次读盘读取模型
		EMesh *clone();

	protected:
		friend class ESceneManager;
		EMesh();
		EMesh(const EString &name, const EString &meshName);

		EObject4D		*mObject;

		EInt			mPolyonNumber;	// 模型多边形个数
		EInt			mVertexNumber;	// 模型顶点数

		EInt			mSubmeshNumber;	// 模型子模型个数

		EVector3D		mRotate;		// 模型各轴的旋转分量

		EBool			mIsVisible;

		COLLSION_TYPE	mCollsionType;
	};
}