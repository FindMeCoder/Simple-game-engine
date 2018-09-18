
#pragma once

#include "EObject.h"
#include "EGameManager.h"
#include "ESceneManager.h"

namespace E3D
{
	// 子弹类型
	class ETank;
	class EBullet : public EObject
	{
	public:
		virtual ~EBullet();

		void setScale(const EVector3D &scale) { mMesh->setScale(scale);}
		// 设置初始发射位置
		void setPosition(const EVector3D &pos);
		EVector3D getPosition() const { return mMesh->getPosition(); }
		// 设置初始发射方向
		void setDirection(const EVector3D &dir);
		// 延Y轴对子弹进行旋转
		void yaw(EFloat degree);

		void update();

		EBool isVisible() const { return mMesh->isVisible();}
		void setVisible(EBool visible) { mMesh->setVisible(visible); }

	protected:
		friend class EGameManager;
		EBullet(ETank *owner, const EString &meshName, EGameManager *scene);

	protected:
		EGameManager	*mGameMgr;
		EMesh			*mMesh;

		EVector3D		mDirection;		// 初始移动方向
		EFloat			mSpeed;			// 移动速度

		ETank			*mOwner;

		static EInt		mID;			// 计数ID
	};
}