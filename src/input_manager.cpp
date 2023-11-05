#include <stdafx.h>
#include <input_manager.h>
#include <application.h>

#define MAX_CONTROLLERS 4
SDL_GameController *ControllerHandles[MAX_CONTROLLERS];
// singleton instance initialization / declaration
template<> ssuge::InputManager* ssuge::Singleton<ssuge::InputManager>::msSingleton = NULL;

ssuge::InputManager::InputManager()
{
	bLeft = false;
	bMiddle = false;
	bRight = false;
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	int MaxJoysticks = SDL_NumJoysticks();
	int ControllerIndex = 0;
	for (int JoystickIndex = 0; JoystickIndex < MaxJoysticks; ++JoystickIndex)
	{
		if (!SDL_IsGameController(JoystickIndex))
			continue;
		if (ControllerIndex >= MAX_CONTROLLERS)
			break;
		ControllerHandles[ControllerIndex] = SDL_GameControllerOpen(JoystickIndex);
		ControllerIndex++;
	}
	Controllers.push_back(Con1);
	Controllers.push_back(Con2);
	Controllers.push_back(Con3);
	Controllers.push_back(Con4);

	for (auto a : Controllers)
	{
		a.insert(std::pair<std::string, int>("Up", 0));
		a.insert(std::pair<std::string, int>("Down", 0));
		a.insert(std::pair<std::string, int>("Left", 0));
		a.insert(std::pair<std::string, int>("Right", 0));
		a.insert(std::pair<std::string, int>("Start", 0));
		a.insert(std::pair<std::string, int>("Back", 0));
		a.insert(std::pair<std::string, int>("LShoulder", 0));
		a.insert(std::pair<std::string, int>("RShoulder", 0));
		a.insert(std::pair<std::string, int>("A", 0));
		a.insert(std::pair<std::string, int>("B", 0));
		a.insert(std::pair<std::string, int>("X", 0));
		a.insert(std::pair<std::string, int>("Y", 0));
		a.insert(std::pair<std::string, int>("LStickButton", 0));
		a.insert(std::pair<std::string, int>("RStickButton", 0));
		a.insert(std::pair<std::string, int>("LStickX", 0));
		a.insert(std::pair<std::string, int>("LStickY", 0));
		a.insert(std::pair<std::string, int>("RStickX", 0));
		a.insert(std::pair<std::string, int>("RStickY", 0));
		a.insert(std::pair<std::string, int>("LTrigger", 0));
		a.insert(std::pair<std::string, int>("RTrigger", 0));
	}
}



ssuge::InputManager::~InputManager()
{
	for (int ControllerIndex = 0; ControllerIndex < MAX_CONTROLLERS; ++ControllerIndex)
	{
		if (ControllerHandles[ControllerIndex])
			SDL_GameControllerClose(ControllerHandles[ControllerIndex]);
	}
}



bool ssuge::InputManager::update(float dt)
{
	SDL_Event sdl_evt;
	while (SDL_PollEvent(&sdl_evt))
	{
		//Keyboard and Mouse input
		if (sdl_evt.type == SDL_QUIT)
			return false;
		if (sdl_evt.type == SDL_KEYDOWN)
		{
			// We could optionally remove this and require the user include
			// this logic in one of their scripts...
			if (sdl_evt.key.keysym.sym == SDLK_ESCAPE)
				return false;
			broadcastKeyEvent(sdl_evt.key.keysym.sym, true);
		}
		else if (sdl_evt.type == SDL_KEYUP)
			broadcastKeyEvent(sdl_evt.key.keysym.sym, false);
		else if (sdl_evt.type == SDL_MOUSEBUTTONDOWN)
		{
			if (sdl_evt.button.button == SDL_BUTTON_LEFT)
				bLeft = true;
			if (sdl_evt.button.button == SDL_BUTTON_MIDDLE)
				bMiddle = true;
			if (sdl_evt.button.button == SDL_BUTTON_RIGHT)
				bRight = true;
		}
		else if (sdl_evt.type == SDL_MOUSEBUTTONUP)
		{
			if (sdl_evt.button.button == SDL_BUTTON_LEFT)
				bLeft = false;
			if (sdl_evt.button.button == SDL_BUTTON_MIDDLE)
				bMiddle = false;
			if (sdl_evt.button.button == SDL_BUTTON_RIGHT)
				bRight = false;
		}
	}
		
	for (int ControllerIndex = 0; ControllerIndex < MAX_CONTROLLERS; ++ControllerIndex)
	{
		if (ControllerHandles[ControllerIndex] != 0 && SDL_GameControllerGetAttached(ControllerHandles[ControllerIndex]))
		{
			int Up = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_UP);
			int Down = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			int Left = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			int Right = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
			int Start = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_START);
			int Back = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_BACK);
			int LeftShoulder = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			int RightShoulder = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			int AButton = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_A);
			int BButton = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_B);
			int XButton = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_X);
			int YButton = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_Y);
			int LStickButton = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_LEFTSTICK);
			int RStickButton = SDL_GameControllerGetButton(ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_RIGHTSTICK);
			int LStickX = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTX);
			int LStickY = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTY);
			int RStickX = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_RIGHTX);
			int RStickY = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_RIGHTY);
			int LTrigger = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			int RTrigger = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

			updateController(ControllerIndex, "Up", Up);
			updateController(ControllerIndex, "Down", Down);
			updateController(ControllerIndex, "Left", Left);
			updateController(ControllerIndex, "Right", Right);
			updateController(ControllerIndex, "Start", Start);
			updateController(ControllerIndex, "Back", Back);
			updateController(ControllerIndex, "LShoulder", LeftShoulder);
			updateController(ControllerIndex, "RShoulder", RightShoulder);
			updateController(ControllerIndex, "A", AButton);
			updateController(ControllerIndex, "B", BButton);
			updateController(ControllerIndex, "X", XButton);
			updateController(ControllerIndex, "Y", YButton);
			updateController(ControllerIndex, "LStickButton", LStickButton);
			updateController(ControllerIndex, "RStickButton", RStickButton);
			updateController(ControllerIndex, "LStickX", LStickX);
			updateController(ControllerIndex, "LStickY", LStickY);
			updateController(ControllerIndex, "RStickX", RStickX);
			updateController(ControllerIndex, "RStickY", RStickY);
			updateController(ControllerIndex, "LTrigger", LTrigger);
			updateController(ControllerIndex, "RTrigger", RTrigger);
		}
	}
	return true;
}

