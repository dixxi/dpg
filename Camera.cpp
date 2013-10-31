#include "gl.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <cmath>
#include <iostream>

#include "World.h"

#include "Camera.h"

using namespace std;
using namespace glm;

extern SDL_Window* mainwindow;
extern World world;

Camera::Camera()
{
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;

    yaw = 0.0f;
    pitch = 0.0f;

    moveSens = CAMERA_MOVE_SENS;
    lookSens = CAMERA_LOOK_SENS;
}

Camera& Camera::getInstance()
{
    static Camera camera;

    return camera;
}

Vector3F Camera::getPosition()
{
    return position;
}

void Camera::setPosition(float fx, float fy, float fz)
{
    position.x = fx;
    position.y = fy;
    position.z = fz;
}

void Camera::setPosition(Vector3F v)
{
    position = v;
}

Vector2F Camera::getViewAngles()
{
    Vector2F vec;
    vec.x = pitch;
    vec.y = yaw;
    return vec;
}

Vector3F Camera::getViewVector()
{
    Vector3F v;
    v.x = 1;
    v.y = 0;
    v.z = 0;

    // rotate pitch along -y
    v = rotateY(-pitch, v);

    // rotate yaw along z
    v = rotateZ(yaw, v);

    return v;
}

void Camera::setViewAngles(float pitch, float yaw)
{
    this->pitch = pitch;
    this->yaw = yaw;
}

void Camera::setPosition(Vector2F v)
{
    pitch = v.x;
    yaw = v.y;
}

float Camera::getMoveSens()
{
    return moveSens;
}

void Camera::setMoveSens(float moveSens)
{
    this->moveSens = moveSens;
}

void Camera::setCaptureMouse(bool captureMouse)
{
    this->captureMouse = captureMouse;

    if(captureMouse)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        mouseOrigin.x = x;
        mouseOrigin.y = y;
    }
}

void Camera::update(double interval)
{
	//
	// view
	//

    if(captureMouse)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Update rotation based on mouse input
        yaw += lookSens * (float)(int)(mouseOrigin.x - x);

        // Correct z angle to interval [0;360]
        if(yaw >= 360.0f)
            yaw -= 360.0f;

        if(yaw < 0.0f)
            yaw += 360.0f;

        // Update up down view
        pitch += lookSens * (float)(int)(mouseOrigin.y - y);

        // Correct x angle to interval [-90;90]
        if (pitch < -90.0f)
            pitch = -90.0f;

        if (pitch > 90.0f)
            pitch = 90.0f;

        // Reset cursor
        SDL_WarpMouseInWindow(mainwindow, mouseOrigin.x, mouseOrigin.y);
    }

	//
	// displacement
	//

	Vector3F newPos = position;

    double tmpMoveSens = moveSens * interval;

    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_SPACE]) // UP
    {
        newPos.y += (float)tmpMoveSens;
    }

    if (keys[SDL_SCANCODE_LCTRL]) // DOWN
    {
        newPos.y -= (float)tmpMoveSens;
    }

    // TODO: If strafing and moving reduce speed to keep total move per frame constant
    if (keys[SDL_SCANCODE_W]) // FORWARD
    {
        newPos.x -= (float)(sin(degToRad(yaw)) * tmpMoveSens);
        newPos.z -= (float)(cos(degToRad(yaw)) * tmpMoveSens);
    }

    if (keys[SDL_SCANCODE_S]) // BACKWARD
    {
        newPos.x += (float)(sin(degToRad(yaw)) * tmpMoveSens);
        newPos.z += (float)(cos(degToRad(yaw)) * tmpMoveSens);
    }

    if (keys[SDL_SCANCODE_A]) // LEFT
    {
        newPos.x -= (float)(sin(degToRad(yaw + 90.0f)) * tmpMoveSens);
        newPos.z -= (float)(cos(degToRad(yaw + 90.0f)) * tmpMoveSens);
    }

    if (keys[SDL_SCANCODE_D]) // RIGHT
    {
        newPos.x -= (float)(sin(degToRad(yaw - 90.0f)) * tmpMoveSens);
        newPos.z -= (float)(cos(degToRad(yaw - 90.0f)) * tmpMoveSens);
    }

	// check move
	// ... TODO

    position = world.move(position, BoundingBox(position, position), newPos);
}

mat4 Camera::getViewMatrix() const
{
    mat4 modelViewMatrix = rotate(mat4(1.0), -pitch, vec3(1.0, 0.0, 0.0));
    modelViewMatrix = rotate(modelViewMatrix, -yaw, vec3(0.0, 1.0, 0.0));
    modelViewMatrix = translate(modelViewMatrix, vec3(-position.x, -position.y, -position.z));

    //cout << "Look: " << "Pitch " << pitch << " Yaw " << yaw << " Position " << position << endl;

    return modelViewMatrix;
}
