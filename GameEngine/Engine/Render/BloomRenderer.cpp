#include "BloomRenderer.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"

#include "Render/Geometry/Geometry.h"
#include "Render/OpenGL/ProgramGL.h"
#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

bool  BloomRenderer::useBloom = true;
float BloomRenderer::gamma = 1.25f;
float BloomRenderer::exposure = 1.0f;

void BloomRenderer::Render(std::shared_ptr<Registry> registry)
{
	if (useBloom)
	{
		auto resourceManager = ResourceManager::Instance();
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

		RenderBloomDown(registry);
		RenderBloomUp(registry);
		RenderBloom(registry);
	}
}

void BloomRenderer::RenderBloomDown(std::shared_ptr<Registry> registry)
{
	glDisable(GL_DEPTH_TEST);

	auto resourceManager = ResourceManager::Instance();
	unsigned int srcTexture = resourceManager->GetFbo("Main")->GetTextureID("bloom");
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

void BloomRenderer::RenderBloomUp(std::shared_ptr<Registry> registry)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Bloom");
	auto program = resourceManager->GetProgram("BloomUp");
	fbo->Bind();
	program->Bind();
	program->SetUniform("filterRadius", 0.0001f);

	for (int i = 6 - 1; i > 0; i--)
	{
		std::string name = "bloom" + std::to_string(i);
		unsigned int srcTexture = fbo->GetTextureID(name);
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

void BloomRenderer::RenderBloom(std::shared_ptr<Registry> registry)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	auto resourceManager = ResourceManager::Instance();
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

nlohmann::json BloomRenderer::Serialize()
{

	auto textureManager = TextureManager::Instance();
	nlohmann::json data;
	
	data["useBloom"] = useBloom;
	data["gamma"] = gamma;
	data["exposure"] = exposure;

	return data;
}

void BloomRenderer::DeSerialize(const nlohmann::json& data)
{
	useBloom = data["useBloom"];
	gamma = data["gamma"];
	exposure = data["exposure"];
}