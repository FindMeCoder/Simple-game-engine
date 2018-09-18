
#include "ETank.h"
#include "EBullet.h"
#include "EGameManager.h"

namespace E3D
{
	ETank::ETank(const EString &name, const EString &meshName, EGameManager *gameMgr)
		: mGameMgr(gameMgr), mScene(gameMgr->getSceneManager()), mSpeed(0.75f), 
		mFireDir(0, 0, -1), mLastFire(0),mBulletType(BULLET_ROCKET), 
		mMoveVec(0,0,0), mColOffset(3.5f), mTankType(TANK_PLAYER)
	{
		mMaxLive		= 100;
		mCurrentLive	= 0;
		mFireFrequency	= 8;

		mName = name;
		mBody = mScene->createMesh(name + "_Body", meshName + "_Body.mesh");
		mHead = mScene->createMesh(name + "_Head", meshName + "_Head.mesh");
	}

	

	ETank::~ETank()
	{
		mScene->destroyMesh(mName + "_Body");
		mScene->destroyMesh(mName + "_Head");
	}


	

	void ETank::fire()
	{
		if (mGameMgr && mLastFire > mFireFrequency)
		{
			EBullet *bullet = mGameMgr->createBullet(this);
			bullet->setPosition(getPosition() + mFireDir * 5.0f + EVector3D(0, 2, 0));
			bullet->setDirection(mFireDir);
			bullet->yaw(mHead->getRotateVec().y);
			mLastFire = 0;

			mGameMgr->playSound(SOUND_FIRE);
		}
	}


	void ETank::update()
	{
		mLastFire++;
	}

	void ETank::setVisible(EBool visible) 
	{ 
		mBody->setVisible(visible); 
		mHead->setVisible(visible);
	}

	

	void ETank::move(const EVector3D &mov)
	{
		mBody->move(mov);
		mHead->move(mov);
	}

	

	EBool ETank::moveUp()
	{
		// �����߼�����ǰ�ƶ�
		// �Ե�ǰtank�ĳ���Ϊǰ
		EInt degree = mBody->getRotateVec().y;
		EInt dir = degree > 0 ? 1 : -1;
		degree = Abs(degree) % 360;

		const static EInt offset = 3;
		EFloat xOff = 0, zOff = 0;	// tank�����������ƫ����

		EVector3D pos = getPosition();	// ���յķ���
		if (degree < 5)
		{
			mMoveVec = EVector3D(0, 0, -mSpeed);
			pos.z	 = pos.z - mColOffset;
			xOff	 = offset;
		}
		else if (degree < 95)
		{
			mMoveVec = EVector3D(-mSpeed * dir, 0, 0);
			pos.x	 = pos.x - mColOffset * dir;
			zOff	 = offset;
		}
		else if (degree < 185)
		{
			mMoveVec = EVector3D(0, 0, mSpeed);
			pos.z	 = pos.z + mColOffset;
			xOff	 = offset;
		}
		else
		{
			mMoveVec = EVector3D(mSpeed * dir, 0, 0);
			pos.x	 = pos.x + mColOffset * dir;
			zOff	 = offset;
		}

		// ���ȼ���Ƿ����ϰ�����ֹ
		// ��tank�������Ҳ෢����������
		if (mGameMgr->canGo(pos.x + mMoveVec.x + xOff, pos.z + mMoveVec.z + zOff) &&
			mGameMgr->canGo(pos.x + mMoveVec.x - xOff, pos.z + mMoveVec.z - zOff))
		{
			move(mMoveVec);
			return true;
		}
		else	// ��, ��ô̹�˺������ƫ������Ϊ0
		{
			mMoveVec = EVector3D::ZERO;
			return false;
		}
	}

	

	EBool ETank::moveDown()
	{
		// �����߼�������ƶ�
		// �Ե�ǰtank�ĳ���ķ�����Ϊ��
		EInt degree = mBody->getRotateVec().y;
		EInt dir = degree > 0 ? 1 : -1;
		degree = Abs(degree) % 360;

		const static EInt offset = 3;
		EFloat xOff = 0, zOff = 0;	// tank�����������ƫ����

		EVector3D pos = getPosition();
		if (degree < 5)
		{
			mMoveVec = EVector3D(0, 0, mSpeed);
			pos.z	 = pos.z + mColOffset;
			xOff	 = offset;
		}
		else if (degree < 95)
		{
			mMoveVec = EVector3D(mSpeed * dir, 0, 0);
			pos.x	 = pos.x + mColOffset * dir;
			zOff	 = offset;
		}
		else if (degree < 185)
		{
			mMoveVec = EVector3D(0, 0, -mSpeed);
			pos.z	 = pos.z - mColOffset;
			xOff	 = offset;
		}
		else
		{
			mMoveVec = EVector3D(-mSpeed * dir, 0, 0);
			pos.x	 = pos.x - mColOffset * dir;
			zOff	 = offset;
		}

		// ���ȼ���Ƿ����ϰ�����ֹ
		if (mGameMgr->canGo(pos.x + mMoveVec.x + xOff, pos.z + mMoveVec.z + zOff) &&
			mGameMgr->canGo(pos.x + mMoveVec.x - xOff, pos.z + mMoveVec.z - zOff))
		{
			move(mMoveVec);
			return true;
		}
		else	// ��, ��ô̹�˺������ƫ������Ϊ0
		{
			mMoveVec = EVector3D::ZERO;
			return false;
		}
	}

	

	void ETank::onHited()
	{
		
	}

	

	EBool ETank::intersect(const EVector3D &point)
	{
		return mHead->intersect(point) || mBody->intersect(point);
	}

	

	void ETank::updateFireDir(EFloat degree)
	{
		EMatrix44 mat;
		GetRotateMatrix44Y(mat, degree);
		GetVector3DMulMatrix44(mFireDir, mat, mFireDir);
	}

	

	void ETank::fireAimLeft()
	{
		updateFireDir(-4.0f);
		mHead->yaw(mHead->getRotateVec().y - 4.0f);
	}

	

	void ETank::fireAimRight()
	{
		updateFireDir(4.0f);
		mHead->yaw(mHead->getRotateVec().y + 4.0f);
	}

	

	void ETank::setPosition(const EVector3D &pos) {
		mBody->setPosition(pos);
		mHead->setPosition(pos);
	}

	

	void ETank::yaw(EFloat degree)
	{
		mBody->yaw(degree);
		mHead->yaw(degree);
	}

	

	void ETank::turnLeft()
	{
		//updateFireDir(-90.0f);
		//mHead->yaw(mHead->getRotateVec().y - 90.0f);
		mBody->yaw(mBody->getRotateVec().y - 90.0f);
	}

	

	void ETank::turnRight()
	{
		//updateFireDir(90.0f);
		//mHead->yaw(mHead->getRotateVec().y + 90.0f);
		mBody->yaw(mBody->getRotateVec().y + 90.0f);
	}

	
}