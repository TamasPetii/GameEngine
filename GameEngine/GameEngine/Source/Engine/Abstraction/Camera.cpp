#include "Camera.h"

Camera::Camera(GLuint width, GLuint height)
{
	mWidth = static_cast<GLfloat>(width);
	mHeigth = static_cast<GLfloat>(height);

	mDirVec = glm::normalize(mAt - mEye);
	mYaw = glm::degrees(atan2f(mDirVec.z, mDirVec.x));
	mPitch = glm::degrees(asinf(mDirVec.y));

	mViewMatrix = glm::lookAt(mEye, mAt, mUp);
	mProjMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeigth, mNear, mFar);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	GLfloat currentTime = static_cast<GLfloat>(glfwGetTime());
	mDeltaTime = currentTime - mLastTime;
	mLastTime = currentTime;

	mEye += mForward * mDirVec * mSpeed * mDeltaTime;
	mEye += mSideways * glm::normalize(glm::cross(mDirVec, mUp)) * mSpeed * mDeltaTime;

	mViewMatrix = glm::lookAt(mEye, mEye + mDirVec, mUp);
}

//--------------|Keyboard Events|--------------//

void Camera::Keyboard_ButtonEvent(GLint key, GLint scancode, GLint action, GLint mods)
{
	if (action == GLFW_PRESS)
	{
		Keyboard_PressEvent(key);
	}
	else if (action == GLFW_RELEASE)
	{
		Keyboard_ReleaseEvent(key);
	}
}

void Camera::Keyboard_PressEvent(GLuint key)
{
	if (key == GLFW_KEY_W)
	{
		mForward = 1;
	}
	if (key == GLFW_KEY_S)
	{
		mForward = -1;
	}
	if (key == GLFW_KEY_A)
	{
		mSideways = -1;
	}
	if (key == GLFW_KEY_D)
	{
		mSideways = 1;
	}
	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		mUpWays = 1;
	}
	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		mUpWays = -1;
	}
}

void Camera::Keyboard_ReleaseEvent(GLint key)
{
	if (key == GLFW_KEY_W || key == GLFW_KEY_S)
	{
		mForward = 0;
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_D)
	{
		mSideways = 0;
	}
	if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_LEFT_SHIFT)
	{
		mUpWays = 0;
	}
}

//--------------|Mouse Events|--------------//

void Camera::Mouse_ClickEvent(GLint button, GLint action, GLint mods)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		mIsMovable = true;
	}
	else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		mIsMovable = false;
		mInitCursorPos = true;
	}
}

void Camera::Mouse_MoveEvent(GLfloat xpos, GLfloat ypos)
{
	if (!mIsMovable) return;

	if (mInitCursorPos)
	{
		mCursorStartX = xpos;
		mCursorStartY = ypos;
		mInitCursorPos = false;
	}

	GLfloat offsetX = xpos - mCursorStartX;
	GLfloat offsetY = mCursorStartY - ypos;

	mCursorStartX = xpos;
	mCursorStartY = ypos;

	float sensitivity = 0.2f * (mFov / 60.f);
	offsetX *= sensitivity;
	offsetY *= sensitivity;


	mYaw += offsetX;
	mPitch += offsetY;
	mPitch = glm::clamp(mPitch, -89.f, 89.f);

	glm::vec3 direction(
		cosf(glm::radians(mYaw)) * cosf(glm::radians(mPitch)),
		sinf(glm::radians(mPitch)),
		sinf(glm::radians(mYaw)) * cosf(glm::radians(mPitch))
	);

	dir = direction;

	mAt = mEye + direction;

	mDirVec = glm::normalize(mAt - mEye);
}

void Camera::Mouse_WheelEvent(GLfloat xoffset, GLfloat yoffset)
{
	mFov -= yoffset;
	mFov = glm::clamp(mFov, 1.f, 60.f);

	mProjMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeigth, mNear, mFar);
}

//--------------|Setter|--------------//

void Camera::SetProjMatrix(GLuint width, GLuint height)
{
	if (height == 0) return;

	glViewport(0, 0, width, height);

	mWidth = static_cast<GLfloat>(width);
	mHeigth = static_cast<GLfloat>(height);

	mProjMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeigth, mNear, mFar);
}

void Camera::SetProjMatrix(GLfloat fov, GLuint width, GLuint height)
{
	glViewport(0, 0, width, height);

	mWidth = static_cast<GLfloat>(width);
	mHeigth = static_cast<GLfloat>(height);
	mFov = fov;
	mProjMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeigth, mNear, mFar);
}

void Camera::SetProjMatrix(GLfloat fov, GLuint width, GLuint height, GLfloat near, GLfloat far)
{
	glViewport(0, 0, width, height);

	mWidth = static_cast<GLfloat>(width);
	mHeigth = static_cast<GLfloat>(height);
	mFov = fov;
	mNear = near;
	mFar = far;
	mProjMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeigth, mNear, mFar);
}

void Camera::SetEye(const glm::vec3& eye)
{
	mEye = eye;

	mViewMatrix = glm::lookAt(mEye, mAt, mUp);
	mDirVec = glm::normalize(mAt - mEye);
	mYaw = glm::degrees(atan2f(mDirVec.z, mDirVec.x));
	mPitch = glm::degrees(asinf(mDirVec.y));
}

void Camera::SetAt(const glm::vec3& at)
{
	mAt = at;

	mViewMatrix = glm::lookAt(mEye, mAt, mUp);
	mDirVec = glm::normalize(mAt - mEye);
	mYaw = glm::degrees(atan2f(mDirVec.z, mDirVec.x));
	mPitch = glm::degrees(asinf(mDirVec.y));
}