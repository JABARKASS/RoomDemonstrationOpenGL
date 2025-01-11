#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

void Light::UpdateDirection(GLfloat deltaTime, bool* keys) {
	GLfloat speed = 1.0f * deltaTime;
	if (keys[GLFW_KEY_RIGHT]) {
		direction.x -= speed;
	}
	if (keys[GLFW_KEY_LEFT]) {
		direction.x += speed;
	}
	if (keys[GLFW_KEY_UP]) {
		direction.y -= speed;
	}
	if (keys[GLFW_KEY_DOWN]) {
		direction.y += speed;
	}
	direction = glm::normalize(direction);
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
		glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
		glUniform1f(ambientIntensityLocation, ambientIntensity);

		glUniform3f(directionLocation, direction.x, direction.y, direction.z);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}

Light::~Light()
{
}
