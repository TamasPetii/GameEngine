#pragma once
#include "EngineApi.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

struct KeyState
{
	bool current = false;
	bool previous = false;
};

struct ButtonState
{
	bool current = false;
	bool previous = false;
};

class ENGINE_API InputManager
{
public:
	static InputManager* Instance();
	static void Destroy();
	void UpdatePrevious();

	void SetKeyboardKey(int key, bool active);
	bool IsKeyPressed(int key);
	bool IsKeyHeld(int key);
	bool IsKeyReleased(int key);

	void SetMouseButton(int button, bool active);
	bool IsButtonPressed(int key);
	bool IsButtonHeld(int key);
	bool IsButtonReleased(int key);

	void SetMousePosition(double x, double y);
	std::pair<double, double> GetMouseDelta() const;
	std::pair<double, double> GetMousePosition() const;
private:
	InputManager() = default;
	static InputManager* m_Instance;
	std::unordered_map<int, KeyState> m_KeyStatus;
	std::unordered_map<int, ButtonState> m_ButtonStatus;
	
	double m_MouseX = 0.0;
	double m_MouseY = 0.0;
	double m_LastMouseX = 0.0;
	double m_LastMouseY = 0.0;
};

