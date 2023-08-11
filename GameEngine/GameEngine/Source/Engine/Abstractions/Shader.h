#pragma once
#include <GLEW/glew.h>
#include <string>
#include <fstream>

class Shader
{
public:
	Shader(unsigned int type, const std::string& path);
	~Shader();

	inline unsigned int GetShaderId() const { return mShaderId; }
private:
	unsigned int mShaderId;
};

class ShaderLayout
{
public:
	ShaderLayout(unsigned int layoutLocation, const std::string& layoutName) : mLayoutLocation(layoutLocation), mLayoutName(layoutName) {}

	inline unsigned int GetLayoutLocation() const { return mLayoutLocation; }
	inline std::string GetLayoutName() const { return mLayoutName; }
private:
	unsigned int mLayoutLocation;
	std::string mLayoutName;
};

