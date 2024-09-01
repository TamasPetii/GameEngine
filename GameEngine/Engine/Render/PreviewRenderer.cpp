#include "PreviewRenderer.h"

AnimationPreviewType PreviewRenderer::animationPreviewType = AnimationPreviewType::NONE;

void PreviewRenderer::Render(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");
	fbo->Bind();
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT0);

	RenderShapePreviews(registry);
	RenderModelPreviews(registry);
	RenderMaterialPreviews(registry);
	RenderAnimationPreviews(registry, deltaTime);

	fbo->UnBind();
}

void PreviewRenderer::RenderShapePreviews(std::shared_ptr<Registry> registry)
{
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto previewManager = PreviewManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");

	auto program = resourceManager->GetProgram("Preview");

	std::for_each(std::execution::seq, previewManager->RefShapePreviews().begin(), previewManager->RefShapePreviews().end(),
		[&](auto& preview) -> void {
			if (preview.second == nullptr)
			{
				fbo->Clear();
				fbo->Bind();

				//Rendering background
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);

				auto program = resourceManager->GetProgram("PreviewBg");
				program->Bind();
				program->SetTexture("u_bgTexture", 0, textureManager->LoadImageTexture("../Assets/PreviewBG.png")->GetTextureID());

				resourceManager->GetGeometry("Cube")->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				resourceManager->GetGeometry("Cube")->UnBind();

				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);

				//Rendering each built in gemoetry
				auto geometry = resourceManager->GetGeometry(preview.first);
				auto& origin = geometry->GetObbOrigin();
				auto& extents = geometry->GetObbExtents();

				float maxExtent = glm::max(extents.x, extents.y);
				float padding = 2;

				// Setup orthographic projection matrix
				glm::mat4 projectionMatrix = glm::ortho(
					-maxExtent * padding,  // left
					maxExtent * padding,   // right
					-maxExtent * padding,  // bottom
					maxExtent * padding,   // top
					0.01f,                  // near
					3 * extents.z * padding // far
				);

				// Camera remains the same as before
				glm::vec3 cameraPosition = origin + glm::vec3(maxExtent, maxExtent, 2 * extents.z);
				glm::vec3 target = origin;
				glm::vec3 up = glm::vec3(0, 1, 0);
				glm::mat4 viewMatrix = glm::lookAt(cameraPosition, target, up);

				program = resourceManager->GetProgram("Preview");
				program->Bind();

				program->SetUniform("u_renderMode", (GLuint)0);
				program->SetUniform("u_viewProj", projectionMatrix * viewMatrix);
				program->SetUniform("u_eye", cameraPosition);

				geometry->Bind();
				glDrawElements(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
				geometry->UnBind();

				//Creating a new texture and copying frambuffer texture
				TextureSpecGL spec;
				spec.textureType = GL_TEXTURE_2D;
				spec.format = GL_RGB;
				spec.internalFormat = GL_RGB8;
				spec.type = GL_UNSIGNED_BYTE;
				spec.width = fbo->GetSize().x;
				spec.height = fbo->GetSize().y;

				std::shared_ptr<TextureGL> previewTexture = std::make_shared<TextureGL>(spec);
				previewTexture->TextureCopy2D(fbo->GetTextureID("preview"));
				preview.second = previewTexture;
			}
		}
	);
}

void PreviewRenderer::RenderModelPreviews(std::shared_ptr<Registry> registry)
{
	auto modelManager = ModelManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto previewManager = PreviewManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");

	std::for_each(std::execution::seq, previewManager->RefModelPreviews().begin(), previewManager->RefModelPreviews().end(),
		[&](auto& preview) -> void {
			if (preview.second == nullptr)
			{
				fbo->Clear();
				fbo->Bind();

				//Rendering background
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);

				auto program = resourceManager->GetProgram("PreviewBg");
				program->Bind();
				program->SetTexture("u_bgTexture", 0, textureManager->LoadImageTexture("../Assets/PreviewBG.png")->GetTextureID());

				resourceManager->GetGeometry("Cube")->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				resourceManager->GetGeometry("Cube")->UnBind();

				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);

				//Rendering each built in gemoetry
				auto model = modelManager->GetModel(preview.first);
				auto& origin = model->GetObbOrigin();
				auto& extents = model->GetObbExtents();

				float maxExtent = glm::max(extents.x, extents.y);
				float padding = 1.1;

				// Setup orthographic projection matrix
				glm::mat4 projectionMatrix = glm::ortho(
					-maxExtent * padding,  // left
					maxExtent * padding,   // right
					-maxExtent * padding,  // bottom
					maxExtent * padding,   // top
					0.01f,                  // near
					3 * extents.z * padding // far
				);

				// Camera remains the same as before
				glm::vec3 cameraPosition = origin + glm::vec3(0, 0, 2 * extents.z);
				glm::vec3 target = origin;
				glm::vec3 up = glm::vec3(0, 1, 0);
				glm::mat4 viewMatrix = glm::lookAt(cameraPosition, target, up);

				program = resourceManager->GetProgram("Preview");
				program->Bind();
				program->SetUniform("u_renderMode", (GLuint)1);
				program->SetUniform("u_viewProj", projectionMatrix * viewMatrix);
				program->SetUniform("u_eye", cameraPosition);

				model->GetMaterialSsbo()->BindBufferBase(0);
				model->Bind();
				glDrawElements(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
				model->UnBind();

				//Creating a new texture and copying frambuffer texture
				TextureSpecGL spec;
				spec.textureType = GL_TEXTURE_2D;
				spec.format = GL_RGB;
				spec.internalFormat = GL_RGB8;
				spec.type = GL_UNSIGNED_BYTE;
				spec.width = fbo->GetSize().x;
				spec.height = fbo->GetSize().y;

				std::shared_ptr<TextureGL> previewTexture = std::make_shared<TextureGL>(spec);
				previewTexture->TextureCopy2D(fbo->GetTextureID("preview"));
				preview.second = previewTexture;
			}
		}
	);
}