void ssuge::InputManager::updateController(int conNum, std::string button, int isPressed)
{
	auto it = Controllers[conNum].find(button);
	if (it != Controllers[conNum].end())
		it->second = isPressed;
}


bool ssuge::InputManager::isKeyDown(SDL_Keycode k)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	return state[SDL_GetScancodeFromKey(k)];
}


bool ssuge::InputManager::isKeyDown(std::string keyname)
{
	SDL_Keycode k = SDL_GetKeyFromName(keyname.c_str());
	if (k != SDLK_UNKNOWN)
		return isKeyDown(k);
	return false;
}

void ssuge::InputManager::registerListener(ssuge::InputObserver * o)
{
	mListeners.insert(o);
}


void ssuge::InputManager::deregisterListener(ssuge::InputObserver * o)
{
	std::set<InputObserver*>::iterator it = mListeners.find(o);
	if (it != mListeners.end())
		mListeners.erase(it);
}


std::string ssuge::InputManager::getKeyName(SDL_Keycode k)
{
	return std::string(SDL_GetKeyName(k));
}

Ogre::Vector2 ssuge::InputManager::getMousePos()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return Ogre::Vector2(x, y);
}

bool ssuge::InputManager::isMouseButtonDown(std::string button)
{
	if (button == "Left" || button == "left")
	{
		if (bLeft)
			return true;
	}
	if (button == "Right" || button == "right")
	{
		if (bRight)
			return true;
	}
	if (button == "Middle" || button == "middle")
	{
		if (bMiddle)
			return true;
	}
	return false;
}

int ssuge::InputManager::getControllerButtonInput(int conNum, std::string button)
{
	if ((button == "Up" || button == "up") && Controllers[conNum]["Up"])
		return 1;
	if ((button == "Down" || button == "down") && Controllers[conNum]["Down"])
		return 1;
	if ((button == "Left" || button == "left") && Controllers[conNum]["Left"])
		return 1;
	if ((button == "Right" || button == "right") && Controllers[conNum]["Right"])
		return 1;
	if ((button == "Start" || button == "start") && Controllers[conNum]["Start"])
		return 1;
	if ((button == "Back" || button == "back") && Controllers[conNum]["Back"])
		return 1;
	if ((button == "LShoulder" || button == "lshoulder" || button == "lShoulder" || button == "Lshoulder") && Controllers[conNum]["LShoulder"])
		return 1;
	if ((button == "RShoulder" || button == "rshoulder" || button == "rShoulder" || button == "Rshoulder") && Controllers[conNum]["RShoulder"])
		return 1;
	if ((button == "A" || button == "a") && Controllers[conNum]["A"])
		return 1;
	if ((button == "B" || button == "b") && Controllers[conNum]["B"])
		return 1;
	if ((button == "X" || button == "x") && Controllers[conNum]["X"])
		return 1;
	if ((button == "Y" || button == "y") && Controllers[conNum]["Y"])
		return 1;
	if ((button == "LStickButton") && Controllers[conNum]["LStickButton"])
		return 1;
	if ((button == "RStickButton") && Controllers[conNum]["RStickButton"])
		return 1;
	return 0;
}

long int ssuge::InputManager::getControllerAxisOrTriggerValue(int conNum, std::string name)
{
	if (name == "LStickX")
		return Controllers[conNum][name];
	if (name == "LStickY")
		return Controllers[conNum][name];
	if (name == "RStickX")
		return Controllers[conNum][name];
	if (name == "RStickY")
		return Controllers[conNum][name];
	if (name == "LTrigger")
		return Controllers[conNum][name];
	if (name == "RTrigger")
		return Controllers[conNum][name];
	return 0;
}


void ssuge::InputManager::broadcastKeyEvent(SDL_Keycode k, bool is_down)
{
	std::set<InputObserver*>::iterator it = mListeners.begin();
	while (it != mListeners.end())
	{
		if (is_down)
			(*it)->onKeyDown(k);
		else
			(*it)->onKeyUp(k);
		++it;
	}
}
