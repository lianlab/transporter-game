#include "transporter.h"

GameEntity::GameEntity( Game* game )
{
	this->visualEntity = NULL;
	this->physicsEntity = NULL;
	this->game = game;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

Ogre::Entity* GameEntity::getVisualEntity()
{
	return this->visualEntity; 
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit GameEntity::init(str name)
{
	return true;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

GameEntity::~GameEntity()
{
	if(visualEntity)
	{		
		if(visualEntity->getParentSceneNode())
		{
			visualEntity->getParentSceneNode()->detachObject(visualEntity);
		}
		delete visualEntity;
		visualEntity = NULL;
	}
	//TODO: delete physics entity
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

hkpRigidBody* GameEntity::getPhysicsEntity()
{
	return this->physicsEntity;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

Game* GameEntity::getGame()
{
	return game;
}