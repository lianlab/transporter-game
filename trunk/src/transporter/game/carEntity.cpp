#include "transporter.h"

CarEntity::CarEntity( Surface* surface )
	: GameEntity(surface->getGame()),
	  steeringInput(&surface->getGame()->inputSystem)	  
{
	this->surface = surface;
	this->debugInfo.setEntity(this);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit CarEntity::init(str name)
{
	//visualEntity initialization
	{
		Ogre::SceneNode* surfaceNode = surface->getVisualEntity()->getParentSceneNode();

		Ogre::Quaternion orient;
		Ogre::Vector3 pos;

		Ogre::SceneNode* abstractNode = surfaceNode->createChildSceneNode(name+"PreNode",2.61f*Ogre::Vector3::UNIT_Y);
		Ogre::SceneNode* carNode = abstractNode->createChildSceneNode(name+"Node");
		//Ogre::SceneNode* carNode = game->visualSystem.getSceneMgr()->getRootSceneNode()->createChildSceneNode("car");
		visualEntity = game->visualSystem.getSceneMgr()->createEntity(name,"LamborghiniBody.mesh");	
		visualEntity->setCastShadows(true);
		visualEntity->getMesh()->getSubMesh(1)->setMaterialName("bodyNotReflecting");
		visualEntity->getMesh()->getSubMesh(2)->setMaterialName("blackmatte");
		visualEntity->getMesh()->getSubMesh(3)->setMaterialName("glassrims");
		visualEntity->getMesh()->getSubMesh(4)->setMaterialName("glassrims2");
		visualEntity->getMesh()->getSubMesh(5)->setMaterialName("glass");
		visualEntity->getMesh()->getSubMesh(6)->setMaterialName("glasslights");
		visualEntity->getMesh()->getSubMesh(0)->setMaterialName("body");	
		//visualEntity->setMaterialName("body");
		carNode->attachObject(visualEntity);		

		Ogre::SceneNode* followCamNode = carNode->createChildSceneNode("followCamNode",Ogre::Vector3(30.0f,18.0f,0.0f));
		followCamNode->attachObject(game->visualSystem.getCamera());
		game->visualSystem.getCamera()->setAutoTracking(true,carNode);	
		game->visualSystem.getCamera()->setFixedYawAxis(true);

		orient.FromAngleAxis(Ogre::Degree(90.0f),Ogre::Vector3(0.0f,1.0f,0.0f));
		pos = Ogre::Vector3(7.4f,-1.83f,-4.25f);
		Ogre::SceneNode* tyreRRNode = carNode->createChildSceneNode(name+"TyreRRNode",pos,orient);
		tyre[0] = new CarTyreEntity(this->game);
		tyre[0]->init("TyreRR");	
		tyreRRNode->attachObject(tyre[0]->getVisualEntity());

		orient.FromAngleAxis(Ogre::Degree(-90.0f),Ogre::Vector3(0.0f,1.0f,0.0f));
		pos = Ogre::Vector3(7.4f,-1.83f,4.25f);
		Ogre::SceneNode* tyreRLNode = carNode->createChildSceneNode(name+"TyreRLNode",pos,orient);
		tyre[1] = new CarTyreEntity(this->game);
		tyre[1]->init("TyreRL");	
		tyreRLNode->attachObject(tyre[1]->getVisualEntity());

		orient.FromAngleAxis(Ogre::Degree(90.0f),Ogre::Vector3(0.0f,1.0f,0.0f));
		pos = Ogre::Vector3(-5.4f,-1.8f,-4.2125f);
		Ogre::SceneNode* tyreFRNode = carNode->createChildSceneNode(name+"TyreFRNode",pos,orient);
		tyreFRNode->setScale(0.97f,0.97f,0.97f);
		tyre[2] = new CarTyreEntity(this->game);
		tyre[2]->init("TyreFR");	
		tyreFRNode->attachObject(tyre[2]->getVisualEntity());

		orient.FromAngleAxis(Ogre::Degree(-90.0f),Ogre::Vector3(0.0f,1.0f,0.0f));
		pos = Ogre::Vector3(-5.4f,-1.8f,4.2125f);
		Ogre::SceneNode* tyreFLNode = carNode->createChildSceneNode(name+"TyreFLNode",pos,orient);
		tyreFLNode->setScale(0.97f,0.97f,0.97f);
		tyre[3] = new CarTyreEntity(this->game);
		tyre[3]->init("TyreFL");	
		tyreFLNode->attachObject(tyre[3]->getVisualEntity());
	}

	//physicsEntity initialization
	{
		VehicleSetup setup;
		hkpConvexVerticesShape* chassisShape = createChassisShape(); 

		int chassisLayer = 1;

		hkpRigidBodyCinfo chassisInfo;
		chassisInfo.m_mass = 750.0f;	
		chassisInfo.m_shape = chassisShape;
		chassisInfo.m_friction = 0.8f;
		chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		chassisInfo.m_position.set(0.0f, 5.0f, 0.0f);
		chassisInfo.m_inertiaTensor.setDiagonal(1.0f, 1.0f, 1.0f);
		chassisInfo.m_centerOfMass.set( -0.037f, 0.143f, 0.0f);
		chassisInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( chassisLayer, 0 );

		physicsEntity = new hkpRigidBody(chassisInfo);
		game->gameScene.physicsWorld.getWorld()->addEntity(physicsEntity, HK_ENTITY_ACTIVATION_DO_ACTIVATE );

		//hkVector4 rotationAxis(1.0f, 0.0f, 0.0f); 	 
		//hkVector4 upAxis(0.0f, 1.0f, 0.0f); 	 
		//physicsActionReorient = new hkpReorientAction(physicsEntity, rotationAxis, upAxis); 	 

		physicsVehicleInstance = new hkpVehicleInstance(physicsEntity);
		setup.buildVehicle(game->gameScene.physicsWorld.getWorld(),*physicsVehicleInstance);
		game->gameScene.physicsWorld.getWorld()->addAction(physicsVehicleInstance);

		physicsEntity->removeReference();
		chassisShape->removeReference();
	}

	return true;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

CarEntity::~CarEntity()
{
	for(u32 i=0 ; i<4 ; i++)
	{
		if(tyre[i])
		{
			delete tyre[i];
			tyre[i] = NULL;
		}
	}
	//if(physicsActionReorient)
	//{
	//	physicsActionReorient->removeReference();
	//	physicsActionReorient = NULL;
	//}
	if(physicsVehicleInstance)
	{
		physicsVehicleInstance->removeReference();
		physicsVehicleInstance = NULL;
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

hkpConvexVerticesShape* CarEntity::createChassisShape()
{
	hkReal xSize = 1.75f;
	hkReal ySize = 0.25f;
	hkReal zSize = 1.1f;

	hkReal xBumper = 1.9f;
	hkReal yBumper = 0.15f;
	hkReal zBumper = 1.0f;

	hkReal xRoofFront = 0.4f;
	hkReal xRoofBack = -1.0f;
	hkReal yRoof = ySize + 0.45f;
	hkReal zRoof = 0.7f;

	hkReal xDoorFront = xRoofFront;
	hkReal xDoorBack = xRoofBack;
	hkReal yDoor = ySize;
	hkReal zDoor = zSize + 0.1f;

	int numVertices = 22;

	// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float).
	int stride = sizeof(float) * 4;

	float vertices[] = { 
		xSize, ySize, zSize, 0.0f,		// v0
		xSize, ySize, -zSize, 0.0f,		// v1
		xSize, -ySize, zSize, 0.0f,		// v2
		xSize, -ySize, -zSize, 0.0f,	// v3
		-xSize, -ySize, zSize, 0.0f,	// v4
		-xSize, -ySize, -zSize, 0.0f,	// v5

		xBumper, yBumper, zBumper, 0.0f,	// v6
		xBumper, yBumper, -zBumper, 0.0f,	// v7
		-xBumper, yBumper, zBumper, 0.0f,	// v8
		-xBumper, yBumper, -zBumper, 0.0f,	// v9

		xRoofFront, yRoof, zRoof, 0.0f,		// v10
		xRoofFront, yRoof, -zRoof, 0.0f,	// v11
		xRoofBack, yRoof, zRoof, 0.0f,		// v12
		xRoofBack, yRoof, -zRoof, 0.0f,		// v13

		xDoorFront, yDoor, zDoor, 0.0f,		// v14
		xDoorFront, yDoor, -zDoor, 0.0f,	// v15
		xDoorFront, -yDoor, zDoor, 0.0f,	// v16
		xDoorFront, -yDoor, -zDoor, 0.0f,	// v17

		xDoorBack, yDoor, zDoor, 0.0f,		// v18
		xDoorBack, yDoor, -zDoor, 0.0f,		// v19
		xDoorBack, -yDoor, zDoor, 0.0f,		// v20
		xDoorBack, -yDoor, -zDoor, 0.0f,	// v21
	};

	// SHAPE CONSTRUCTION.
	hkpConvexVerticesShape* chassisShape;
	hkArray<hkVector4> planeEquations;
	hkGeometry geom;
	{
		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = numVertices;
			stridedVerts.m_striding = stride;
			stridedVerts.m_vertices = vertices;
		}

		hkpGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );
		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		chassisShape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
	}
	chassisShape->setRadius(0.1f);

	return chassisShape;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void CarEntity::update( u32 evId,u32 param )
{
	if(evId == EV_UPDATE_PHYSICS_FORCE)
	{
		updatePhysics(param);
	}
	else if(evId == EV_UPDATE_VISUAL_ENTITIES)
	{
		updateVisual();
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void CarEntity::updatePhysics(u32 timeElapse)
{
	steeringInput.update(timeElapse);
	physicsEntity->activate();
	hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)physicsVehicleInstance->m_deviceStatus;

	f32 Yaxis = steeringInput.getThrottle();
	if(Yaxis < steeringInput.getBraking())
	{
		Yaxis = -steeringInput.getBraking();
	}
	deviceStatus->m_positionX = -steeringInput.getSteering();
	deviceStatus->m_positionY = Yaxis;
	deviceStatus->m_handbrakeButtonPressed = steeringInput.getHandBrake();

	if(steeringInput.getGearShift() == -1 && Yaxis>0.0f)
	{
		deviceStatus->m_reverseButtonPressed = true;
	}
	else
	{
		deviceStatus->m_reverseButtonPressed = false;
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void CarEntity::updateVisual()
{
	hkVector4 vpos = physicsEntity->getPosition();
	hkVector4 vvel = physicsEntity->getLinearVelocity();
	hkQuaternion vOrient = physicsEntity->getRotation();

	Ogre::SceneNode* node = visualEntity->getParentSceneNode();
	node->setPosition(vpos(0),vpos(1),vpos(2));
	node->setOrientation(vOrient(3),vOrient(0),vOrient(1),vOrient(2));

	f32 Yaxis = steeringInput.getThrottle() - steeringInput.getBraking();
	Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	offset += 20.0f*Ogre::Vector3::UNIT_Z*steeringInput.getSteering();
	offset += 5.0f*Ogre::Vector3::UNIT_X*Yaxis;
	game->visualSystem.getCamera()->setPosition(offset);

	Ogre::SceneNode* RRNode = tyre[0]->getVisualEntity()->getParentSceneNode(); 
	Ogre::SceneNode* RLNode = tyre[1]->getVisualEntity()->getParentSceneNode();
	Ogre::SceneNode* FRNode = tyre[2]->getVisualEntity()->getParentSceneNode(); 
	Ogre::SceneNode* FLNode = tyre[3]->getVisualEntity()->getParentSceneNode(); 

	f32 susT0 = physicsVehicleInstance->m_wheelsInfo[0].m_currentSuspensionLength;
	f32 susT1 = physicsVehicleInstance->m_wheelsInfo[1].m_currentSuspensionLength;
	f32 susT2 = physicsVehicleInstance->m_wheelsInfo[2].m_currentSuspensionLength;
	f32 susT3 = physicsVehicleInstance->m_wheelsInfo[3].m_currentSuspensionLength;

	f32 susRT0 = physicsVehicleInstance->m_suspension->m_wheelParams[0].m_length;
	f32 susRT1 = physicsVehicleInstance->m_suspension->m_wheelParams[1].m_length;
	f32 susRT2 = physicsVehicleInstance->m_suspension->m_wheelParams[2].m_length;
	f32 susRT3 = physicsVehicleInstance->m_suspension->m_wheelParams[3].m_length;

	f32 deltaSusT0 = susRT0 - susT0;
	f32 deltaSusT1 = susRT1 - susT1;
	f32 deltaSusT2 = susRT2 - susT2;
	f32 deltaSusT3 = susRT3 - susT3;

	FRNode->setPosition(Ogre::Vector3(-5.4f,-1.80f,-4.2125f) + 3.05f*Ogre::Vector3::UNIT_Y*deltaSusT2);
	FLNode->setPosition(Ogre::Vector3(-5.4f,-1.80f, 4.2125f) + 3.05f*Ogre::Vector3::UNIT_Y*deltaSusT3);
	RRNode->setPosition(Ogre::Vector3( 7.4f,-1.83f,-4.25f  ) + 3.25f*Ogre::Vector3::UNIT_Y*deltaSusT0);
	RLNode->setPosition(Ogre::Vector3( 7.4f,-1.83f, 4.25f  ) + 3.25f*Ogre::Vector3::UNIT_Y*deltaSusT1);

	static f32 lastWheelRotation;
	lastWheelRotation += (hkReal)physicsEntity->getLinearVelocity().length3();

	Ogre::Degree degAngleR(Ogre::Radian(physicsVehicleInstance->m_wheelsSteeringAngle[0]));
	Ogre::Degree degAngleL(Ogre::Radian(physicsVehicleInstance->m_wheelsSteeringAngle[1]));

	Ogre::Quaternion q1;
	Ogre::Quaternion q2;
	
	q1 = Ogre::Quaternion::IDENTITY;
	q2 = Ogre::Quaternion::IDENTITY;
	q1.FromAngleAxis(Ogre::Degree(-lastWheelRotation),Ogre::Vector3::UNIT_X);	
	q2.FromAngleAxis(degAngleR - Ogre::Degree(270.0f),Ogre::Vector3::UNIT_Y);		
	FRNode->setOrientation(q2*q1);

	q1 = Ogre::Quaternion::IDENTITY;
	q2 = Ogre::Quaternion::IDENTITY;	
	q1.FromAngleAxis(Ogre::Degree(lastWheelRotation),Ogre::Vector3::UNIT_X);	
	q2.FromAngleAxis(degAngleL - Ogre::Degree(90.0f),Ogre::Vector3::UNIT_Y);	
	FLNode->setOrientation(q2*q1);	
	
	q1 = Ogre::Quaternion::IDENTITY;
	q2 = Ogre::Quaternion::IDENTITY;
	q1.FromAngleAxis(Ogre::Degree(-lastWheelRotation),Ogre::Vector3::UNIT_X);	
	q2.FromAngleAxis(Ogre::Degree(90.0f),Ogre::Vector3::UNIT_Y);		
	RRNode->setOrientation(q2*q1);

	q1 = Ogre::Quaternion::IDENTITY;
	q2 = Ogre::Quaternion::IDENTITY;
	q1.FromAngleAxis(Ogre::Degree(lastWheelRotation),Ogre::Vector3::UNIT_X);	
	q2.FromAngleAxis(Ogre::Degree(270.0f),Ogre::Vector3::UNIT_Y);	 
	RLNode->setOrientation(q2*q1);	
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

CarDebugInfo::CarDebugInfo()
:DebugRegistryBaseItem("CarProperties")
{
	entity = NULL;
	DebugRegistry::getInstance()->addItem(this);
}

CarDebugInfo::~CarDebugInfo()
{
	DebugRegistry::getInstance()->removeItem(this->debugItemName);
}

void CarDebugInfo::setEntity( CarEntity* entity )
{
	this->entity = entity;
}

str CarDebugInfo::print()
{
	if(entity)
	{
		hkVector4 vpos = entity->physicsEntity->getPosition();
		hkVector4 vvel = entity->physicsEntity->getLinearVelocity();
		hkVector4 vavel= entity->physicsEntity->getAngularVelocity();		
		hkQuaternion q = entity->physicsEntity->getRotation();
		hmath::quat orient(q(3),q(0),q(1),q(2));

		str text = StrPrintf("car pos:{x=%.2f, y=%.2f, z=%.2f} orient:{y=%.2f, p=%.2f, r=%.2f}",
			vpos(0),vpos(1),vpos(2),
			orient.getYaw().asDeg(),
			orient.getPitch().asDeg(),
			orient.getRoll().asDeg());
		
		text += StrPrintf("\ncarMotion LinearVel{x=%.2f, y=%.2f, z=%.2f} AngularVel{x=%.2f, y=%.2f, z=%.2f}",
			vvel(0),vvel(1),vvel(2),vavel(0),vavel(1),vavel(2));

		f32 speed = entity->physicsVehicleInstance->calcKMPH();
		f32 rpm   = entity->physicsVehicleInstance->calcRPM();
		u32 agear = (u32)entity->physicsVehicleInstance->m_currentGear;		
		f32 torque = entity->physicsVehicleInstance->m_torque;

		text += StrPrintf("\ncarPerformance speed:%.2fKM/H, engine RPM:%.1f, Torque:%.2f, auto gear:%d",
			              speed,rpm,torque,agear);
		
		f32 susT0 = entity->physicsVehicleInstance->m_wheelsInfo[0].m_currentSuspensionLength;
		f32 susT1 = entity->physicsVehicleInstance->m_wheelsInfo[1].m_currentSuspensionLength;
		f32 susT2 = entity->physicsVehicleInstance->m_wheelsInfo[2].m_currentSuspensionLength;
		f32 susT3 = entity->physicsVehicleInstance->m_wheelsInfo[3].m_currentSuspensionLength;

		text += StrPrintf("\ncarWheels suspension W1:%.2f, W2:%.2f, W3:%.2f, W4:%.2f",
			susT0,susT1,susT2,susT3);

		f32 dirAngle = (f32)Rad2Deg(entity->physicsVehicleInstance->m_mainSteeringAngle);
		return text;
	}
	return str("entity = NULL");
}

