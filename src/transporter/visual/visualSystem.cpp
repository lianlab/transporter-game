#include "transporter.h"

VisualSystem::VisualSystem()
{
	visualRoot		= NULL;
	renderWindow    = NULL;
	renderSystem    = NULL;
	renderCamera    = NULL;
	renderViewport  = NULL;
	sceneMgr		= NULL;
	renderWindowHandle = NULL;
	game = NULL;
	isVisualInited  = false;
	isVisualRunning = false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit VisualSystem::init(Game* game)
{
	if(isVisualInited)
	{
		return false;
	}
	this->game = game;
	visualRoot = new Ogre::Root("visual-plugins.cfg",
		                        "visual-config.cfg",
								"visual-log.txt");

	glPlugin = new Ogre::GLPlugin;
	visualRoot->installPlugin(glPlugin);

	octreePlugin = new Ogre::OctreePlugin;
	visualRoot->installPlugin(octreePlugin);

	cgPlugin = new Ogre::CgPlugin;
	visualRoot->installPlugin(cgPlugin);

	renderSystem = visualRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	visualRoot->setRenderSystem(renderSystem);

	renderSystem->setConfigOption("Full Screen", "No");
	renderSystem->setConfigOption("Video Mode", "800 x 600 @ 32-bit");
	renderSystem->setConfigOption("VSync", "No");
	renderSystem->setConfigOption("RTT Preferred Mode","FBO");	
	renderSystem->setConfigOption("FSAA","4");

	Ogre::ConfigFile cf;
	cf.load("visual-resources.cfg");
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	str secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

	renderWindow   = visualRoot->initialise(true);
	//Ogre::NameValuePairList misc;
	//misc["externalWindowHandle"] = Ogre::StringConverter::toString((int)window->getHandle());
	//renderWindow = visualRoot->createRenderWindow("Main RenderWindow", 800, 600, false, &misc);
	renderWindow->getCustomAttribute("WINDOW", &renderWindowHandle);

	sceneMgr = visualRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);

	renderCamera = sceneMgr->createCamera("mainCamera");
	renderCamera->setNearClipDistance( 1 );
	renderCamera->setFarClipDistance(99999);

	renderViewport = renderWindow->addViewport(renderCamera);
	renderViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	renderCamera->setAspectRatio(f32(renderViewport->getActualWidth()) / 
		                         f32(renderViewport->getActualHeight()));

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	if( renderWindow &&
		renderSystem &&
		renderCamera &&
		renderViewport &&
		sceneMgr &&
		visualRoot )
	{
		isVisualInited = true;
		return true;
	}
	return false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

VisualSystem::~VisualSystem()
{
	cleanUp();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void VisualSystem::run(VisualScene* scene)
{
	if(!isVisualInited)
	{
		return;
	}
	
	this->scene = scene;
	scene->init(game);

	isVisualRunning    = true;
	u64 usedtick       = 0;
	u32 currentFrame   = 0;
	u32 framePerSecond = 60;

	LARGE_INTEGER starttick;
	LARGE_INTEGER tick;
	LARGE_INTEGER tickPerSecond;

	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	while( !renderWindow->isClosed() && isVisualRunning)
	{
		QueryPerformanceCounter(&starttick);

		Ogre::WindowEventUtilities::messagePump();
		scene->update();
		visualRoot->renderOneFrame();

		currentFrame++;
		u64 lasttick = tick.QuadPart;
		u32 frameleft = framePerSecond - currentFrame;
		QueryPerformanceCounter(&tick);

		if( ((usedtick+(tick.QuadPart - lasttick)) < (u64)tickPerSecond.QuadPart) && frameleft)
		{			
			u64 deltatick = tick.QuadPart - starttick.QuadPart;
			usedtick     += tick.QuadPart - lasttick;
			u64 tickleft  = tickPerSecond.QuadPart - usedtick;
			u64 sleeptick = tickleft/frameleft;
			u32 sleepMS   = (u32)(1000*sleeptick/tickPerSecond.QuadPart);	

			Sleep(sleepMS);
		}
		else
		{
			usedtick = 0;
			currentFrame = 0;
			frameleft = framePerSecond;
		}
	}

	cleanUp();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void VisualSystem::stop()
{
	isVisualRunning = false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit VisualSystem::isRunnning()
{
	return isVisualRunning;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void VisualSystem::cleanUp()
{
	stop();

	if(visualRoot)
	{
		delete visualRoot;
		visualRoot = NULL;
		renderWindow = NULL;
		renderCamera = NULL;
		renderViewport = NULL;
		renderSystem = NULL;
	}

	//delete sceneMgr;
	//sceneMgr = NULL;

	if(glPlugin)
	{
		delete glPlugin;
		glPlugin = NULL;
	}

	isVisualInited = false;
}

HWND VisualSystem::getWindowHandle()
{
	return renderWindowHandle;
}

Ogre::RenderWindow* VisualSystem::getRenderWindow()
{
	return renderWindow;
}

Ogre::RenderSystem* VisualSystem::getRenderSystem()
{
	return renderSystem;
}

Ogre::Camera* VisualSystem::getCamera()
{
	return renderCamera;
}

Ogre::SceneManager* VisualSystem::getSceneMgr()
{
	return sceneMgr;
}