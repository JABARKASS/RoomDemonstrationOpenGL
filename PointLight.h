#pragma once
#include "Light.h"
#include "Window.h"

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat positionLocation,
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

	void TurnPointLight(GLfloat deltaTime, bool* keys) {
		if (keys[GLFW_KEY_R] && keys[GLFW_KEY_EQUAL]) {
			if (colour.r <= 1.0f) {
				colour.r += 0.01;
			}
		}
		if (keys[GLFW_KEY_R] && keys[GLFW_KEY_MINUS]) {
			if (colour.r >= 0.0f) {
				colour.r -= 0.01;
			}
		}
		if (keys[GLFW_KEY_G] && keys[GLFW_KEY_EQUAL]) {
			if (colour.g <= 1.0f) {
				colour.g += 0.01;
			}
		}
		if (keys[GLFW_KEY_G] && keys[GLFW_KEY_MINUS]) {
			if (colour.g >= 0.0f) {
				colour.g -= 0.01;
			}
		}
		if (keys[GLFW_KEY_B] && keys[GLFW_KEY_EQUAL]) {
			if (colour.b <= 1.0f) {
				colour.b += 0.01;
			}
		}
		if (keys[GLFW_KEY_B] && keys[GLFW_KEY_MINUS]) {
			if (colour.b >= 0.0f) {
				colour.b -= 0.01;
			}
		}
	};

	void TurnOffPointLight(bool* keys) {
		static GLfloat bufferPoint = this->ambientIntensity;
		if (keys[GLFW_KEY_Y]) {
			this->ambientIntensity = bufferPoint;
		}

		if (keys[GLFW_KEY_N]) {
			this->ambientIntensity = 0;
		}
	}

	void ControlPointLight(bool* keys, PointLight& pointlight, GLfloat deltaTime, Window& window) {
		pointlight.TurnOffPointLight(window.getsKeys());
		pointlight.TurnPointLight(deltaTime, window.getsKeys());
	};

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};

