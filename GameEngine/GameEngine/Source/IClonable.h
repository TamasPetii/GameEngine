#pragma once

class ICloneable
{
public:
	virtual ICloneable* Clone() const = 0;
};