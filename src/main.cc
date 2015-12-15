#define GLEW_STATIC
#define RUN_GRAPHICS_DISPLAY 0x00;
#define GLM_FORCE_RADIANS 

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <memory>
#include <boost/program_options.hpp>

#include "common.h"
#include "GameWorld.h"

// Lousy global variables
const Uint8* keyboard_input;
SDL_Window * _window;
SDL_DisplayMode display;

/*
 * SDL timers run in separate threads.  In the timer thread
 * push an event onto the event queue.  This event signifies
 * to call display() from the thread in which the OpenGL 
 * context was created.
 */
Uint32 tick(Uint32 interval, void *param)
{
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = RUN_GRAPHICS_DISPLAY;
	event.user.data1 = 0;
	event.user.data2 = 0;
	SDL_PushEvent(&event);
	return interval;
}

struct SDLWindowDeleter
{
	inline void operator()(SDL_Window* window)
	{
		SDL_DestroyWindow(window);
	}
};

/*
 * Handles input
 */
void HandleInput(const std::shared_ptr<GameWorld> game_world)
{
	// Camera controller
	int x, y;
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y); 
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_WarpMouseInWindow(_window, display.w/2, display.h/2); 
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
	SDL_PumpEvents(); 
	game_world->MoveCamera(glm::vec2(x,y), glm::vec2(display.w, display.h));

	// Update game_world camera
	if(keyboard_input[SDL_SCANCODE_W])
		game_world->CameraController(1); // forward
	if(keyboard_input[SDL_SCANCODE_A])
		game_world->CameraController(2); // left
	if(keyboard_input[SDL_SCANCODE_S])
		game_world->CameraController(3); // back
	if(keyboard_input[SDL_SCANCODE_D])
		game_world->CameraController(4); // right
	if(keyboard_input[SDL_SCANCODE_SPACE])
		game_world->CameraController(9); // player: +y ("fly" up)
	if(keyboard_input[SDL_SCANCODE_LCTRL])
		game_world->CameraController(10); // player: -y ("fly" down)

	if(keyboard_input[SDL_SCANCODE_E])
		game_world->DoAction(1); 
	if(keyboard_input[SDL_SCANCODE_Q])
		game_world->DoAction(2); 
	if(keyboard_input[SDL_SCANCODE_G])
		game_world->DoAction(3);

	if(keyboard_input[SDL_SCANCODE_ESCAPE])
		SDL_Quit();
}

/*
 * Draws the game world and handles buffer switching.
 */
void Draw(const std::shared_ptr<SDL_Window> window, const std::shared_ptr<GameWorld> game_world)
{
	// Background colour for the window
	glClearColor(0.0f, 0.2f, 0.2f, 0.3f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Draw the gameworld
	game_world->Draw();

	// Swap buffers
	SDL_GL_SwapWindow(window.get());
}

/*
 * Handles the initialisation of the game world.
 * Configures the SDL window and initialises GLEW
 */
std::shared_ptr<SDL_Window> InitWorld()
{
	// Window properties
	std::shared_ptr<SDL_Window> window;

	// Glew will later ensure that OpenGL 3 *is* supported
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Do double buffering in GL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Initialise SDL - when using C/C++ it's common to have to
	// initialise libraries by calling a function within them.
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) < 0)
	{
		std::cout << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
		return nullptr;
	}
	SDL_GetError();

	// When we close a window quit the SDL application
	atexit(SDL_Quit);

	SDL_ShowCursor(0);
	SDL_GetCurrentDisplayMode(0, &display);

	// Create a new window with an OpenGL surface
	_window = SDL_CreateWindow("BlockWorld"
							 , SDL_WINDOWPOS_CENTERED
							 , SDL_WINDOWPOS_CENTERED
							 , display.w
							 , display.h
							 , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	
	SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
	
	if(!_window)
	{
		std::cout << "Failed to create SDL window: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (!glContext)
	{
		std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Initialise GLEW - an easy way to ensure OpenGl 3.0+
	// The *must* be done after we have set the video mode - otherwise we have no
	// OpenGL context to test.
	glewInit();
	if(!glewIsSupported("GL_VERSION_3_0"))
	{
		std::cerr << "OpenGL 3.0 not available" << std::endl;
		return nullptr;
	}

	// OpenGL settings
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	window.reset(_window, SDL_DestroyWindow);
	return window;
}

ApplicationMode ParseOptions (int argc, char ** argv)
{
	namespace po = boost::program_options;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "print this help message")
		("translate", "Show translation example (default)")
		("rotate", "Show rotation example")
		("scale", "Show scale example");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help"))
	{
		std::cout << desc << std::endl;
		exit(0);
	}

	if(vm.count("rotate"))
	{
		return ROTATE;
	}

	if(vm.count("scale"))
	{
		return SCALE;
	}

	// The default
	return TRANSFORM;
}

int main(int argc, char ** argv) {
	Uint32 delay = 1000/60; // in milliseconds

	auto mode = ParseOptions(argc, argv);
	auto window = InitWorld();
	auto game_world = std::make_shared<GameWorld>(mode);

	if(!window)
	{
		SDL_Quit();
	}

	keyboard_input = SDL_GetKeyboardState(NULL);

	// Call the function "tick" every delay milliseconds
	SDL_AddTimer(delay, tick, NULL);

	// Add the main event loop
	SDL_Event event;
	while (SDL_WaitEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				SDL_Quit();
				break;
			case SDL_USEREVENT:
				Draw(window, game_world);
				HandleInput(game_world);
				break;
			default:
			  break;
		}
	}
}
