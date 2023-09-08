#pragma once

class Renderable
{
public:
	virtual void Render() const = 0;
	virtual Renderable* Clone() const = 0;
};