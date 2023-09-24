#pragma once

namespace OpenGL
{
	class IBindable
	{
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}

