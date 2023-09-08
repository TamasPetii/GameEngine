#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform2.hpp>

class Camera
{
public:
	Camera(GLuint width, GLuint height);
	~Camera();

	void Update();

	void Keyboard_ButtonEvent(GLint key, GLint scancode, GLint action, GLint mods);
	void Mouse_WheelEvent(GLfloat xoffset, GLfloat yoffset);
	void Mouse_ClickEvent(GLint button, GLint action, GLint mods);
	void Mouse_MoveEvent(GLfloat xpos, GLfloat ypos);

	//Setter
	void SetEye(const glm::vec3& eye);
	void SetAt(const glm::vec3& at);
	void SetProjMatrix(GLuint width, GLuint height);
	void SetProjMatrix(GLfloat fov, GLuint width, GLuint height);
	void SetProjMatrix(GLfloat fov, GLuint width, GLuint height, GLfloat near, GLfloat far);

	//Getter
	inline GLfloat GetWidth() const { return mWidth; }
	inline GLfloat GetHeight() const { return mHeigth; }
	inline glm::mat4 GetProjMatrix() const { return mProjMatrix; }
	inline glm::mat4 GetViewMatrix() const { return mViewMatrix; }
	inline glm::mat4 GetViewProjMatrix() const { return mProjMatrix * mViewMatrix; }
	inline glm::vec3 GetCameraEye() const { return mEye; }
	inline glm::vec3 GetCameraAt() const { return mAt; }
	inline glm::vec3 GetCameraUp() const { return mUp; }
	inline glm::vec3 GetDirVec() const { return mDirVec; }
	inline float GetDirYaw() const { return mYaw; }
	inline float GetDirPitch() const { return mPitch; }
	inline GLfloat& GetSpeed() { return mSpeed; }
	inline void ResetTime() { mLastTime = glfwGetTime(); }

	glm::vec3 dir;
protected:
	void Keyboard_PressEvent(GLuint key);
	void Keyboard_ReleaseEvent(GLint key);
private:
	GLfloat mDeltaTime = 0.f;
	GLfloat mLastTime = 0.f;

	GLfloat mSpeed = 5.f;
	GLfloat mForward = 0.f;
	GLfloat mSideways = 0.f;
	GLfloat mUpWays = 0.f;

	glm::vec3 mEye = glm::vec3(0.f, 1.f, 2.5f);
	glm::vec3 mAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 mUp = glm::vec3(0.f, 1.f, 0.f);

	GLfloat mFov = 60.f;
	GLfloat mWidth = 0.f;
	GLfloat mHeigth = 0.f;
	GLfloat mNear = 0.1f;
	GLfloat mFar = 1000.f;

	glm::vec3 mDirVec = glm::vec3(0.f);
	GLfloat mYaw = 0.f;
	GLfloat mPitch = 0.f;

	GLfloat mCursorStartX = 0.f;
	GLfloat mCursorStartY = 0.f;
	GLboolean mInitCursorPos = true;
	GLboolean mIsMovable = false;

	glm::mat4 mProjMatrix = glm::mat4(1);
	glm::mat4 mViewMatrix = glm::mat4(1);
};