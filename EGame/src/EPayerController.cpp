
#include "EPlayerController.h"
#include "EGameManager.h"

namespace E3D
{
	

	EPlayerController::EPlayerController(EGameManager *gameMgr) 
		: mCurDir(0), mGameMgr(gameMgr), mScene(gameMgr->getSceneManager()), mFireDir(0)
	{
		mCamera = mScene->getCamera();
		mTank = new ETank("Player", "Green", gameMgr);
	}

	

	EPlayerController:: ~EPlayerController()
	{
		SafeDelete(mTank);
	}

	

	void EPlayerController::setPosition(const EVector3D &pos)
	{
		// 由于坦克和摄像机被绑定到一起所以在修改坦克的位置时
		// 需要修改摄像机的位置和目标点
		mTank->setPosition(pos);
		mCamera->setPosition(pos + EVector3D(0, 25, 30));
		mCamera->setTarget(pos);
	}

	

	void EPlayerController::keyPress(EInt key)
	{
		if (key == 'W')
			mCurDir |= DIR_UP;
		else if (key == 'S')
			mCurDir |= DIR_DOWN;
		else if (key == 'A')
		{
			mCurDir |= DIR_LEFT;
			mTank->turnLeft();
		}
		else if (key == 'D')
		{
			mCurDir |= DIR_RIGHT;
			mTank->turnRight();
		}
		else if (key == VK_SPACE)
			mTank->fire();
		else if (key == VK_LEFT)
		{
			mFireDir = -1;
		}
		else if (key == VK_RIGHT)
		{
			mFireDir = 1;
			//mTank->fireAimRight();
		}
		else if (key == 'F')
		{
			if (mCamera->getRenderMode() == RENDER_WIRE)
				mCamera->setRenderMode(RENDER_SOILD);
			else
				mCamera->setRenderMode(RENDER_WIRE);
		}
		else if (key == 'G')
		{
			mTank->setBulletType(mTank->getBulletTye() == BULLET_ROCKET ? BULLET_BALL : BULLET_ROCKET);
		}
	}

	

	void EPlayerController::keyRlease(EInt key)
	{
		if (key == 'W')
			mCurDir &= ~DIR_UP;
		else if (key == 'S')
			mCurDir &= ~DIR_DOWN;
		else if (key == 'A')
			mCurDir &= ~DIR_LEFT;
		else if (key == 'D')
			mCurDir &= ~DIR_RIGHT;
		else if (key == VK_LEFT || key == VK_RIGHT)
			mFireDir = 0;
	}


	void EPlayerController::update()
	{
		if (mCurDir & DIR_UP)
		{
			mTank->moveUp();
			mCamera->move(mTank->getCameraStep());
		}
		else if(mCurDir & DIR_DOWN)
		{
			mTank->moveDown();
			mCamera->move(mTank->getCameraStep());
		}

		if (mFireDir == -1)
			mTank->fireAimLeft();
		else if (mFireDir == 1)
			mTank->fireAimRight();

		mTank->update();
	}
}