#include <glm/ext.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#ifndef GAMECAMERA_H
#define GAMECAMERA_H

class GameCamera {
	public:
		GameCamera(glm::vec3);
		
		void MoveCamera(glm::vec2, glm::vec2);
		void CameraController(int);
		void UpdateFacingDirection();
		void ChangeBlockDist(int);

		glm::vec3 GetOffset();
		glm::vec3 DrawCamera();

	private:
		GLfloat camera_speed = 0.1;
		GLfloat camera_x = 0.0;
		GLfloat camera_y = 0.0;

		glm::vec3 z_direction;
		glm::vec3 x_direction;
		glm::vec3 position;
		glm::mat4 cam_view;

		int block_dist = 3;
		double point = 0.39375;
		std::string f_pos = "N";
};

#endif