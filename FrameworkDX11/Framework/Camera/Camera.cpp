#include "Camera.h"

Camera::Camera(XMFLOAT4 position, XMFLOAT4 at, XMFLOAT4 up, int windowWidth, int windowHeight, FLOAT nearDepth, FLOAT farDepth, float movementSpeed, CameraType type)
{
	camera._eyeVector = XMLoadFloat4(&position);
	camera._atVector = XMLoadFloat4(&at);
	camera._upVector = XMLoadFloat4(&up);

	camera._windowWidth = windowWidth;
	camera._windowHeight = windowHeight;
	camera._nearDepth = nearDepth;
	camera._farDepth = farDepth;
	camera.type = type;

	camera.movementSpeed = movementSpeed;
	SetViewMatrix();
	SetProjMatrix();
}

Camera::~Camera()
{

}

void Camera::SetViewMatrix()
{
	switch (camera.type)
	{
	case 0: //Lookat
		XMStoreFloat4x4(&camera._view, XMMatrixLookAtLH(camera._eyeVector, camera._atVector, camera._upVector));
		break;
	case 1:  //LookTo
		XMStoreFloat4x4(&camera._view, XMMatrixLookToLH(camera._eyeVector, camera._atVector, camera._upVector));
		break;
	}
}

void Camera::SetProjMatrix()
{
	XMStoreFloat4x4(&camera._projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, camera._windowWidth / (FLOAT)camera._windowHeight, camera._nearDepth, camera._farDepth));
}

XMFLOAT4X4* Camera::GetView()
{
	return &camera._view;
}

XMFLOAT4X4* Camera::GetProjection()
{
	return &camera._projection;
}


XMFLOAT4 Camera::GetPos()
{
	XMStoreFloat4(&camera._eye, camera._eyeVector);

	return camera._eye;
}

XMFLOAT4 Camera::GetAt()
{
	XMStoreFloat4(&camera._at, camera._atVector);

	return camera._at;
}

XMFLOAT4 Camera::GetUp()
{
	XMStoreFloat4(&camera._up, camera._upVector);

	return camera._up;
}


void Camera::Move(float movementSpeed, Direction direction)
{
	float moveSpeed = movementSpeed;
	camera.camRight = XMVector3Cross(camera._atVector, camera._upVector);
	switch (direction)
	{
	case Forward:
		camera._eyeVector = camera._eyeVector + (camera._atVector * moveSpeed);
		SetViewMatrix();
		break;
	case Backward:
		camera._eyeVector = camera._eyeVector - (camera._atVector * moveSpeed);
		SetViewMatrix();
		break;
	case Right:
		camera._eyeVector = camera._eyeVector + (camera.camRight * moveSpeed);
		SetViewMatrix();
		break;
	case Left:
		camera._eyeVector = camera._eyeVector - (camera.camRight * moveSpeed);
		SetViewMatrix();
		break;
	default:
		break;
	}
}

/**********************************************
MARKING SCHEME: Camera
Description: Free-look Camera for moving around the world
***********************************************/

void Camera::Rotate(float rotateAmountX, float rotateAmountY)
{
	camera.camRotationMatrix = XMMatrixRotationRollPitchYaw(rotateAmountX, rotateAmountY, 0);
	
	camera._atVector = XMVector3TransformCoord(camera.DefaultForward, camera.camRotationMatrix);
	camera._atVector = XMVector3Normalize(camera._atVector);

	camera.camRight = XMVector3TransformCoord(camera.DefaultRight, camera.camRotationMatrix);
	camera.camForward = XMVector3TransformCoord(camera.DefaultForward, camera.camRotationMatrix);
	camera._upVector = XMVector3Cross(camera.camForward, camera.camRight);

	SetViewMatrix();
}
