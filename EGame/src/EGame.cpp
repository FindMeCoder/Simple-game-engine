
#include "EGame.h"
#include "ECamera.h"
#include "ELight.h"
#include "EMesh.h"
#include "ESceneManager.h"
#include "EPlayerController.h"
#include "EGameManager.h"

namespace E3D
{
	const static EString map001 = "map001.map";
	const static EString map002 = "map002.map";

	

	EGame::EGame() : plane(NULL), showHelp(false), scene(NULL), 
		player(NULL), gameMgr(NULL), mLogicX(-1), mLogicZ(-1)
	{
		scene	= new ESceneManager();
		gameMgr = new EGameManager(scene);
		player	= gameMgr->getPlayerController();

		currentObjectNum = 1;
		visiblePolyons = 0;

		createScene();
	}

	

	void EGame::createScene()
	{
		ELight *light = scene->createLight(LIGHT_DIRECTION);
		light->diffuse = EColor(255, 255, 255);
		light->direction = EVector4D(-1, -1, -1);
		light->shadowFactor = 0.5f;
		light->power = 1.5f;

		gameMgr->loadMap(map001);

		plane = scene->createMesh("Object2", "Plane_2.mesh");
		plane->setPosition(EVector3D(30, 15.0f, 30));

		gameMgr->startGame();
	}

	

	bool EGame::keyPress(EInt key)
	{
		if (key == 'Y')
		{
			scene->enableLights(!scene->isLightsEnable());
		}
		else if (key == 'H')
		{
			showHelp = !showHelp;
		}
		else if (key == 'C')
		{
			if (gameMgr->getCurrentMapName() == map001)
				gameMgr->changeMap(map002);
			else
				gameMgr->changeMap(map001);
			plane	= scene->createMesh("Object2", "Plane_2.mesh");
			player	= gameMgr->getPlayerController();
		}
		else
		{
			player->keyPress(key);
		}

		return true;
	}

	

	bool EGame::keyRlease(EInt key)
	{
		player->keyRlease(key);
		return true;
	}

	

	void EGame::update()
	{
		static EFloat degree = 0;
		degree += 10.f;
		plane->yaw(degree + 10.f);
		plane->setPosition(EVector3D(30 * sin(degree * 0.01f), 15.0f, 30 * cos(degree * 0.01f)));

		gameMgr->update();
		scene->update();

		mLogicX = player->getPosition().x;
		mLogicZ = player->getPosition().z;
		gameMgr->getLogicXZ(mLogicX, mLogicZ, mLogicX, mLogicZ);
	}

	

	void EGame::onPaint()
	{
		showInfo();	
	}

	

	void EGame::showInfo()
	{
		currentObjectNum	= scene->getVisibleObjectNumber();
		visiblePolyons		= scene->getVisiblePolyonNumber();

		if (showHelp)
		{
			scene->drawString("1. W A S D 移动Tank", 10, 10);
			scene->drawString("2. <-   -> 瞄准", 10, 30);
			scene->drawString("3. Space   发射子弹", 10, 50);
			scene->drawString("4. F       切换线框/实体模式", 10, 70);
			scene->drawString("5. Y       开启/关闭灯光", 10, 90);
			scene->drawString("6. G       切换子弹", 10, 110);
			scene->drawString("7. C       切换场景", 10, 130);

			scene->drawString("当前物体数 : " + IntToString(scene->getVisibleObjectNumber()),650, 10, EColor(255, 255, 0));
			scene->drawString("所有面数 : " + IntToString(scene->getTotalPolyonNumber()), 650, 30, EColor(255, 255, 0));
			scene->drawString("可见面数 : " + IntToString(scene->getVisiblePolyonNumber()),650, 50, EColor(255, 255, 0));
		}
		else
			scene->drawString("H 显示帮助", 10, 10);

		scene->drawString("击毁敌人 - " + IntToString(gameMgr->getDestoryEnemyNumber()), 20, 510, EColor(255, 255, 0));
		scene->drawString("剩余敌人 - " + IntToString(gameMgr->getLastEnemyNumber()), 20, 530, EColor(255, 255, 0));

		scene->drawString("GridPos # " + IntToString(mLogicX) + " : " +IntToString(mLogicZ), 200, 510, EColor(255, 255, 0));
		
	}
}