#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	// Creates a CubeAsset in the asset manager
	asset_manager = std::make_shared<GameAssetManager>(mode);

	// Starting point for cube (0,0,0)
	std::shared_ptr<CubeAsset> tmp_obj = std::make_shared<CubeAsset>(1000.0, 0.0, 0.0);
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
	// Slow collision checker
	std::vector<std::shared_ptr<CubeAsset>> asset_list = asset_manager->GetAssets();
	for(int i = 0; i < asset_list.size(); i++)
	{
		CubeAsset c = *asset_list[i];
		if(CheckCollision(c.GetVec3()))
		{
			std::cout << "[P]Detected collision with cube at: (X: " << position.x << ", Y: " << position.y << ", Z: " << position.z << ")" << std::endl;
			std::cout << "[C]Detected collision with cube at: (X: " << c.GetVec3().x << ", Y: " << c.GetVec3().y << ", Z: " << c.GetVec3().z << ")" << std::endl;
		}
	}

	// For W A S D
	if(k == 1)
		position += z_direction * camera_speed;
	if(k == 2)
		position -= x_direction * camera_speed;
	if(k == 3)
		position -= z_direction * camera_speed;
	if(k == 4)
		position += x_direction * camera_speed;

	// I K 
	if(k == 5)
		camera_y += 0.5f * camera_speed;
	if(k == 7)
		camera_y -= 0.5f * camera_speed;

	// J L
	if(k == 6)
		camera_x += 0.5f * camera_speed;
	if(k == 8)
		camera_x -= 0.5f * camera_speed;

	// For R F
	if(k == 9)
		position.y += 0.5f * camera_speed;
	if(k == 10)
		position.y -= 0.5f * camera_speed;

	// Resetting camera position X
	if(camera_x <= 0)
		camera_x = 6.279;
	if(camera_x >= 6.28)
		camera_x = 0;

	// Resetting camera position Y
	if(camera_y >= 1.5f)
		camera_y = 1.5;
	if(camera_y <= -1.5f)
		camera_y = -1.5;
	
	if(camera_x <= point && camera_x >= 0.0 && f_pos != "N")
	{
		f_pos = "N";
		std::cout << "[N]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point && camera_x <= point*3 && f_pos != "NW")
	{
		f_pos = "NW";
		std::cout << "[NW]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*3 && camera_x <= point*5 && f_pos != "W")
	{
		f_pos = "W";
		std::cout << "[W]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*5 && camera_x <= point*7 && f_pos != "SW")
	{
		f_pos = "SW";
		std::cout << "[SW]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*7 && camera_x <= point*9 && f_pos != "S")
	{
		f_pos = "S";
		std::cout << "[S]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*9 && camera_x <= point*11 && f_pos != "SE")
	{
		f_pos = "SE";
		std::cout << "[SE]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*11 && camera_x <= point*13 && f_pos != "E")
	{
		f_pos = "E";
		std::cout << "[E]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*13 && camera_x <= point*15 && f_pos != "NE")
	{
		f_pos = "NE";
		std::cout << "[NE]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
	if(camera_x >= point*15 && camera_x <= point*16 && f_pos != "NE")
	{
		f_pos = "NE";
		std::cout << "[NE]" << "X " << camera_x << ", Y " << camera_y << std::endl;
	}
}

// test
void GameWorld::MoveCamera(int x, int y)
{
	camera_y -= y * camera_speed / 10.0f;
	camera_x += x * camera_speed / 10.0f;
}

/**
 * Draws a grid in 3D space
 */
void GameWorld::DrawGrid()
{
	for(float i = -500; i <= 500; i += 0.5)
	{
		glBegin(GL_LINES);
		glVertex3f(-500, -0.5, i);
		glVertex3f(500, -0.5, i);
		glVertex3f(i, -0.5, -500);
		glVertex3f(i, -0.5, 500);
		glEnd();
	}
}

/**
 * Handles action codes passed from main
 * @param a (int) ID of action to perform
 */
void GameWorld::DoAction(int a)
{
	if(a == 1)
	{
		int x = 0, y = 0, z = 0;
		if(f_pos == "N")
		{
			z += 2;
		}
		if(f_pos == "NE")
		{
			z += 1;
			x -= 1;
		}
		if(f_pos == "E")
		{
			x -= 2;
		}
		if(f_pos == "SE")
		{
			z -= 1;
			x -= 1;
		}
		if(f_pos == "S")
		{
			z -= 2;
		}
		if(f_pos == "SW")
		{
			z -= 1;
			x += 1;
		}
		if(f_pos == "W")
		{
			x += 2;
		}
		if(f_pos == "NW")
		{
			z += 1;
			x += 1;
		}

		std::shared_ptr<CubeAsset> new_cube = std::make_shared<CubeAsset>(0.0f + int(round(position.x)) + x, 0.0f + int(round(position.y)) + y, 0.0f + int(round(position.z)) + z); // Cube to make
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
	glm::mat4 cam_proj = glm::perspective(75.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	cam_view = glm::lookAt(
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

	// Draws our grid
	DrawGrid();
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

/**
 * Returns true if the position passed is already filled with a block
 */
bool GameWorld::CheckCollision(glm::vec3 point)
{
	if(point == glm::vec3(int(position.x), int(position.y), int(position.z)))
		return true;
	
	return false;
}