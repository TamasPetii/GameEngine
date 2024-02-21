#include "BloomRenderer.h"

<<<<<<< HEAD
void BloomRenderer::Render(std::shared_ptr<Registry> registry)
{
	if (useBloom)
	{
		auto resourceManager = ResourceManager::Instance();
=======
void BloomRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
{
	if (useBloom)
	{
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
		auto fbo = resourceManager->GetFbo("Bloom");
		fbo->Bind();

		for (int i = 0; i < 6; ++i)
		{
			std::string name = "bloom" + std::to_string(i);
			fbo->BindTexture(name);
			fbo->ActivateTexture(name);
			glViewport(0, 0, fbo->GetTextureSpec(name).width, fbo->GetTextureSpec(name).height);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}

<<<<<<< HEAD
		RenderBloomDown(registry);
		RenderBloomUp(registry);
		RenderBloom(registry);
	}
}

void BloomRenderer::RenderBloomDown(std::shared_ptr<Registry> registry)
{
	glDisable(GL_DEPTH_TEST);

	auto resourceManager = ResourceManager::Instance();
=======
		RenderBloomDown(registry, resourceManager);
		RenderBloomUp(registry, resourceManager);
		RenderBloom(registry, resourceManager);
	}
}

void BloomRenderer::RenderBloomDown(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
{
	glDisable(GL_DEPTH_TEST);

>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	GLuint srcTexture = resourceManager->GetFbo("Main")->GetTextureID("bloom");
	glm::vec2 srcResolution = resourceManager->GetFbo("Main")->GetSize();
	auto fbo = resourceManager->GetFbo("Bloom");
	auto program = resourceManager->GetProgram("BloomDown");
	fbo->Bind();
	program->Bind();

	for (int i = 0; i < 6; i++)
	{
		program->SetUniform("srcResolution", srcResolution);
		program->SetTexture("srcTexture", 0, srcTexture);

		std::string name = "bloom" + std::to_string(i);
		glm::vec2 size = glm::vec2(fbo->GetTextureSpec(name).width, fbo->GetTextureSpec(name).height);
		fbo->ActivateTexture(name);
		fbo->BindTexture(name);
		glViewport(0, 0, size.x, size.y);

		resourceManager->GetGeometry("Cube")->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		resourceManager->GetGeometry("Cube")->UnBind();

		srcTexture = fbo->GetTextureID(name);
		srcResolution = size;
	}

	program->UnBind();

	glEnable(GL_DEPTH_TEST);
}

<<<<<<< HEAD
void BloomRenderer::RenderBloomUp(std::shared_ptr<Registry> registry)
=======
void BloomRenderer::RenderBloomUp(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto fbo = resourceManager->GetFbo("Bloom");
	auto program = resourceManager->GetProgram("BloomUp");
	fbo->Bind();
	program->Bind();
	program->SetUniform("filterRadius", 0.0001f);

	for (int i = 6 - 1; i > 0; i--)
	{
		std::string name = "bloom" + std::to_string(i);
		GLuint srcTexture = fbo->GetTextureID(name);
		glm::vec2 srcResolution = glm::vec2(fbo->GetTextureSpec(name).width, fbo->GetTextureSpec(name).height);

		program->SetUniform("srcResolution", srcResolution);
		program->SetTexture("srcTexture", 0, srcTexture);

		std::string dstName = "bloom" + std::to_string(i - 1);
		fbo->ActivateTexture(dstName);
		fbo->BindTexture(dstName);
		glViewport(0, 0, fbo->GetTextureSpec(dstName).width, fbo->GetTextureSpec(dstName).height);

		resourceManager->GetGeometry("Cube")->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		resourceManager->GetGeometry("Cube")->UnBind();
	}

	program->SetUniform("srcResolution", glm::vec2(fbo->GetTextureSpec("bloom0").width, fbo->GetTextureSpec("bloom0").height));
	program->SetTexture("srcTexture", 0, fbo->GetTextureID("bloom0"));

	auto mainFbo = resourceManager->GetFbo("Main");
	mainFbo->Bind();
	mainFbo->ActivateTexture("bloom");

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	resourceManager->GetGeometry("Cube")->UnBind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	program->UnBind();
}

<<<<<<< HEAD
void BloomRenderer::RenderBloom(std::shared_ptr<Registry> registry)
=======
void BloomRenderer::RenderBloom(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();
	fbo->ActivateTexture("main");

	auto program = resourceManager->GetProgram("Bloom");
	program->Bind();
	program->SetTexture("bloomTexture", 0, fbo->GetTextureID("bloom"));
	program->SetUniform("gamma", gamma);
	program->SetUniform("exposure", exposure);

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	resourceManager->GetGeometry("Cube")->UnBind();

	program->UnBind();
	fbo->UnBind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}