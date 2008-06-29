#include "transporter.h"

void Game::run()
{
	input.run();
	visual.run(&scene.visual);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit Game::init(HINSTANCE instance)
{
	this->appInstance = instance;
	if(visual.init(this))
	{
		input.init(this);
		scene.init(this);
		return true;
	}
	return false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

Game::~Game()
{
	scene.cleanUp();
	input.stop();
	visual.stop();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

HINSTANCE Game::getAppInstance()
{
	return appInstance;
}