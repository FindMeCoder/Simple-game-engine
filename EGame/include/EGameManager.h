
#pragma once
#include "ECommon.h"
#include <list>
#include <vector>

namespace E3D
{
	enum SOUND_YYPE
	{
		SOUND_BACKGROUND,
		SOUND_FIRE,
		SOUND_EXPLODE,
	};

	class EMesh;
	class EBullet;
	class ETank;
	class EPlayerController;
	class ESceneManager;
	class EGameManager
	{
		typedef std::list<EBullet*>::iterator BulletIter;
		typedef std::list<ETank*>::iterator TankIter;

	public:
		EGameManager(ESceneManager *scene);
		~EGameManager();

		// 开始游戏
		void startGame(EInt maxEnemyNumber = 10);
		EBool isGameBegin() const { return mMaxEnemyNumber > 0; }
		// 是否结束游戏
		EBool finishGame() const;

		// 击毁敌人数目
		EInt getDestoryEnemyNumber() const { return mCurrentEnemyNumber;}
		// 剩余敌人数目
		EInt getLastEnemyNumber() const { return mMaxEnemyNumber - mCurrentEnemyNumber;}

		// 创建子弹
		EBullet* createBullet(ETank *owner);
		void destroyBullet(EBullet *bullet);
		// 创建AI坦克
		ETank *createAITank(const EVector3D &pos);
		// 获取当前角色坦克的控制器
		EPlayerController *getPlayerController() {return mMainPlayer; }

		// 播放声音
		void playSound(SOUND_YYPE sound, EBool loop = false);

		// 获得场景管理器
		ESceneManager *getSceneManager() { return mScene; }

		// 加载场景地图
		EBool loadMap(const EString& mapName);
		void changeMap(const EString& mapName);
		EString getCurrentMapName() const { return mCurMap; }
		// 获取地图的高宽
		EFloat getMapWidth() const { return mXL; }
		EFloat getMapHeight() const { return mZL; }

		// 通过canGo, checkObject来模拟简单的碰撞效果
		// XZ平面坐标中x, z坐标是否可通过
		EBool canGo(EFloat x, EFloat z);
		void getLogicXZ(EFloat x, EFloat z, EInt &outX, EInt &outZ);
		// 给定x,z坐标, 返回在此坐标的物体(静态物体, 不包括子弹或者坦克)
		EMesh* checkObject(EFloat x, EFloat z);
		// 给定x,y,z坐标, 查看是否击中tank, 没有集中返回NULL
		ETank* checkTank(EFloat x, EFloat y, EFloat z);

		// 更新
		void update();

	protected:
		ESceneManager					*mScene;
		std::list<EBullet*>				mBullets;
		std::list<ETank*>				mTanks;

		EPlayerController				*mMainPlayer;

		EString							mCurMap;

		EFloat							mBlockSize;
		EInt							mX, mZ;
		EFloat							mHXL, mHZL;	// map宽, 高
		EFloat							mXL, mZL;	// map宽/2 高/2
		std::vector<std::vector<EInt>>	mCollisionValue;	// 存储当前加载地图的障碍物
		std::vector<std::vector<EMesh*>>mCollisionMeshs;

		EMesh							*mTerrainMesh;
		EInt							mMaxEnemyNumber;	// 最大敌人数目
		EInt							mCurrentEnemyNumber; // 当前击毁敌人数
		EInt							mVisibleEnemyNumber; // 一次最多显示的敌人数目
	};
}