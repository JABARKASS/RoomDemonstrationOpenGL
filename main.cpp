#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture tableTexture;
Texture closetTexture;
Texture sofaSideTexture;
Texture floorTexture;
Texture poster1Texture;
Texture poster2Texture;
Texture woodTexture;
Texture monitorTexture;
Texture monitorScreenTexture;
Texture wallsTexture;
Texture closet_frontTexture;

Material shinyMaterial;
Material dullMaterial;

Model chair;
Model guitar;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

int curKey(bool* keys) {
	if (keys[GLFW_KEY_1]) { return 1; }
	if (keys[GLFW_KEY_2]) { return 2; }
	if (keys[GLFW_KEY_3]) { return 3; }
	if (keys[GLFW_KEY_4]) { return 4; }
}

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() 
{
	unsigned int walls_indices[] = {		
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		/*2, 3, 7,
		2, 6, 7,*/

		3, 0, 7,
		7, 4, 0,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat walls_vertices[] = {
	//	x      y      z			u	  v			nx	   ny    nz
		0.0f,  0.0f,   0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f,  0.0f,   4.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		6.0f,  0.0f,   4.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		6.0f,  0.0f,   0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f,  3.0f,   0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f,  3.0f,   4.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		6.0f,  3.0f,   4.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		6.0f,  3.0f,   0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int sofaSideIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat sofaSideVertices[]{
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 1.2f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		0.25f, 0.0f, 1.2f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //2
		0.25f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.65f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.65f, 1.2f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //5
		0.25f, 0.65f, 1.2f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //6
		0.25f, 0.65f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int sofaMainIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat sofaMainVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.25f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		0.25f, 0.0f, 1.15f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		2.5f, 0.0f, 1.15f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		2.5f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.25f, 0.45f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //4
		0.25f, 0.45f, 1.15f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //5
		2.5f, 0.45f, 1.15f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		2.5f, 0.45f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int sofaBackIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat sofaBackVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.25f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		0.25f, 0.0f, 0.25f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		2.5f, 0.0f, 0.25f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		2.5f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.25f, 0.8f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //4
		0.25f, 0.8f, 0.25f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //5
		2.5f, 0.8f, 0.25f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		2.5f, 0.8f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int closetIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6,

		0, 4, 9,
		0, 8, 9,

		8, 9, 10,
		9, 11, 10
	};

	GLfloat closetVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		1.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		3.8f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		3.8f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //3

		1.0f, 2.9f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		1.0f, 2.9f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		3.8f, 2.9f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		3.8f, 2.9f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7

		0.2f, 0.0f, 0.8f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //8
		0.2f, 2.9f, 0.8f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //9

		0.2f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //10
		0.2f, 2.9f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //11

	};

	unsigned int tableMainIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat tableMainVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.95f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		2.0f, 0.0f, 0.95f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		2.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.1f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.1f, 0.95f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //5
		2.0f, 0.1f, 0.95f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //6
		2.0f, 0.1f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int tableSideIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat tableSideVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.15f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.9f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		0.07f, 0.0f, 0.9f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.07f, 0.0f, 0.15f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.75f, 0.15f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.75f, 0.9f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //5
		0.07f, 0.75f, 0.9f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //6
		0.07f, 0.75f, 0.15f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int tableClosetIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 6, 2,
		1, 6, 5,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat tableClosetVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.15f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.9f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.6f, 0.0f, 0.9f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //2
		0.6f, 0.0f, 0.15f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.75f, 0.15f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.75f, 0.9f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.6f, 0.75f, 0.9f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //6
		0.6f, 0.75f, 0.15f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int computerIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat computerVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.6f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		0.5f, 0.0f, 0.6f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //2
		0.5f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.53f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.53f, 0.6f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //5
		0.5f, 0.53f, 0.6f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //6
		0.5f, 0.53f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int computerLegIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat computerLegVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.06f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.08f, 0.0f, 0.06f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.08f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.03f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.03f, 0.06f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.08f, 0.03f, 0.06f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.08f, 0.03f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int monitorMainIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat monitorMainVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.08f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.65f, 0.0f, 0.08f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.65f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.4f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.4f, 0.08f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.65f, 0.4f, 0.08f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.65f, 0.4f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int monitorLegIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat monitorLegVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.15f, 0.0f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.15f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.35f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.35f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.15f, 0.35f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.15f, 0.35f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int posterIndices[] = {
		0, 2, 3,
		0, 1, 2
	};

	GLfloat posterVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f,	0.0f,	0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		0.8f,	0.0f,	0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		0.8f,	1.2f,	0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.0f,	1.2f,	0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3
	};

	unsigned int monitorBaseIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat monitorBaseVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.2f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.3f, 0.0f, 0.2f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.3f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.02f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.02f, 0.2f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.3f, 0.02f, 0.2f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.3f, 0.02f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int monitorScreenIndices[] = {
		0, 2, 3,
		0, 1, 2
	};

	GLfloat monitorScreenVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f,	0.0f,	0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //0
		0.63f,	0.0f,	0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //1
		0.63f,	0.37f,	0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.0f,	0.37f,	0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3
	};

	unsigned int lampBaseIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat lampBaseVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.34f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.17f, 0.0f, 0.34f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.17f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.02f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.02f, 0.34f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.17f, 0.02f, 0.34f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.17f, 0.02f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int lampLegIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat lampLegVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.05f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.09f, 0.0f, 0.05f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.09f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.4f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.4f, 0.05f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.09f, 0.4f, 0.05f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.09f, 0.4f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int lampMainIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat lampMainVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.45f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.11f, 0.0f, 0.45f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.11f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.02f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.02f, 0.45f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.11f, 0.02f, 0.45f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.11f, 0.02f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int doorIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat doorVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		1.2f, 0.0f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		1.2f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 2.15f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 2.15f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		1.2f, 2.15f, 0.1f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		1.2f, 2.15f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int pillowIndices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 2, 6,
		1, 5, 6,

		2, 3, 7,
		2, 6, 7,

		3, 0, 7,
		7, 4, 0,

		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6
	};

	GLfloat pillowVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.25f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		0.8f, 0.0f, 0.25f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.8f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		0.0f, 0.55f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		0.0f, 0.55f, 0.25f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		0.8f, 0.55f, 0.25f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		0.8f, 0.55f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7
	};

	unsigned int plinthIndices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12
	};

	GLfloat plinthVertices[] = {
		//	x      y      z			u	  v			nx	   ny    nz
		0.0f, 0.0f, 0.01f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //0
		6.0f, 0.0f, 0.01f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //1
		6.0f, 0.08f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //2
		0.0f, 0.08f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //3

		5.99f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //4
		5.99f, 0.0f, 4.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
		6.0f, 0.08f, 4.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
		6.0f, 0.08f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7

		0.0f, 0.0f, 4.49f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //8
		0.0f, 0.08f, 4.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //9
		6.0f, 0.08f, 4.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //10
		6.0f, 0.0f, 4.49f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //11

		0.0f, 0.08f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //12
		0.00f, 0.08f, 4.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //13
		0.01f, 0.0f, 4.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //14
		0.01f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //15
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	60.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 60.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		60.0f, 60.0f,	0.0f, -1.0f, 0.0f
	};

	//calcAverageNormals(walls_indices, 30, walls_vertices, 32, 8, 5);
	/*calcAverageNormals(sofaSideIndices, 36, sofaSideVertices, 64, 8, 5);
	calcAverageNormals(sofaMainIndices, 36, sofaMainVertices, 64, 8, 5);
	calcAverageNormals(sofaBackIndices, 36, sofaBackVertices, 64, 8, 5);
	calcAverageNormals(closetIndices, 48, closetVertices, 96, 8, 5);*/
	/*calcAverageNormals(tableMainIndices, 36, tableMainVertices, 64, 8, 5);
	calcAverageNormals(tableSideIndices, 36, tableSideVertices, 64, 8, 5);
	calcAverageNormals(tableClosetIndices, 36, tableClosetVertices, 64, 8, 5);*/
	/*calcAverageNormals(computerIndices, 36, computerVertices, 64, 8, 5);
	calcAverageNormals(computerLegIndices, 36, computerLegVertices, 64, 8, 5);*/
	calcAverageNormals(monitorBaseIndices, 36, monitorBaseVertices, 64, 8, 5);
	calcAverageNormals(monitorLegIndices, 36, monitorLegVertices, 64, 8, 5);
	calcAverageNormals(monitorMainIndices, 6, monitorMainVertices, 32, 8, 5);
	calcAverageNormals(posterIndices, 6, posterVertices, 32, 8, 5);
	calcAverageNormals(floorIndices, 6, floorVertices, 32, 8, 5);

	Mesh* walls = new Mesh();
	walls->CreateMesh(walls_vertices, walls_indices, 64, 24);
	meshList.push_back(walls);

	Mesh* floor = new Mesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floor);

	Mesh* sofaSide = new Mesh();
	sofaSide->CreateMesh(sofaSideVertices, sofaSideIndices, 64, 36);
	meshList.push_back(sofaSide);

	Mesh* sofaMain = new Mesh();
	sofaMain->CreateMesh(sofaMainVertices, sofaMainIndices, 64, 36);
	meshList.push_back(sofaMain);

	Mesh* sofaBack = new Mesh();
	sofaBack->CreateMesh(sofaBackVertices, sofaBackIndices, 64, 36);
	meshList.push_back(sofaBack);

	Mesh* closet = new Mesh();
	closet->CreateMesh(closetVertices, closetIndices, 96, 48);
	meshList.push_back(closet);

	Mesh* tableMain = new Mesh();
	tableMain->CreateMesh(tableMainVertices, tableMainIndices, 64, 36);
	meshList.push_back(tableMain);

	Mesh* tableSide = new Mesh();
	tableSide->CreateMesh(tableSideVertices, tableSideIndices, 64, 36);
	meshList.push_back(tableSide);

	Mesh* tableCloset = new Mesh();
	tableCloset->CreateMesh(tableClosetVertices, tableClosetIndices, 64, 36);
	meshList.push_back(tableCloset);

	Mesh* computer = new Mesh();
	computer->CreateMesh(computerVertices, computerIndices, 64, 36);
	meshList.push_back(computer);

	Mesh* computerLeg = new Mesh();
	computerLeg->CreateMesh(computerLegVertices, computerLegIndices, 64, 36);
	meshList.push_back(computerLeg);

	Mesh* poster = new Mesh();
	poster->CreateMesh(posterVertices, posterIndices, 32, 6);
	meshList.push_back(poster);

	Mesh* monitorMain = new Mesh();
	monitorMain->CreateMesh(monitorMainVertices, monitorMainIndices, 64, 36);
	meshList.push_back(monitorMain);

	Mesh* monitorLeg = new Mesh();
	monitorLeg->CreateMesh(monitorLegVertices, monitorLegIndices, 64, 36);
	meshList.push_back(monitorLeg);

	Mesh* monitorBase = new Mesh();
	monitorBase->CreateMesh(monitorBaseVertices, monitorBaseIndices, 64, 36);
	meshList.push_back(monitorBase);

	Mesh* lampBase = new Mesh();
	lampBase->CreateMesh(lampBaseVertices, lampBaseIndices, 64, 36);
	meshList.push_back(lampBase);

	Mesh* lampLeg = new Mesh();
	lampLeg->CreateMesh(lampLegVertices, lampLegIndices, 64, 36);
	meshList.push_back(lampLeg);

	Mesh* lampMain = new Mesh();
	lampMain->CreateMesh(lampMainVertices, lampMainIndices, 64, 36);
	meshList.push_back(lampMain);

	Mesh* monitorScreen = new Mesh();
	monitorScreen->CreateMesh(monitorScreenVertices, monitorScreenIndices, 32, 6);
	meshList.push_back(monitorScreen);

	Mesh* door = new Mesh();
	door->CreateMesh(doorVertices, doorIndices, 64, 36);
	meshList.push_back(door);

	Mesh* plinth = new Mesh();
	plinth->CreateMesh(plinthVertices, plinthIndices, 128, 24);
	meshList.push_back(plinth);

	Mesh* pillow = new Mesh();
	pillow->CreateMesh(pillowVertices, pillowIndices, 64, 36);
	meshList.push_back(pillow);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);	
}

