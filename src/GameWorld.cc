#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	// Creates a CubeAsset in the asset manager
	asset_manager = std::make_shared<GameAssetManager>(mode);

	// Make a few cubes
	for(int i = 0; i < 5; i++)
	{
		if(i == 0)
		{
			asset_manager->AddAsset(std::make_shared<CubeAsset>(0.0, 0.0, 0.0));
		}
		else
		{
			asset_manager->AddAsset(std::make_shared<CubeAsset>(2.0*i, 0.0, 0.0));
		}
	}
}

/**
* Handles the keyboard inputs passed from main.
* 1 = W, 2 = A, 3 = S, 4 = D (Player Movement)
* 5 = I, 6 = J, 7 = K, 8 = L (Camera Look)
*/
void GameWorld::CameraController(int k)
{
	if(k == 1)
		position += z_direction * camera_speed;
	if(k == 2)
		position -= x_direction * camera_speed;
	if(k == 3)
		position -= z_direction * camera_speed;
	if(k == 4)
		position += x_direction * camera_speed;
	if(k == 5)
		camera_y += 0.5f / 10.0f;
	if(k == 6)
		camera_x += 0.5f / 10.0f;
	if(k == 7)
		camera_y -= 0.5f / 10.0f;
	if(k == 8)
		camera_x -= 0.5f / 10.0f;
}

void GameWorld::DoAction(int a)
{
	if(a == 1)
	{
		asset_manager->AddAsset(std::make_shared<CubeAsset>(position.x, position.y, position.z));
		std::cout << "Created cube at position: (X: " << position.x << ", Y: " << position.y << ", Z: " << position.z << ")" << std::endl;
	}
}

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
