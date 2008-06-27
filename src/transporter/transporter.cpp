#include "transporter.h"

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{	
	Game transporter;
	if(transporter.init(hInst))
	{
		transporter.run();
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void Game::run()
{
	input.run();
	visual.run(&scene);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit Game::init(HINSTANCE instance)
{
	this->appInstance = instance;
	if(visual.init(this))
	{
		input.init(this);
		return true;
	}
	return false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

Game::~Game()
{
	input.stop();
	visual.stop();
}

HINSTANCE Game::getAppInstance()
{
	return appInstance;
}