int main() 
{
	printf("'WASD' - move;\n");
	printf("'PageUp/PageDown' - Zoom IN/Zoom OUT;\n\n");
	printf("'1' - handling light source 1;\n");
	printf("'2' - handling light source 2;\n");
	printf("'3' - handling light source 3;\n");
	printf("'4' - handling light source 4;\n\n");
	printf("'handled light source' + 'IJKL' - move handled light source's direction;\n");
	printf("'handled light source' + 'Arrows' - move handled light source's position;\n\n");
	printf("'R' + 'handled light source' + '=' - increase handled light source's RED color intensity;\n");
	printf("'R' + 'handled light source' + '-' - decrease handled light source's RED color intensity;\n");
	printf("'G' + 'handled light source' + '=' - increase handled light source's GREEN color intensity;\n");
	printf("'G' + 'handled light source' + '-' - decrease handled light source's GREEN color intensity;\n");
	printf("'B' + 'handled light source' + '=' - increase handled light source's BLUE color intensity;\n");
	printf("'B' + 'handled light source' + '-' - decrease handled light source's BLUE color intensity;\n\n");
	printf("'N' + 'handled light source' - turn OFF handled light source;\n");
	printf("'Y' + 'handled light source' - turn ON handled light source;\n\n");

	mainWindow = Window(1280, 720);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(6.0f, 1.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.5f, 0.35f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	tableTexture = Texture("Textures/table.png");
	tableTexture.LoadTextureA();
	closetTexture = Texture("Textures/closet.png");
	closetTexture.LoadTextureA();
	sofaSideTexture = Texture("Textures/sofaSide.png");
	sofaSideTexture.LoadTextureA();
	floorTexture = Texture("Textures/floor.png");
	floorTexture.LoadTextureA();
	poster1Texture = Texture("Textures/poster1.png");
	poster1Texture.LoadTextureA();
	poster2Texture = Texture("Textures/poster2.png");
	poster2Texture.LoadTextureA();
	woodTexture = Texture("Textures/wood.png");
	woodTexture.LoadTextureA();
	monitorTexture = Texture("Textures/monitor.png");
	monitorTexture.LoadTextureA();
	monitorScreenTexture = Texture("Textures/monitorScreen.png");
	monitorScreenTexture.LoadTextureA();
	wallsTexture = Texture("Textures/walls.png");
	wallsTexture.LoadTextureA();
	closet_frontTexture = Texture("Textures/closet_front.png");
	closet_frontTexture.LoadTextureA();

	shinyMaterial = Material(32.0f, 64);
	dullMaterial = Material(0.05f, 2);

	chair = Model();
	chair.LoadModel("Models/chair.obj");

	guitar = Model();
	guitar.LoadModel("Models/guitar.obj");

	mainLight = DirectionalLight(0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.8f, 0.8f, 0.7f,
						0.5f, 1.0f,
						2.5f, 2.9f, 1.8f,
						0.5f, 0.2f, 0.1f);
	pointLightCount++;

	/*pointLights[1] = PointLight(1.0f, 1.0f, 0.7f,
		0.3f, 1.0f,
		4.5f, 1.8f, 0.6f,
		0.5f, 0.2f, 0.1f);
	pointLightCount++;*/

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 0.5f,
						0.5f, 1.0f,
						4.528f, 1.87f, 0.55f,
						-0.05f, -1.0f, 0.0f,
						0.3f, 0.2f, 0.1f,
						45.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 0.3f, 0.0f,
						1.5f, 1.0f,
						0.0f, 3.0f, 4.5f,
						0.5f, -0.75f, -0.5f,
						0.3f, 0.2f, 0.1f,
						50.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(0.3f, 0.0f, 1.0f,
						1.5f, 1.0f,
						0.0f, 3.0f, 0.0f,
						0.5f, -0.75f, 0.5f,
						0.3f, 0.2f, 0.1f,
						50.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(85.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		curKey(mainWindow.getsKeys());

		// Get + Handle User Input
		glfwPollEvents();

		projection = glm::perspective(camera.GetFOV(), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (curKey(mainWindow.getsKeys()) == 1) {
			spotLights[1].ControlSpotLight(mainWindow.getsKeys(), spotLights[1], deltaTime, mainWindow);
		};

		if (curKey(mainWindow.getsKeys()) == 2) {
			spotLights[2].ControlSpotLight(mainWindow.getsKeys(), spotLights[2], deltaTime, mainWindow);
		};

		if (curKey(mainWindow.getsKeys()) == 4) {
			spotLights[0].ControlSpotLight(mainWindow.getsKeys(), spotLights[0], deltaTime, mainWindow);
		};

		if (curKey(mainWindow.getsKeys()) == 3) {
			pointLights[0].ControlPointLight(mainWindow.getsKeys(), pointLights[0], deltaTime, mainWindow);
		};

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f);	
		
		//walls
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wallsTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		//walls

		//floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		//floor

		//sofa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.03f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sofaSideTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.03f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sofaSideTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.03f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tableTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.03f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tableTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		//sofa

		//closet
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.2f, 0.0f, 3.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		closet_frontTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		//closet

		//table
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.75f, 0.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.77f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.9f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.901f, 0.55f, 0.02f));
		model = glm::scale(model, glm::vec3(0.99f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.901f, 0.08f, 0.02f));
		model = glm::scale(model, glm::vec3(0.99f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();
		//table

		//computer
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.2f, 0.88f, 0.15f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.2f, 0.85f, 0.15f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[10]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.62f, 0.85f, 0.67f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[10]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.62f, 0.85f, 0.15f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[10]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.2f, 0.85f, 0.67f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[10]->RenderMesh();
		//computer

		//poster DOOM
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.6f, 1.5f, 0.02f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		poster1Texture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[11]->RenderMesh();
		//poster DOOM

		//poster Pulp Fiction
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 1.48f, 0.02f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		poster2Texture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[11]->RenderMesh();
		//poster Pulp Fiction


		//monitor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.4f, 1.0f, 0.15f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(-0.02f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		monitorTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[12]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.65f, 0.85f, 0.05f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		monitorTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[13]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.57f, 0.85f, 0.02f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		monitorTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[14]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.41f, 1.03f, 0.231f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(-0.02f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		monitorScreenTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[18]->RenderMesh();
		//monitor

		//lamp
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.48f, 1.41f, 0.22f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, -0.1f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[15]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.52f, 1.41f, 0.26f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, -0.1f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[16]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.51f, 1.81f, 0.25f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(-0.1f, -0.1f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[17]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.515f, 1.81f, 0.315f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(-0.1f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[17]->RenderMesh();
		//lamp

		//door
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 4.45f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tableTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[19]->RenderMesh();
		//door

		//chair
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.4f, 0.0f, 1.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		chair.RenderModel();
		//chair

		//plinth
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		closetTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[20]->RenderMesh();
		//plinth

		//pillows
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.8f, 0.78f, 0.07f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		closetTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[21]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.67f, 0.78f, 0.07f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		closetTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[21]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 1.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(82.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		closetTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[21]->RenderMesh();
		//pillows

		//guitar
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.48f, 0.0f, 1.5f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		guitar.RenderModel();
		//guitar

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}