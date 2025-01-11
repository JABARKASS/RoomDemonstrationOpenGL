#pragma once
#include "PointLight.h"
#include "Window.h"

class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);

	void TurnSpotLight(GLfloat deltaTime, bool* keys) {
		if (keys[GLFW_KEY_RIGHT]) {
			position.x += 0.05;
		}
		if (keys[GLFW_KEY_LEFT]) {
			position.x -= 0.05;
		}
		if (keys[GLFW_KEY_UP]) {
			position.z -= 0.05;
		}
		if (keys[GLFW_KEY_DOWN]) {
			position.z += 0.05;
		}
		if (keys[GLFW_KEY_L]) {
			direction.x += 0.005;
		}
		if (keys[GLFW_KEY_J]) {
			direction.x -= 0.005;
		}
		if (keys[GLFW_KEY_I]) {
			direction.z -= 0.005;
		}
		if (keys[GLFW_KEY_K]) {
			direction.z += 0.005;
		}
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

	void TurnOffSpotLight(bool* keys) {
		static GLfloat buffer1Spot = this->ambientIntensity;
		static GLfloat buffer2Spot = this->diffuseIntensity;
		if(keys[GLFW_KEY_Y]) {
			this->ambientIntensity = buffer1Spot;
			this->diffuseIntensity = buffer2Spot;
		}

		if (keys[GLFW_KEY_N]) {
			this->ambientIntensity = 0;
			this->diffuseIntensity = 0;
		}
	}

	void ControlSpotLight(bool* keys, SpotLight& spotlight, GLfloat deltaTime, Window& window) {
		spotlight.TurnSpotLight(deltaTime, window.getsKeys());
		spotlight.TurnOffSpotLight(window.getsKeys());
	};

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

