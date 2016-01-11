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
		CubeAsset(glm::vec3, glm::vec3);
		~CubeAsset();

		virtual void Draw(GLuint);

		float rf();
		glm::vec3 GetVec3();

	private:
		glm::vec3 position;
		GLuint element_buffer_length, vertex_buffer_length, color_buffer_length;
		GLuint vertex_buffer_token, element_buffer_token, color_buffer_token;

		/*
		GLfloat diamond_vertex [] {
			-0.5f + position.x,  0.0f + position.y,  0.5f + position.z, //0
			 0.0f + position.x, -0.5f + position.y,  0.0f + position.z, //1
			 0.5f + position.x,  0.0f + position.y,  0.5f + position.z, //2
			 0.0f + position.x,  0.5f + position.y,  0.0f + position.z, //3
			 0.0f + position.x,  0.5f + position.y,  0.0f + position.z, //4
			 0.5f + position.x,  0.0f + position.y, -0.5f + position.z, //5
			 0.0f + position.x,  0.5f + position.y,  0.0f + position.z, //6
			-0.5f + position.x,  0.0f + position.y, -0.5f + position.z, //7
			 0.5f + position.x,  0.0f + position.y, -0.5f + position.z, //8
			 0.0f + position.x,  0.5f + position.y,  0.0f + position.z //9
		};
		GLuint diamond_element [] {
			0,2,1,
			3,2,0,
			2,4,5,
			2,5,1,
			0,1,7,
			6,0,7,
			7,1,8,
			7,8,9
		};*/
};

#endif // CUBEASSET_H