void PreviewRenderer::RenderMaterialPreviews(std::shared_ptr<Registry> registry)
{
	
}

void PreviewRenderer::RenderAnimationPreviews(std::shared_ptr<Registry> registry, float deltaTime)
{
	static float time = 0;
	static float frameRate = 1.f / 60.f;
	time += deltaTime;

	if (time < frameRate)
	{
		return;
	}

	time = 0;

	static AnimationComponent animationComponent;
	static bool animationComponentInited = false;

	if (!animationComponentInited)
	{
		animationComponentInited = true;
		animationComponent.boneTransformSsbo = std::make_shared<ShaderStorageBufferGL>();
	}

	auto modelManager = ModelManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto previewManager = PreviewManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");

	std::for_each(std::execution::seq, previewManager->RefAnimationPreviews().begin(), previewManager->RefAnimationPreviews().end(),
		[&](auto& preview) -> void {
			if (animationPreviewType == AnimationPreviewType::ALL_ANIMATION || 
				(
					animationPreviewType == AnimationPreviewType::ACTIVE_ANIMATION &&
					registry->HasComponent<AnimationComponent>(registry->GetActiveEntity()) &&
					registry->GetComponent<AnimationComponent>(registry->GetActiveEntity()).animation &&
					preview.first == registry->GetComponent<AnimationComponent>(registry->GetActiveEntity()).animation->GetPath()
				)
			)
			{
				fbo->Clear();
				fbo->Bind();

				//Rendering background
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);

				auto program = resourceManager->GetProgram("PreviewBg");
				program->Bind();
				program->SetTexture("u_bgTexture", 0, textureManager->LoadImageTexture("../Assets/PreviewBG.png")->GetTextureID());

				resourceManager->GetGeometry("Cube")->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				resourceManager->GetGeometry("Cube")->UnBind();

				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);

				auto model = modelManager->GetModel(preview.first);
				auto animation = modelManager->GetAnimation(preview.first);

				auto& origin = model->GetObbOrigin();
				auto& extents = model->GetObbExtents();

				float maxExtent = glm::max(extents.x, extents.y);
				float padding = 1.25;

				// Setup orthographic projection matrix
				glm::mat4 projectionMatrix = glm::ortho(
					-maxExtent * padding,  // left
					maxExtent * padding,   // right
					-maxExtent * padding,  // bottom
					maxExtent * padding,   // top
					-1000.f, // near
					1000.f // far
				);

				// Camera remains the same as before
				glm::vec3 cameraPosition = origin + glm::vec3(0, 0, 2 * extents.z);
				glm::vec3 target = origin;
				glm::vec3 up = glm::vec3(0, 1, 0);
				glm::mat4 viewMatrix = glm::lookAt(cameraPosition, target, up);

				program = resourceManager->GetProgram("PreviewAnimation");
				program->Bind();
				program->SetUniform("u_viewProj", projectionMatrix * viewMatrix);
				program->SetUniform("u_eye", cameraPosition);

				animationComponent.time = previewManager->RefAnimationPreviewComponent(preview.first).time;
				animationComponent.animation = animation;
				animationComponent.boneTransforms.clear();
				animationComponent.boneTransforms.resize(animationComponent.animation->GetBoneCount());

				AnimationSystem::CalculateBoneTransforms(animationComponent, frameRate);

				previewManager->RefAnimationPreviewComponent(preview.first).time = animationComponent.time;
				animationComponent.boneTransformSsbo->BufferData(animationComponent.boneTransforms.size() * sizeof(glm::mat4), animationComponent.boneTransforms.data(), GL_DYNAMIC_DRAW);

				animationComponent.boneTransformSsbo->BindBufferBase(0);
				animation->GetVertexBoneSsbo()->BindBufferBase(1);
				model->GetMaterialSsbo()->BindBufferBase(2);
				model->Bind();
				glDrawElements(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
				model->UnBind();

				//Creating a new texture and copying frambuffer texture
				TextureSpecGL spec;
				spec.textureType = GL_TEXTURE_2D;
				spec.format = GL_RGB;
				spec.internalFormat = GL_RGB8;
				spec.type = GL_UNSIGNED_BYTE;
				spec.width = fbo->GetSize().x;
				spec.height = fbo->GetSize().y;

				std::shared_ptr<TextureGL> previewTexture = std::make_shared<TextureGL>(spec);
				previewTexture->TextureCopy2D(fbo->GetTextureID("preview"));
				preview.second = previewTexture;
			}
		}
	);

	resourceManager->GetProgram("PreviewAnimation")->UnBind();
}