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
		CubeAsset(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z);
		~CubeAsset();
		virtual void Draw(GLuint);

	private:
		GLuint element_buffer_length;
		GLuint vertex_buffer_length;

		GLuint vertex_buffer_token, element_buffer_token;
};

#endif // CUBEASSET_H
