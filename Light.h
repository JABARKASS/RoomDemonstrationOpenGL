#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW/glfw3.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	Light(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity);

	void ControlIntensity(GLfloat deltaTime, bool* keys) {
		if (keys[GLFW_KEY_EQUAL]) {
			if (ambientIntensity < 0.7) {
				ambientIntensity += 0.001;
			}
		}
		if (keys[GLFW_KEY_MINUS]) {
			if (ambientIntensity > 0.1) {
				ambientIntensity -= 0.001;
			}
		}
	}

	void UpdateDirection(GLfloat deltaTime, bool* keys);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};