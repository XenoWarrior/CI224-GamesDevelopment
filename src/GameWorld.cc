#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	// Creates a CubeAsset in the asset manager
	asset_manager = std::make_shared<GameAssetManager>(mode);

	// Starting point for cube (0,0,0)
	std::shared_ptr<CubeAsset> tmp_obj = std::make_shared<CubeAsset>(0.0, 0.0, 0.0);
	asset_manager->AddAsset(tmp_obj);
}

/**
* Handles the keyboard inputs passed from main.
* 1 = W, 2 = A, 3 = S, 4 = D (Player Movement)
* 5 = I, 6 = J, 7 = K, 8 = L (Camera Look)
* 9 = R, 10= F (Move on Y axis)
* This could likely be handled better, but it works.
*
* @param k (int) - ID of action to perform
*/
void GameWorld::CameraController(int k)
{
	// For W A S D
	if(k == 1)
		position += z_direction * camera_speed;
	if(k == 2)
		position -= x_direction * camera_speed;
	if(k == 3)
		position -= z_direction * camera_speed;
	if(k == 4)
		position += x_direction * camera_speed;


	// For I J K L
	if(k == 5)
		camera_y += 0.5f * camera_speed;
	if(k == 6)
		camera_x += 0.5f * camera_speed;
	if(k == 7)
		camera_y -= 0.5f * camera_speed;
	if(k == 8)
		camera_x -= 0.5f * camera_speed;

	// For R F
	if(k == 9)
		position.y += 0.5f * camera_speed;
	if(k == 10)
		position.y -= 0.5f * camera_speed;
}

// test
void GameWorld::MoveCamera(int x, int y)
{
	camera_y -= y * camera_speed / 10.0f;
	camera_x += x * camera_speed / 10.0f;
}

/**
 * Handles action codes passed from main
 * @param a (int) ID of action to perform
 */
void GameWorld::DoAction(int a)
{
	if(a == 1)
	{
		std::shared_ptr<CubeAsset> new_cube = std::make_shared<CubeAsset>(0.0f + int(position.x), 0.0f + int(position.y), 0.0f + int(position.z)); // Cube to make
		asset_manager->AddAsset(new_cube);
	}
	if(a == 2)
	{
		asset_manager->RemoveAsset(position);
	}
	if(a == 3)
	{
		asset_manager->RemoveAll();
		CreateFloor(rand() % 20 + 1, rand() % 20 + 1);
	}
}

/**
 * Handles drawing of the camera and game world assets
 */
void GameWorld::Draw()
{
	glm::vec3 direction(
		cos(camera_y) * sin(camera_x),
		sin(camera_y),
		cos(camera_y) * cos(camera_x)
	);

	glm::vec3 move_direction(
		cos(camera_y) * sin(camera_x),
		0,
		cos(camera_y) * cos(camera_x)
	);
	z_direction = move_direction;

	x_direction = glm::vec3(
		sin(camera_x - 3.14f/2.0f),
		0,
		cos(camera_x - 3.14f/2.0f)
	);

	glm::vec3 vup = glm::cross(x_direction, direction);
	glm::mat4 cam_proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 cam_view = glm::lookAt(
		position,
		position + direction,
		vup
	);
	glm::mat4 cam_mod(1.0f);

	glUniformMatrix4fv(0, 1, GL_FALSE, &cam_proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &cam_view[0][0]);
	glUniformMatrix4fv(2, 1, GL_FALSE, &cam_mod[0][0]);

	// Calls draw from the asset manager
	asset_manager->Draw();
}

/**
 * Generates a floor from 0,0 to X and Y specified
 */
void GameWorld::CreateFloor(int x, int y)
{
	for(int i = 0; i < x; i++)
	{
		for(int o = 0; o < y; o++)
		{
			asset_manager->AddAsset(std::make_shared<CubeAsset>(0.0+i, 0.0, 0.0+o));
		}
	}
}
