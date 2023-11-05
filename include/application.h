#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <stdafx.h>
#include <singleton.h>
#include <log_manager.h>
#include <game_object_manager.h>
#include <script_manager.h>
#include <input_manager.h>

// A "convenience macro" to make accessing the Application singleton
// a bit easier.
#define APPLICATION ssuge::Application::getSingletonPtr()

/*! \mainpage S.S.U.G.E (SSU Game Engine)
 * \section intro_sec Introducation
 * SSUGE is a simple, but functional game engine.  The   Engine is written
 * in C++ and supports lua scripting (in theory, you can make an entire game in lua.
 * It also uses these libraries:
 *    - Ogre (for Rendering and rendering-related resource management and window-support)
 *    - SDL (for input-handling)
 *    - tinyxml2 (for scene parsing)
 *    - Lua (for scripting)
 *    - ...
 * \section using SSUGE
 * <later>
 */
namespace ssuge
{
	// Forward declaration to camera component
	class CameraComponent;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// The Application class is our central hub.  It initializes all of our managers,       
	/// Has access to the ogre objects, etc. and contains the main loop.  Most of the *real* 
	/// work is delegated to the various managers (ScriptManager, InputManager, etc.)         
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class Application : public OgreBites::ApplicationContext, 
		public Singleton<Application>
	{
	// ****** ATTRIBUTES ****** //
	protected:
		/// The Ogre root -- controls all other ogre objects
		Ogre::Root * mOgreRoot;

		/// The ogre window.
		Ogre::RenderWindow * mWindow;

		/// The SDL window -- this is actually just a reference to the ogre window
		SDL_Window * mSDLWindow;

		/// The scene manager -- controls most renderable ogre objects
		Ogre::SceneManager * mSceneManager;

		/// The main viewport (contains the entire window area)
		Ogre::Viewport * mMainViewport;

		/// The log manager
		LogManager * mLogManager;

		/// The game object manager
		GameObjectManager * mGameObjectManager;

		/// The script manager
		ScriptManager * mScriptManager;

		/// The input manager
		InputManager * mInputManager;

	// ****** CONSTRUCTORS / DESTUCTOR ******
	public:
		/// Default constructor -- calls the initialize method.
		Application();

		/// The destructor -- does nothing.  The shutdown method will be called by ApplicationContext.
		~Application() {  }

	// ***** OVERRIDES (from inherited classes) *****
	protected:
		/// Our callback used to set up everything (all managers, the scene, etc.)
		void setup() override;

		/// Our callback used to shut down everything (de-allocate everything)
		void shutdown() override;

		/// Our callback used to update the scene (called before rendering) -- we update all our managers and stats panel here.
		bool frameStarted(const Ogre::FrameEvent & evt) override;
	
		
	// ***** GETTERS / SETTERS ***** 
	public:
		/// Gets the ogre scene manager
		Ogre::SceneManager * getSceneManager() { return mSceneManager; }

		/// Tells ogre to start its loop
		void run() { mRoot->startRendering(); }

	// ***** OTHER METHODS ******
	protected:
		/// Sets up the scene (and possibly the viewport)
		void createScene();

	public:
		/// Change the background colour
		void setBackgroundColor(Ogre::ColourValue c);

		/// Set the active camera
		void setActiveCamera(CameraComponent * cc);
	};
}

#endif
