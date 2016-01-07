#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	asset_manager = std::make_shared<GameAssetManager>(mode);
	asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(1000.0, 0.0, 0.0), colour_manager.COLOUR_NONE));

	CreateShape("ground", 20);
}

/**
 * Handles drawing of the camera and game world assets
 */
void GameWorld::Draw()
{
	// Update the facing direction
	camera->UpdateFacingDirection();

	// Calls draw from the asset manager
	asset_manager->Draw();

	// Draws our grid
	DrawGrid();
}

void GameWorld::HandleInput(SDL_Window * _WINDOW, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	// Camera controller
	int x, y;
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y); 
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_WarpMouseInWindow(_WINDOW, WINDOW_WIDTH/2, WINDOW_HEIGHT/2); 
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
	SDL_PumpEvents(); 

	KEYBOARD_INPUT = SDL_GetKeyboardState(NULL);

	// Update game_world camera
	if(KEYBOARD_INPUT[SDL_SCANCODE_W])
		camera->CameraController(1); // forward
	if(KEYBOARD_INPUT[SDL_SCANCODE_A])
		camera->CameraController(2); // left
	if(KEYBOARD_INPUT[SDL_SCANCODE_S])
		camera->CameraController(3); // back
	if(KEYBOARD_INPUT[SDL_SCANCODE_D])
		camera->CameraController(4); // right
	if(KEYBOARD_INPUT[SDL_SCANCODE_SPACE])
		camera->CameraController(9); // player: +y ("fly" up)
	if(KEYBOARD_INPUT[SDL_SCANCODE_LCTRL])
		camera->CameraController(10); // player: -y ("fly" down)

	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		DoAction(1); 
	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		DoAction(2); 
	if(KEYBOARD_INPUT[SDL_SCANCODE_G])
		DoAction(3);
	if(KEYBOARD_INPUT[SDL_SCANCODE_H])
		DoAction(4);
	
	if(KEYBOARD_INPUT[SDL_SCANCODE_J])
		LoadMap("heightmap.ppm");

	if(KEYBOARD_INPUT[SDL_SCANCODE_E])
		camera->ChangeBlockDist(1);
	if(KEYBOARD_INPUT[SDL_SCANCODE_Q])
		camera->ChangeBlockDist(-1);

	if(KEYBOARD_INPUT[SDL_SCANCODE_ESCAPE])
		SDL_Quit();
}

/**
 * Handles action codes passed from main
 */
void GameWorld::DoAction(int a)
{
	if(a == 1)
	{
		offset_pos = camera->GetOffset();
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0f + int(round(position.x)) + offset_pos.x, 0.0f + int(round(position.y)) + offset_pos.y, 0.0f + int(round(position.z)) + offset_pos.z), colour_manager.COLOUR_RANDOM));
	}
	if(a == 2)
	{
		offset_pos = camera->GetOffset();
		asset_manager->RemoveAsset(position, offset_pos);
	}
	if(a == 3)
	{
		asset_manager->RemoveAll();
		CreateShape("sphere", rand() % 19 + 1);
	}
	if(a == 4)
	{
		asset_manager->RemoveAll();
		CreateShape("cube", rand() % 19 + 1);
	}
}

/**
 * Draws a grid in 3D space
 */
void GameWorld::DrawGrid()
{
	glm::vec3 offset_pos = camera->GetOffset();

	glBegin(GL_QUADS);
	// Bottom
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);
	
	// Top
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);

	// Front
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.52);

	// Back
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.52);

	// Left
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);
	
	// Right
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);

	glEnd();

	for(float i = -20; i <= 20; i += 0.5)
	{
		glBegin(GL_LINES);
		glVertex3f(-20, -0.5, i);
		glVertex3f(20, -0.5, i);
		glVertex3f(i, -0.5, -20);
		glVertex3f(i, -0.5, 20);
		glEnd();
	}
}

/**
 * Generates a shape
 */
void GameWorld::CreateShape(std::string shape, int size)
{
	if(shape == "sphere")
	{
		for (int z = 0; z < size; z++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					if (sqrt((float) (x-size/2)*(x-size/2) + (y-size/2)*(y-size/2) + (z-size/2)*(z-size/2)) <= size/2)
					{
						asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0+x, 0.0+y, 0.0+z), colour_manager.COLOUR_RANDOM));
					}
				}
			}
		}
		std::cout << "Created Shape 1!" << std::endl;
		return;
	}
	if(shape == "cube")
	{
		for (int z = 0; z < size; z++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0+x, 0.0+y, 0.0+z), colour_manager.COLOUR_RANDOM));
				}
			}
		}
		std::cout << "Created Shape 2!" << std::endl;
		return;
	}
	if(shape == "ground")
	{
		for(int z = 0; z < size; z++)
		{
			for(int x = 0; x < size; x++)
			{
				asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(-10.0+x, -1.0f, -10.0+z), colour_manager.COLOUR_GREEN));
			}
		}
		std::cout << "Created Shape 3!" << std::endl;
		return;
	}
	std::cout << "Shape not found!" << std::endl;
}


/**
 * Returns true if the position passed is already filled with a block
 */
bool GameWorld::CheckCollision(glm::vec3 point)
{
	if(point == glm::vec3(int(position.x), int(position.y), int(position.z)))
	{
		return true;
	}
	
	return false;
}

/**
 * A very bad example of rendering heightmaps
 * Takes in a PPM file, reads the headers and then the map data
 */
void GameWorld::LoadMap(std::string filename)
{
	std::cout << "Reading file: " << filename << "..." << std::endl;
	std::ifstream infile(filename);
	
	if(infile.is_open())
	{
		// Stores the cube Y cordinates from the PPM file
		std::vector<int> cube_y;

		// For the loop, increment and Y co-ordinate
		int push_i = 0, push_s = 0, i = 0;

		// For the headers
		std::string MAP_TYPE; 
		int MAP_WIDTH, MAP_HEIGHT, MAP_COLOURS, TOTAL_BLOCKS;
		double PERCENTAGE;

		// Read in headers
		infile >> MAP_TYPE >> MAP_WIDTH >> MAP_HEIGHT >> MAP_COLOURS;
		TOTAL_BLOCKS = MAP_WIDTH * MAP_HEIGHT;

		std::cout 	<< "File Data:" << std::endl
					<< "    File Type: " << MAP_TYPE << std::endl
					<< "    Map Width: " << MAP_WIDTH << std::endl
					<< "    Map Height: " << MAP_HEIGHT << std::endl
					<< "    Map Colours: " << MAP_COLOURS << std::endl;

		// Read in map data
		while (infile >> push_s)
		{
			if(push_i == 1)
				cube_y.push_back(push_s);

			if(push_i == 2)
				push_i = 0;
			else
				push_i++;
		}
	
		for(int z = 0; z < MAP_HEIGHT; z++)
		{
			for(int x = 0; x < MAP_WIDTH; x++)
			{
				asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(x, cube_y[i], z), colour_manager.COLOUR_GREEN));
				i++;

				std::cout << "[" << i << " / " << TOTAL_BLOCKS << "]\r";
				std::cout.flush();
			}
		} 	
	}
	return;
}