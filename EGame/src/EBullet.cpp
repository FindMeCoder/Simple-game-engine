
#include "EBullet.h"
#include "ETank.h"

namespace E3D
{
	

	EInt EBullet::mID = 0;

	

	EBullet::EBullet(ETank *owner, const EString &meshName, EGameManager *gameMgr) 
		: mOwner(owner), mGameMgr(gameMgr), mDirection(0, 0, 0), mSpeed(1.75f)
	{
		mMaxLive	 = 100;
		mCurrentLive = 0;

		mName = "Bullet" + IntToString(mID++);
		mMesh = mGameMgr->getSceneManager()->createMesh(mName, meshName);
	}

	

	EBullet::~EBullet()
	{
		mGameMgr->getSceneManager()->destroyMesh(mName);
	}

	

	void EBullet::setPosition(const EVector3D &pos)
	{
		mMesh->setPosition(pos);
	}

	

	void EBullet::setDirection(const EVector3D &dir)
	{
		mDirection = dir;
	}

	

	void EBullet::yaw(EFloat degree)
	{
		mMesh->yaw(degree);
	}

	void EBullet::update()
	{
		mCurrentLive ++;
		mMesh->setPosition(mMesh->getPosition() + mDirection * mSpeed);

		// �����ȡ�ӵ�ͷ��λ��
		EVector3D pos = getPosition() + mDirection * 3.0f;
		// ����Ƿ����Ŀ��
		// ���ȼ�⾲̬����
		EMesh* obj = mGameMgr->checkObject(pos.x, pos.z);
		if (obj)
		{
			mGameMgr->playSound(SOUND_EXPLODE);
			mGameMgr->destroyBullet(this);
		}
		else
		{
			// ���û�о�̬����, ���tank
			ETank *tank = mGameMgr->checkTank(pos.x, pos.y - 1.5f, pos.z);
			if (tank && tank->getTankType() != mOwner->getTankType())
			{
				// ������
				tank->onHited();
				mGameMgr->playSound(SOUND_EXPLODE);
				mGameMgr->destroyBullet(this);
			}
		}
	}
}