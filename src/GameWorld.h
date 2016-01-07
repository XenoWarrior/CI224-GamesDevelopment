#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <memory>
#include <iostream>
#include <fstream>
#include <string>

#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>

#include "common.h"
#include "GameAssetManager.h"
#include "CubeAsset.h"
#include "GameAsset.h"
#include "BlockColours.h"
#include "GameCamera.h"

/**
 * GameWorld allows us to separate the management of the game world from the
 * nuts and bolts of game loop initialisation.  The GameWorld currently has
 * a very simplified scene graph consisiting of a single GameAssetManager.
 */
class GameWorld {
	public:
		/**
		* We thread the ApplicationMode through the GameWorld ss we want to read it
		* in from the user.  Threading the state through the various function calls
		* is preferable (in this case) to having some kind of global state.
		*/
		GameWorld(ApplicationMode);

		void Draw();
		void DrawGrid();
		void DoAction(int);
		void CreateShape(std::string, int);
		void ChangeBlockDist(int);
		void LoadMap(std::string);
		void HandleInput(SDL_Window*, int, int);

		bool CheckCollision(glm::vec3);

		BlockColours colour_manager;
		std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>(glm::vec3(0,0,0));
		
	private:
		std::shared_ptr<GameAssetManager> asset_manager;


		glm::vec3 offset_pos;
		glm::vec3 position = glm::vec3(0, 0, -3);

		const unsigned char* KEYBOARD_INPUT;

};

#endif // GAMEWORLD_H
