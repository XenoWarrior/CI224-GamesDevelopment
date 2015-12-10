#ifndef CUBEASSET_H
#define CUBEASSET_H

#include <vector>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameAsset.h"

class CubeAsset : public GameAsset
{
	public:
		GLfloat cpos_x, cpos_y, cpos_z;

		CubeAsset(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z);
		~CubeAsset();

		virtual void Draw(GLuint);

		// Gets the cube position
		glm::vec3 GetVec3();


		// Random float value
		float rf();

	private:
		GLuint element_buffer_length, vertex_buffer_length, color_buffer_length;
		GLuint vertex_buffer_token, element_buffer_token, color_buffer_token;
};

#endif // CUBEASSET_H
