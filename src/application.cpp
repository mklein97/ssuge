#include <stdafx.h>
#include <application.h>
#include <camera_component.h>
#include <game_object.h>

// Set this to 1 to use opengl 3+, else set it to 1 to use DX11
#define SSUGE_USE_GL 1

// The icky template-specialization / static-initializer for Application's msSingleton attribute
template<> ssuge::Application* ssuge::Singleton<ssuge::Application>::msSingleton = NULL;


ssuge::Application::Application() : OgreBites::ApplicationContext("ssuge (Matt Klein)"), mOgreRoot(NULL),
	mWindow(NULL), mSDLWindow(NULL), mSceneManager(NULL), mMainViewport(NULL), mLogManager(NULL), 
	mGameObjectManager(NULL), mScriptManager(NULL), mInputManager(NULL)
{
	// Intentionally empty -- the initializers above did the real work.
}


void ssuge::Application::setup()
{
	// Create our log manager
	mLogManager = new LogManager("ssuge_log.txt");

	// do not forget to call the base first.  This creates
	// the root and the ogre window
	LOG_MANAGER->log("Calling OgreBites::ApplicationContext::setup");
	OgreBites::ApplicationContext::setup();

	// Get pointers to the objects created by ApplicationContext::setup.
	mOgreRoot = getRoot();
	mWindow = getRenderWindow();
	if (mWindow->getNumViewports() > 0)
		mMainViewport = mWindow->getViewport(0);
	else
		mMainViewport = mWindow->addViewport(NULL);

	// Create the scene manager
	LOG_MANAGER->log("Creating the scene manager");
	std::string scene_manager_type = "OctreeSceneManager";
	mSceneManager = mOgreRoot->createSceneManager(scene_manager_type);

	// Set default shadow type -- doesn't seem to be doing anything...
	mSceneManager->setAmbientLight(Ogre::ColourValue(0, 0, 0));		
	mSceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);		
	mSceneManager->setShadowColour(Ogre::ColourValue(0, 0, 0));
	mSceneManager->setShadowFarDistance(5000.0f);

	// register our scene manager with the RTSS
	LOG_MANAGER->log("Registering scene manager with the RTShader System");
	Ogre::RTShader::ShaderGenerator::initialize();
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mSceneManager);

	// Create the other ssuge managers
	mGameObjectManager = new GameObjectManager();
	mScriptManager = new ScriptManager();
	mInputManager = new InputManager();

	// Call createScene
	LOG_MANAGER->log("Calling createScene");
	createScene();
}

void ssuge::Application::createScene()
{
	// Load a scene.  Future work: it would be nice if we could tap into
	// Ogre's auto-discover routines and just be able to load "silly_scene_hierarchy.scene" rather
	// than having to give the full path (this is totally possible:-)!)
	SCRIPT_MANAGER->loadScript("../media/init.lua");
	mSceneManager->setSkyBox(true, "MySky");
	mMainViewport->getCamera()->setAutoAspectRatio(true);
}

void ssuge::Application::setBackgroundColor(Ogre::ColourValue c)
{
	mMainViewport->setBackgroundColour(c);
}


void ssuge::Application::setActiveCamera(ssuge::CameraComponent * cc)
{
	mMainViewport->setCamera(cc->mCamera);
}


void ssuge::Application::shutdown() 
{
	// Destroy our ssuge managers
	if (mInputManager)
		delete mInputManager;
	if (mScriptManager)
		delete mScriptManager;
	if (mGameObjectManager)
		delete mGameObjectManager;

	// Call the base-class shutdown method -- this will shut down
	// all ogre objects
	LOG_MANAGER->log("Calling OgreBites::ApplicationContext::shutdown");
	OgreBites::ApplicationContext::shutdown();

	// SDL quit
	SDL_DestroyWindow(mSDLWindow);
	SDL_Quit();

	// Destroy our log manager
	LOG_MANAGER->log("Destroying log manager:-(");
	if (mLogManager)
		delete mLogManager;
}


bool ssuge::Application::frameStarted(const Ogre::FrameEvent & evt)
{
	bool stay_open = true;
	
	// Update our managers
	float dt = evt.timeSinceLastFrame;
	mScriptManager->update(dt);
	mGameObjectManager->update(dt);
	
	stay_open = mInputManager->update(evt.timeSinceLastFrame);

	return stay_open;
}