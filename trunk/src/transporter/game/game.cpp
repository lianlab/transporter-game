#include "transporter.h"

void Game::run()
{
	isRunning = true;
	inputSystem.run();
	physicsSystem.run(&gameScene.physicsWorld);
	visualSystem.run(&gameScene.visualWorld);	
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit Game::init(HINSTANCE instance)
{
	this->appInstance = instance;
	if(visualSystem.init(this))
	{
		bit result = true;
		result &= inputSystem.init(this);
		result &= physicsSystem.init(this);
		result &= gameScene.init(this);
		return result;
	}
	return false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

Game::Game()
{
	isRunning = false;
}

Game::~Game()
{	
	stop();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

HINSTANCE Game::getAppInstance()
{
	return appInstance;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void Game::stop()
{
	if(isRunning)
	{
		inputSystem.stop();
		physicsSystem.stop();
		visualSystem.stop();

		gameScene.cleanUp();
		isRunning = false;
	}
}
