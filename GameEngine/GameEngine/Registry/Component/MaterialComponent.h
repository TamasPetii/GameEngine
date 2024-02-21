#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <Registry/Component/Component.h>
#include <Render/OpenGL/TextureGL.h>

struct MaterialComponent : public Component
{
	bool useBloom{false};
	glm::vec4 color{1.f};
	float shinniness{0.25f};
<<<<<<< HEAD
	glm::vec2 textureScale{ 1.f, 1.f };
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	std::shared_ptr<TextureGL> diffuse;
	std::shared_ptr<TextureGL> normal;
	std::shared_ptr<TextureGL> specular;
	std::shared_ptr<TextureGL> environment;
};

struct MaterialGLSL
{
	MaterialGLSL(const MaterialComponent& component)
	{
		this->color = component.color;
		this->shinniness.x = component.shinniness;
		this->shinniness.y = component.useBloom ? 1 : 0;
<<<<<<< HEAD
		this->scale.x = component.textureScale.x;
		this->scale.y = component.textureScale.y;
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
		this->diffuse = component.diffuse ? component.diffuse->GetTextureHandler() : 0;
		this->normal = component.normal ? component.normal->GetTextureHandler() : 0;
		this->specular = component.specular ? component.specular->GetTextureHandler() : 0;
		this->environment = component.environment ? component.environment->GetTextureHandler() : 0;
	}

	glm::vec4 color;
	glm::vec4 shinniness;
<<<<<<< HEAD
	glm::vec4 scale;
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	GLuint64 diffuse;
	GLuint64 normal;
	GLuint64 specular;
	GLuint64 environment;
};