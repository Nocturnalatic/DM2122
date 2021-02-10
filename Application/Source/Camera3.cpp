#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "Assignment2.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    doCamera = true;
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
    view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 80.f; //Rotate Speeds
	static float ZOOM_SPEED = 20.f; //Movement Speeds
	view = (target - position).Normalized();
	if (doCamera)
	{
        if (Application::IsKeyPressed('A'))
        {
            Vector3 right = view.Cross(up).Normalized();
            right.y = 0;

            up = right.Cross(view).Normalized();

            position -= right * (float)dt * ZOOM_SPEED;
            target -= right * (float)dt * ZOOM_SPEED;
        }
        if (Application::IsKeyPressed('D'))
        {
            Vector3 right = view.Cross(up).Normalized();
            right.y = 0;

            up = right.Cross(view).Normalized();

            position += right * (float)dt * ZOOM_SPEED;
            target += right * (float)dt * ZOOM_SPEED;
        }
        if (Application::IsKeyPressed('W'))
        {
            if (position.y >= -14)
            {
                Vector3 right = view.Cross(up).Normalized();
                right.y = 0;

                up = right.Cross(view).Normalized();
                Vector3 temp = view;
                temp.y = 0;
                temp.Normalized();
                position += temp * (float)dt * ZOOM_SPEED;
                target += temp * (float)dt * ZOOM_SPEED;
            }
            else
            {
                position.y = -10;
            }
        }
        if (Application::IsKeyPressed('S'))
        {
            if (position.y >= -14)
            {
                Vector3 right = view.Cross(up).Normalized();
                right.y = 0;

                up = right.Cross(view).Normalized();

                Vector3 temp = view;
                temp.y = 0;
                temp.Normalized();
                position -= temp * (float)dt * ZOOM_SPEED;
                target -= temp * (float)dt * ZOOM_SPEED;
            }
            else
            {
                position.y = -10;
            }
        }
        if (Application::IsKeyPressed(VK_NUMPAD8)) //Look Up | Pitch
        {
            float pitch = CAMERA_SPEED * static_cast<float>(dt);
            Vector3 view = (target - position).Normalized();
            Vector3 right = view.Cross(up);
            right.y = 0;
            right.Normalize();
            up = right.Cross(view).Normalized();
            Mtx44 rotation;
            rotation.SetToRotation(pitch, right.x, right.y, right.z);
            view = rotation * view;
            target = position + view;

        }
        if (Application::IsKeyPressed(VK_NUMPAD5)) //Look Down | -Pitch
        {
            float pitch = -CAMERA_SPEED * static_cast<float>(dt);
            Vector3 view = (target - position).Normalized();
            Vector3 right = view.Cross(up);
            right.y = 0;
            right.Normalize();
            up = right.Cross(view).Normalized();
            Mtx44 rotation;
            rotation.SetToRotation(pitch, right.x, right.y, right.z);
            view = rotation * view;
            target = position + view;
        }
        if (Application::IsKeyPressed(VK_NUMPAD6)) //Look Right | Yaw
        {
            float yaw = -CAMERA_SPEED * static_cast<float>(dt);
            Vector3 view = (target - position).Normalized();
            Mtx44 rotation;
            rotation.SetToRotation(yaw, 0, 1, 0);
            view = rotation * view;
            target = position + view;
            up = rotation * up;
        }
        if (Application::IsKeyPressed(VK_NUMPAD4))
        {
            float yaw = CAMERA_SPEED * static_cast<float>(dt);
            Vector3 view = (target - position).Normalized();
            Mtx44 rotation;
            rotation.SetToRotation(yaw, 0, 1, 0);
            view = rotation * view;
            target = position + view;
            up = rotation * up;
        }
        if (Application::IsKeyPressed('R'))
        {
            Reset();
        }
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}