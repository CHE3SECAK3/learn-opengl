#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Move_Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPEED_UP,
	SLOW_DOWN
};

const float DEF_Z			= 3.0f;

const float FOV_DEF			= 45.0f;
const float FOV_MIN			= 0.0f;
const float FOV_MAX			= 45.0f;

const float PITCH_MIN		= -89.0f;
const float PITCH_MAX		= 89.0f;

const float MOUSE_SENS_DEF	= 0.5f;
const float CAM_SPEED_DEF	= 3.0f;
const float CAM_SPEED_MAX	= 10.0f;
const float CAM_SPEED_INCR	= 0.25f;


class Camera {
private:
	const glm::vec3 upHint = glm::vec3(0.0f, 1.0f, 0.0f);
	float pitch;
	float yaw;
	float mouseSens;
	float camSpeed;
	float fov;

public:
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Up;
	glm::vec3 Right;
	
	Camera()
		: Position(glm::vec3(0.0f, 0.0f, DEF_Z)), Direction(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), Right(glm::vec3(1.0f, 0.0f, 0.0f)),
		  pitch(0.0f), yaw(0.0f), fov(FOV_DEF), mouseSens(MOUSE_SENS_DEF), camSpeed(CAM_SPEED_DEF) {
		updateCamera();
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Direction, Up);
	}

	float getFOV() {
		return fov;
	}

	void processKeyboardInput(Camera_Move_Direction dir, float dt) {
		float frameSpeed = camSpeed * dt;
		if (dir == FORWARD) Position += Direction * frameSpeed;
		if (dir == BACKWARD) Position -= Direction * frameSpeed;
		if (dir == LEFT) Position -= Right * frameSpeed;
		if (dir == RIGHT) Position += Right * frameSpeed;

		if (dir == SPEED_UP) camSpeed += CAM_SPEED_INCR;
		if (dir == SLOW_DOWN) {
			camSpeed -= CAM_SPEED_INCR;
			camSpeed = clipValuef(camSpeed, 0.0f, CAM_SPEED_MAX);
		}
	}

	void processMouseMovementInput(float xoffset, float yoffset) {
		yaw += xoffset * mouseSens;
		pitch += yoffset * mouseSens;
		clipValuef(pitch, -89.0f, 89.0f);

		updateCamera();
	}

	void processScrollwheelInput(float offset) {
		fov -= offset;
		clipValuef(fov, FOV_MIN, FOV_MAX);
	}

private:
	float clipValuef(float val, float min, float  max) {
		return val < min ? min : val > max ? max : val;
	}

	void updateCamera() {
		float pitchRadians = glm::radians(pitch);
		float yawRadians = glm::radians(yaw);

		Direction.y = sin(pitchRadians);
		Direction.x = cos(pitchRadians) * sin(yawRadians);
		Direction.z = cos(pitchRadians) * -cos(yawRadians);
		Direction = glm::normalize(Direction);

		Right = glm::normalize(glm::cross(Direction, upHint));
		Up = glm::normalize(glm::cross(Right, Direction));
	}
};

#endif